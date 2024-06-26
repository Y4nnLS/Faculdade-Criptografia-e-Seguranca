import requests
import json
from cryptography.fernet import Fernet

# Carrega a chave de criptografia
with open("secret.key", "rb") as key_file:
    secret_key = key_file.read()

cipher_suite = Fernet(secret_key)

webhook_url = 'https://localhost:5000/webhook'

data = {
    'event': 'test_event',
    'data': 'This is a test'
}

# Criptografa os dados
encrypted_data = cipher_suite.encrypt(json.dumps(data).encode()).decode()

# Corrompe os dados criptografados
corrupted_data = encrypted_data[:-1] + 'X'

print(f'Dados criptografados a serem enviados: {corrupted_data}')

payload = {
    'data': corrupted_data
}

try:
    response = requests.post(webhook_url, json=payload, verify='cert.pem')
    print(f'Status Code: {response.status_code}')
    print(f'Response: {response.json()}')
except requests.exceptions.RequestException as e:
    print(f'Request Error: {e}')
except ValueError as e:
    print(f'Erro ao processar a resposta: {e}')
