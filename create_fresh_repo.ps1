Write-Host "========================================================================" -ForegroundColor Cyan
Write-Host "     Creating Standalone GitHub Repository for HyperRAM                 " -ForegroundColor Cyan
Write-Host "========================================================================" -ForegroundColor Cyan

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptDir

# 1. Initialize local git in hyper_ram folder
if (-not (Test-Path ".git")) {
    Write-Host "[*] Initializing fresh Git repository..." -ForegroundColor Yellow
    git init -b main
}

# Configure local git author identity
git config user.name "peter14l"
git config user.email "peter14l@users.noreply.github.com"

# 2. Stage and commit all files
Write-Host "[*] Staging all files in hyper_ram..." -ForegroundColor Yellow
git add -A
git commit -m "feat(init): initial commit of HyperRAM hardware-accelerated compressed memory controller IP and 24x7 runtime"

# 3. Check if gh CLI is installed & authenticated
if (Get-Command "gh" -ErrorAction SilentlyContinue) {
    Write-Host "[*] GitHub CLI (gh) detected. Creating public repository..." -ForegroundColor Green
    
    $repoDescription = "Hardware-Accelerated Compressed Memory Controller IP & 24x7 Real-Time Zero-Swap Engine for Consumer & Cloud Hardware (BDI Parallel Decompressor in <5ns)"
    
    gh repo create hyper-ram --public --description "$repoDescription" --source=. --remote=origin --push
    
    Write-Host "`n[SUCCESS] HyperRAM repository created and pushed to GitHub!" -ForegroundColor Green
} else {
    Write-Host "[!] GitHub CLI (gh) not found in PATH. Please run 'gh auth login' or install GitHub CLI." -ForegroundColor Red
}
