from dilithium_py.ml_dsa import ML_DSA_44, ML_DSA_65, ML_DSA_87

# Supported ML-DSA (FIPS 204) parameter sets, keyed by their canonical names.
# These names match the liboqs algorithm identifiers used by the client, so a
# package signed here can be verified there without any extra translation.
ALGORITHMS = {
    "ML-DSA-44": ML_DSA_44,
    "ML-DSA-65": ML_DSA_65,
    "ML-DSA-87": ML_DSA_87,
}

DEFAULT_ALGORITHM = "ML-DSA-65"


class DilithiumCrypto:
    ALGORITHM = DEFAULT_ALGORITHM
    SUPPORTED = tuple(ALGORITHMS.keys())

    @staticmethod
    def _impl(algorithm: str):
        impl = ALGORITHMS.get(algorithm)
        if impl is None:
            raise ValueError(
                f"Unsupported algorithm '{algorithm}'. "
                f"Choose one of: {', '.join(ALGORITHMS)}"
            )
        return impl

    @staticmethod
    def generate_keypair(algorithm: str = DEFAULT_ALGORITHM):
        pk, sk = DilithiumCrypto._impl(algorithm).keygen()
        return pk, sk

    @staticmethod
    def sign(sk: bytes, message: bytes, algorithm: str = DEFAULT_ALGORITHM) -> bytes:
        return DilithiumCrypto._impl(algorithm).sign(sk, message)

    @staticmethod
    def verify(
        pk: bytes, message: bytes, signature: bytes, algorithm: str = DEFAULT_ALGORITHM
    ) -> bool:
        return DilithiumCrypto._impl(algorithm).verify(pk, message, signature)
