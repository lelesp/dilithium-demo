"""Standalone entry point for the ML-DSA signature server.

This module is the target for PyInstaller so the server can ship as a single
self-contained executable (no Python, no pip, no Redis required). It runs the
FastAPI app via uvicorn by passing the app object directly (not an import
string) so it works inside a frozen bundle.
"""
import multiprocessing
import os

import uvicorn

from app.main import app

HOST = os.environ.get("HOST", "127.0.0.1")
PORT = int(os.environ.get("PORT", "8000"))


def main() -> None:
    print(f"ML-DSA signature server starting on http://{HOST}:{PORT}")
    print(f"Swagger UI: http://{HOST}:{PORT}/docs")
    print("Press Ctrl+C to stop.")
    uvicorn.run(app, host=HOST, port=PORT)


if __name__ == "__main__":
    # Required so PyInstaller one-file builds do not re-spawn the bundle when
    # any dependency uses multiprocessing.
    multiprocessing.freeze_support()
    main()
