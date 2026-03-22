import redis
from typing import Optional
from app.config import get_settings


class RedisStorage:
    def __init__(self):
        settings = get_settings()
        self.client = redis.Redis(
            host=settings.redis_host,
            port=settings.redis_port,
            db=settings.redis_db,
            decode_responses=True
        )
    
    def _key(self, key_id: str, suffix: str) -> str:
        return f"dilithium:{key_id}:{suffix}"
    
    def save_keypair(self, key_id: str, public_key: bytes, private_key: bytes) -> None:
        pipe = self.client.pipeline()
        pipe.set(self._key(key_id, "public_key"), public_key.hex())
        pipe.set(self._key(key_id, "private_key"), private_key.hex())
        pipe.set(self._key(key_id, "algorithm"), "Dilithium3")
        pipe.execute()
    
    def get_public_key(self, key_id: str) -> Optional[str]:
        return self.client.get(self._key(key_id, "public_key"))
    
    def get_private_key(self, key_id: str) -> Optional[str]:
        return self.client.get(self._key(key_id, "private_key"))
    
    def get_algorithm(self, key_id: str) -> Optional[str]:
        return self.client.get(self._key(key_id, "algorithm"))
    
    def key_exists(self, key_id: str) -> bool:
        return self.client.exists(self._key(key_id, "public_key")) > 0
    
    def delete_keypair(self, key_id: str) -> None:
        pipe = self.client.pipeline()
        pipe.delete(self._key(key_id, "public_key"))
        pipe.delete(self._key(key_id, "private_key"))
        pipe.delete(self._key(key_id, "algorithm"))
        pipe.execute()
    
    def ping(self) -> bool:
        try:
            return self.client.ping()
        except redis.ConnectionError:
            return False
