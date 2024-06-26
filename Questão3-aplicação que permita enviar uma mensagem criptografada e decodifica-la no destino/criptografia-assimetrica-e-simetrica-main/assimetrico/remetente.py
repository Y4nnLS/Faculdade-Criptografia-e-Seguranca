import socket
import pickle
from rsa import generate_keypair, encrypt
from config import p, q

# Configurações de conexão
HOST = 'localhost'
PORT = 12345  

# Gerar chave pública
public_key, _ = generate_keypair(p, q)

# Mensagem a ser enviada
mensagem = "TESTE"

# Criptografar mensagem
mensagem_cifrada = encrypt(public_key, mensagem)

# Preparar dados para envio
data = {
    'public_key': public_key,
    'mensagem_cifrada': mensagem_cifrada
}

# Estabelecer conexão e enviar mensagem cifrada
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(pickle.dumps(data))
