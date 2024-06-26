
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
from config import KEY_SIZE, BLOCK_SIZE
import socket
import pickle


def decrypt_message(ciphertext, key, iv):
    cipher = AES.new(key, AES.MODE_CBC, iv)
    decrypted_message = unpad(cipher.decrypt(ciphertext), BLOCK_SIZE)
    return decrypted_message

# Exemplo de uso
if __name__ == "__main__":
    # Configurações de conexão
    HOST = 'localhost'
    PORT = 12345  
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        conn, addr = s.accept()
        with conn:
            print('Conectado por', addr)
            # Receber dados
            data = conn.recv(4096)
            if data:
                # Carregar dados recebidos
                data = pickle.loads(data)
                print(data)
                key = data['key']
                mensagem_cifrada = data['mensagem_cifrada']
                iv = data['iv']
                # Chave AES usada no remetente
                key = key  # Substitua pela chave usada no remetente

                # IV usada no remetente
                iv = iv  # Substitua pelo IV usado no remetente

                # Mensagem criptografada (obtida do remetente)
                ciphertext = mensagem_cifrada  # Substitua pela mensagem criptografada real

                # Descriptografar a mensagem
                decrypted_message = decrypt_message(ciphertext, key, iv)

                print("Mensagem Descriptografada:", decrypted_message.decode())
