from flask import Flask, request, jsonify
import ssl
from cryptography.fernet import Fernet, InvalidToken

# Carrega a chave de criptografia
with open("secret.key", "rb") as key_file:
    secret_key = key_file.read()

cipher_suite = Fernet(secret_key)

app = Flask(__name__)

@app.route('/webhook', methods=['POST'])
def webhook():
    encrypted_data = request.json.get('data')
    print(f'Dados criptografados recebidos: {encrypted_data}')
    
    try:
        # Descriptografa os dados
        decrypted_data = cipher_suite.decrypt(encrypted_data.encode()).decode()
        print(f'Dados descriptografados: {decrypted_data}')
        return jsonify({'status': 'success'}), 200
    except InvalidToken as e:
        print(f'Erro de descriptografia: {e}')
        return jsonify({'status': 'error', 'message': 'Decryption failed'}), 400
    except Exception as e:
        print(f'Erro desconhecido: {e}')
        return jsonify({'status': 'error', 'message': str(e)}), 500

if __name__ == '__main__':
    context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    context.load_cert_chain(certfile='cert.pem', keyfile='key.pem')
    app.run(host='0.0.0.0', port=5000, ssl_context=context)
