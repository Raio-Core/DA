#!/usr/bin/env python3
"""
Generate SVG badges and charts for README.

Requires:
    pip install requests matplotlib numpy

Run:
    python scripts/generate_badges.py
"""

import os
import sys
import json
import requests
from datetime import datetime, timedelta
from collections import Counter

REPO_OWNER = "Raio-Core"
REPO_NAME = "DA"
GITHUB_API = "https://api.github.com"

def get_headers():
    token = os.environ.get("GITHUB_TOKEN")
    headers = {"Accept": "application/vnd.github.v3+json"}
    if token:
        headers["Authorization"] = f"token {token}"
    return headers

def fetch_commits(since_days=365):
    """Fetch commit history for the last year."""
    since = (datetime.now() - timedelta(days=since_days)).isoformat()
    url = f"{GITHUB_API}/repos/{REPO_OWNER}/{REPO_NAME}/commits"
    params = {"since": since, "per_page": 100}
    
    commits = []
    while url:
        resp = requests.get(url, headers=get_headers(), params=params)
        if resp.status_code != 200:
            print(f"Error fetching commits: {resp.status_code}")
            break
        commits.extend(resp.json())
        # Pagination
        url = resp.links.get("next", {}).get("url")
        params = None  # Only use params for first request
    return commits

def fetch_languages():
    """Fetch language breakdown."""
    url = f"{GITHUB_API}/repos/{REPO_OWNER}/{REPO_NAME}/languages"
    resp = requests.get(url, headers=get_headers())
    if resp.status_code == 200:
        return resp.json()
    return {}

def fetch_contributors():
    """Fetch contributors."""
    url = f"{GITHUB_API}/repos/{REPO_OWNER}/{REPO_NAME}/contributors"
    params = {"per_page": 100, "anon": "true"}
    resp = requests.get(url, headers=get_headers(), params=params)
    if resp.status_code == 200:
        return resp.json()
    return []

def generate_commit_activity_svg(commits, output_path):
    """Generate commit activity chart as SVG."""
    try:
        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot as plt
        import matplotlib.dates as mdates
        from matplotlib.ticker import MaxNLocator
    except ImportError:
        print("matplotlib not installed, skipping commit activity chart")
        return

    # Group commits by week
    weekly = Counter()
    for c in commits:
        date_str = c["commit"]["author"]["date"][:10]
        dt = datetime.strptime(date_str, "%Y-%m-%d")
        week_start = dt - timedelta(days=dt.weekday())
        weekly[week_start] += 1

    # Fill missing weeks with 0
    if weekly:
        min_week = min(weekly.keys())
        max_week = max(weekly.keys())
        current = min_week
        while current <= max_week:
            if current not in weekly:
                weekly[current] = 0
            current += timedelta(weeks=1)

    weeks = sorted(weekly.keys())
    counts = [weekly[w] for w in weeks]

    # Create plot
    fig, ax = plt.subplots(figsize=(10, 3), facecolor='#0d1117')
    ax.set_facecolor('#0d1117')
    
    # Bar chart
    bars = ax.bar(weeks, counts, width=6, color='#58a6ff', edgecolor='none', alpha=0.8)
    
    # Highlight current week
    if weeks:
        current_week = datetime.now() - timedelta(days=datetime.now().weekday())
        if current_week in weekly:
            idx = weeks.index(current_week)
            bars[idx].set_color('#f78166')

    ax.set_xlim(weeks[0] - timedelta(days=3), weeks[-1] + timedelta(days=3))
    ax.set_ylim(0, max(counts) * 1.3 if counts else 1)
    
    # Styling
    ax.xaxis.set_major_locator(mdates.MonthLocator())
    ax.xaxis.set_major_formatter(mdates.DateFormatter('%b'))
    ax.yaxis.set_major_locator(MaxNLocator(integer=True))
    
    ax.tick_params(colors='#8b949e', labelsize=8)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_color('#30363d')
    ax.spines['bottom'].set_color('#30363d')
    ax.grid(axis='y', color='#30363d', alpha=0.5)
    
    plt.tight_layout()
    plt.savefig(output_path, format='svg', facecolor='#0d1117', bbox_inches='tight')
    plt.close()
    print(f"Generated {output_path}")

def generate_languages_svg(languages, output_path):
    """Generate language breakdown as SVG."""
    try:
        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot as plt
    except ImportError:
        print("matplotlib not installed, skipping languages chart")
        return

    if not languages:
        return

    # Filter and sort
    total = sum(languages.values())
    sorted_langs = sorted(languages.items(), key=lambda x: x[1], reverse=True)[:8]
    
    labels = [lang for lang, _ in sorted_langs]
    sizes = [bytes / total * 100 for _, bytes in sorted_langs]
    
    # Colors for common languages
    color_map = {
        'C++': '#f34b7d',
        'C': '#555555',
        'C#': '#178600',
        'Python': '#3572A5',
        'CMake': '#6d8086',
        'HLSL': '#aace60',
        'ShaderLab': '#5a4cdb',
        'GLSL': '#4da8da',
    }
    colors = [color_map.get(lang, '#8b949e') for lang in labels]

    fig, ax = plt.subplots(figsize=(8, 4), facecolor='#0d1117')
    ax.set_facecolor('#0d1117')
    
    wedges, texts, autotexts = ax.pie(
        sizes, labels=labels, colors=colors,
        autopct='%1.1f%%', startangle=90,
        textprops={'color': '#c9d1d9', 'fontsize': 9},
        wedgeprops={'edgecolor': '#0d1117', 'linewidth': 2}
    )
    
    for autotext in autotexts:
        autotext.set_color('#0d1117')
        autotext.set_fontweight('bold')
    
    ax.set_title('Language Breakdown', color='#c9d1d9', fontsize=12, pad=20)
    
    plt.tight_layout()
    plt.savefig(output_path, format='svg', facecolor='#0d1117', bbox_inches='tight')
    plt.close()
    print(f"Generated {output_path}")

def generate_contributors_svg(contributors, output_path):
    """Generate contributors visualization as SVG."""
    if not contributors:
        return

    # Simple grid of avatars
    max_contributors = min(len(contributors), 20)
    cols = 5
    rows = (max_contributors + cols - 1) // cols
    
    avatar_size = 40
    spacing = 10
    width = cols * (avatar_size + spacing) + spacing
    height = rows * (avatar_size + spacing) + spacing + 40  # +40 for title

    svg = f'''<?xml version="1.0" encoding="UTF-8"?>
<svg width="{width}" height="{height}" xmlns="http://www.w3.org/2000/svg">
  <rect width="100%" height="100%" fill="#0d1117"/>
  <text x="{width/2}" y="25" font-family="system-ui" font-size="14" fill="#c9d1d9" text-anchor="middle" font-weight="600">Contributors</text>
'''
    
    for i, contrib in enumerate(contributors[:max_contributors]):
        row = i // cols
        col = i % cols
        x = spacing + col * (avatar_size + spacing)
        y = 35 + row * (avatar_size + spacing)
        
        avatar_url = contrib.get("avatar_url", "")
        login = contrib.get("login", "unknown")
        contributions = contrib.get("contributions", 0)
        
        svg += f'''
  <g>
    <a href="https://github.com/{login}" target="_blank">
      <circle cx="{x + avatar_size/2}" cy="{y + avatar_size/2}" r="{avatar_size/2}" fill="#161b22"/>
      <image x="{x}" y="{y}" width="{avatar_size}" height="{avatar_size}" 
             href="{avatar_url}" clip-path="url(#avatarClip{i})"/>
    </a>
    <defs>
      <clipPath id="avatarClip{i}">
        <circle cx="{x + avatar_size/2}" cy="{y + avatar_size/2}" r="{avatar_size/2}"/>
      </clipPath>
    </defs>
    <text x="{x + avatar_size/2}" y="{y + avatar_size + 12}" 
          font-family="system-ui" font-size="8" fill="#8b949e" text-anchor="middle">
      {login[:12]}{'...' if len(login) > 12 else ''}
    </text>
  </g>
'''
    
    svg += '</svg>'
    
    with open(output_path, 'w') as f:
        f.write(svg)
    print(f"Generated {output_path}")

def main():
    print(f"Fetching data for {REPO_OWNER}/{REPO_NAME}...")
    
    commits = fetch_commits()
    print(f"Fetched {len(commits)} commits")
    
    languages = fetch_languages()
    print(f"Fetched {len(languages)} languages")
    
    contributors = fetch_contributors()
    print(f"Fetched {len(contributors)} contributors")
    
    assets_dir = os.path.join(os.path.dirname(__file__), '..', '.github', 'assets')
    os.makedirs(assets_dir, exist_ok=True)
    
    generate_commit_activity_svg(commits, os.path.join(assets_dir, 'commit-activity.svg'))
    generate_languages_svg(languages, os.path.join(assets_dir, 'languages.svg'))
    generate_contributors_svg(contributors, os.path.join(assets_dir, 'contributors.svg'))
    
    print("Done!")

if __name__ == "__main__":
    main()