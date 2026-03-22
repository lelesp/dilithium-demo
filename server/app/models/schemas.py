from pydantic import BaseModel


class HealthResponse(BaseModel):
    status: str
    algorithm: str = "ML-DSA-65"
