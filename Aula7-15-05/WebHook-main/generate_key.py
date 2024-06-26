# generate_key.py
from cryptography.fernet import Fernet

# Gera e salva a chave
key = Fernet.generate_key()
with open("secret.key", "wb") as key_file:
    key_file.write(key)
