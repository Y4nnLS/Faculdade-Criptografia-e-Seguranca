
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad
from config import KEY_SIZE, BLOCK_SIZE
import socket
import pickle

def encrypt_message(message, key):
    cipher = AES.new(key, AES.MODE_CBC)
    message_bytes = message.encode('utf-8')  # Converter a mensagem para bytes
    ciphertext = cipher.encrypt(pad(message_bytes, BLOCK_SIZE))
    iv = cipher.iv
    return ciphertext, iv

# Exemplo de uso
if __name__ == "__main__":

    # Configurações de conexão
    HOST = 'localhost'
    PORT = 12345  

    # Criar uma chave AES com o comprimento correto
    key = get_random_bytes(KEY_SIZE)

    # Mensagem a ser criptografada
    message = "Esta é uma mensagem secreta!"

    # Criptografar a mensagem
    ciphertext, iv = encrypt_message(message, key)

    # Preparar dados para envio
    data = {
        'key': key,
        'mensagem_cifrada': ciphertext,
        'iv': iv
    }

    # Estabelecer conexão e enviar mensagem cifrada
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(pickle.dumps(data))
    print("Mensagem Criptografada:", ciphertext)
    print("IV:", iv)
