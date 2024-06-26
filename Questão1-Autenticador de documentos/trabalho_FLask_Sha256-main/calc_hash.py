import hashlib
import os

def calcular_hash_arquivo(nome_arquivo):
    """
    Calcula o hash SHA-256 de um arquivo.
    
    Args:
        nome_arquivo (str): O nome do arquivo.

    Returns:
        str: O hash SHA-256 do arquivo.
    """
    # Inicializa o objeto hash SHA-256
    hasher = hashlib.sha256()

    # Lê o conteúdo do arquivo em blocos de 4KB e atualiza o hash
    with open(nome_arquivo, 'rb') as arquivo:
        for bloco in iter(lambda: arquivo.read(4096), b''):
            hasher.update(bloco)

    # Retorna a representação hexadecimal do hash
    return hasher.hexdigest()


def limpar_diretorio_verificacao():
    # Obtém a lista de arquivos no diretório uploads_verificacao
    arquivos = os.listdir('trabalho_FLask_Sha256-main\\uploads_verif')
    
    # Percorre cada arquivo e remove-o
    for arquivo in arquivos:
        caminho_arquivo = os.path.join('trabalho_FLask_Sha256-main\\uploads_verif', arquivo)
        os.remove(caminho_arquivo)
    
limpar_diretorio_verificacao()

