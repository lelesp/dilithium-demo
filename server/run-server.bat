@echo off
REM ============================================================
REM  ML-DSA-65 signature server - native Windows launcher
REM  Requires: Python 3.11+ on PATH (https://www.python.org/)
REM  No Docker and no Redis are required for the demo.
REM ============================================================
setlocal
cd /d "%~dp0"

where python >nul 2>nul
if errorlevel 1 (
    echo [ERROR] Python not found on PATH. Install Python 3.11+ from https://www.python.org/ and retry.
    pause
    exit /b 1
)

if not exist ".venv" (
    echo [*] Creating virtual environment...
    python -m venv .venv
    if errorlevel 1 (
        echo [ERROR] Failed to create virtual environment.
        pause
        exit /b 1
    )
)

call ".venv\Scripts\activate.bat"

echo [*] Installing dependencies...
python -m pip install --upgrade pip >nul
pip install -r requirements.txt
if errorlevel 1 (
    echo [ERROR] Failed to install dependencies.
    pause
    exit /b 1
)

echo.
echo [*] Starting server on http://localhost:8000
echo     Swagger UI: http://localhost:8000/docs
echo     Press Ctrl+C to stop.
echo.
uvicorn app.main:app --host 0.0.0.0 --port 8000

endlocal
