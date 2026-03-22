from dilithium_py.ml_dsa import ML_DSA_65


class DilithiumCrypto:
    ALGORITHM = "ML-DSA-65"
    
    @staticmethod
    def generate_keypair():
        pk, sk = ML_DSA_65.keygen()
        return pk, sk
    
    @staticmethod
    def sign(sk: bytes, message: bytes) -> bytes:
        signature = ML_DSA_65.sign(sk, message)
        return signature
    
    @staticmethod
    def verify(pk: bytes, message: bytes, signature: bytes) -> bool:
        return ML_DSA_65.verify(pk, message, signature)
