import requests
import json
from cryptography.fernet import Fernet

# Carrega a chave de criptografia
with open("secret.key", "rb") as key_file:
    secret_key = key_file.read()

cipher_suite = Fernet(secret_key)

# Tentando conectar-se a '127.0.0.1' em vez de 'localhost'
webhook_url = 'https://127.0.0.1:5000/webhook'

data = {
    'event': 'test_event',
    'data': 'This is a test'
}

# Criptografa os dados
encrypted_data = cipher_suite.encrypt(json.dumps(data).encode()).decode()

print(f'Dados criptografados a serem enviados: {encrypted_data}')

payload = {
    'data': encrypted_data
}

try:
    response = requests.post(webhook_url, json=payload, verify='cert.pem')
    print(f'Status Code: {response.status_code}')
    print(f'Response: {response.json()}')
except requests.exceptions.SSLError as e:
    print(f'SSL Error: {e}')
