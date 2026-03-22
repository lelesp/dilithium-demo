from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from app.routes import router

app = FastAPI(
    title="ML-DSA-65 Signature Service",
    description="""
## Post-Quantum Digital Signature Service

This service provides post-quantum digital signatures using **ML-DSA-65** algorithm (CRYSTALS-Dilithium).

### Features
- **Digital Signatures**: Sign files with post-quantum security
- **Signature Verification**: Verify signatures locally using liboqs

### Algorithm Details
- **Algorithm**: ML-DSA-65
- **Security Level**: ~192 bits
- **Public Key Size**: ~1952 bytes (3864 hex chars)
- **Signature Size**: ~3293 bytes (6586 hex chars)

### Usage Flow
1. Sign file via `POST /api/sign` → receive ZIP with file + `.sig`
2. `.sig` file contains `public_key`, `signature`, `algorithm`, and `timestamp`
3. Verify signature locally using liboqs

### Documentation
- Swagger UI: `/docs`
- ReDoc: `/redoc`
    """,
    version="1.0.0",
    docs_url="/docs",
    redoc_url="/redoc",
    docs_title="ML-DSA-65 API Documentation",
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(router)


@app.get("/")
async def root():
    return {
        "service": "ML-DSA-65 Signature Service",
        "algorithm": "ML-DSA-65",
        "version": "1.0.0",
        "documentation": {
            "swagger_ui": "/docs",
            "redoc": "/redoc"
        },
        "endpoints": {
            "sign_file": "POST /api/sign",
            "health": "GET /api/health"
        }
    }
