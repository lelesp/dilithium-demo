import io
import zipfile
import os
import logging
from fastapi import APIRouter, UploadFile, File, Form, HTTPException
from fastapi.responses import StreamingResponse

from app.crypto import DilithiumCrypto
from app.crypto.dilithium import DEFAULT_ALGORITHM

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

router = APIRouter(prefix="/api", tags=["dilithium"])


@router.post("/sign")
async def sign_file(
    file: UploadFile = File(...),
    algorithm: str = Form(DEFAULT_ALGORITHM),
):
    if algorithm not in DilithiumCrypto.SUPPORTED:
        raise HTTPException(
            status_code=400,
            detail=(
                f"Unsupported algorithm '{algorithm}'. "
                f"Choose one of: {', '.join(DilithiumCrypto.SUPPORTED)}"
            ),
        )
    try:
        logger.info(f"Signing with algorithm: {algorithm}")
        pk, sk = DilithiumCrypto.generate_keypair(algorithm)

        pk_hex = pk.hex() if isinstance(pk, bytes) else pk
        sk_hex = sk.hex() if isinstance(sk, bytes) else sk

        logger.info(f"pk_hex length: {len(pk_hex)}")

        sk_bytes = bytes.fromhex(sk_hex)
        file_content = await file.read()

        logger.info(f"file_content length: {len(file_content)}")

        signature = DilithiumCrypto.sign(sk_bytes, file_content, algorithm)
        
        if isinstance(signature, str):
            signature_hex = signature
        else:
            signature_hex = signature.hex()
        
        logger.info(f"signature_hex length: {len(signature_hex)}")
        
        original_filename = os.path.basename(file.filename)
        
        if isinstance(signature, str):
            sig_hex = signature
        else:
            sig_hex = signature.hex()
        
        zip_buffer = io.BytesIO()
        with zipfile.ZipFile(zip_buffer, 'w', zipfile.ZIP_DEFLATED) as zipf:
            zipf.writestr(original_filename, file_content)
            zipf.writestr(f"{original_filename}.sig", sig_hex)
            zipf.writestr(f"{original_filename}.pub", pk_hex)
        
        zip_buffer.seek(0)
        
        safe_filename = original_filename.encode('utf-8').decode('latin-1')
        content_disposition = f"attachment; filename*=UTF-8''signed_{safe_filename}.zip"
        
        return StreamingResponse(
            zip_buffer,
            media_type="application/zip",
            headers={
                "Content-Disposition": content_disposition
            }
        )
    except Exception as e:
        logger.error(f"Sign error: {str(e)}")
        import traceback
        logger.error(traceback.format_exc())
        raise HTTPException(status_code=500, detail=f"Sign error: {str(e)}")


@router.get("/health")
async def health_check():
    return {
        "status": "healthy",
        "default_algorithm": DEFAULT_ALGORITHM,
        "supported_algorithms": list(DilithiumCrypto.SUPPORTED),
    }
