import socket
import pickle
from rsa import decrypt, generate_keypair
from config import p, q

# Configurações de conexão
HOST = 'localhost'
PORT = 12345  

# Gerar chave privada do destinatário
_, private_key = generate_keypair(p, q)

# Estabelecer conexão
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
            public_key = data['public_key']
            mensagem_cifrada = data['mensagem_cifrada']
            
            # Descriptografar mensagem usando a chave privada do destinatário
            mensagem_decifrada = decrypt(private_key, mensagem_cifrada)
            print("Mensagem recebida:", mensagem_decifrada)
