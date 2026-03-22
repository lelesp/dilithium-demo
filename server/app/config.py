from pydantic_settings import BaseSettings
from functools import lru_cache


class Settings(BaseSettings):
    redis_host: str = "localhost"
    redis_port: int = 6379
    redis_db: int = 0
    keys_dir: str = "/app/keys"
    
    class Config:
        env_prefix = ""


@lru_cache()
def get_settings() -> Settings:
    return Settings()
