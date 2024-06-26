import sqlite3
import pandas as pd
#Cria banco de dados caso ele não exista
def criar_banco_de_dados():
    # Conectar ao banco de dados ou criar se não existir
    conexao = sqlite3.connect('trabalho_FLask_Sha256-main\\bd\\trabalhoFlaskSha256.db')

    # Criar um cursor para interagir com o banco de dados
    cursor = conexao.cursor()

    # Criar a tabela arquivos
    cursor.execute('''CREATE TABLE IF NOT EXISTS arquivos
                    (id INTEGER PRIMARY KEY AUTOINCREMENT,
                    hash TEXT NOT NULL, arquivo TEXT NOT NULL)''')

    # Salvar as alterações e fechar a conexão com o banco de dados
    conexao.commit()
    conexao.close()

    print("Banco de dados criado com sucesso.")

#adiciona o hash e o nome do arquivo ao banco
def adicionar_valores(hash, nome_arquivo):
    conexao = sqlite3.connect('trabalho_FLask_Sha256-main\\bd\\trabalhoFlaskSha256.db')
    cursor = conexao.cursor()
        
    cursor.execute("INSERT INTO arquivos (hash, arquivo) VALUES (?, ?)", (hash, nome_arquivo))
    # Salvar as alterações e fechar a conexão com o banco de dados
    conexao.commit()
    conexao.close()
    print(f"Hash {hash} salvo com sucesso!")
    return "foi inserido com sucesso!"

#seleciona um hash especifico do banco de dados, caso ele não exita, retorna None
def selecionar_hash_do_banco(hash):
    conexao = sqlite3.connect('trabalho_FLask_Sha256-main\\bd\\trabalhoFlaskSha256.db')
    cursor = conexao.cursor()
    arquivo = cursor.execute(f"SELECT * FROM arquivos where hash = (?)", (hash,))
    arquivo = arquivo.fetchone()
    conexao.close()
    return arquivo

def selecionar_todos_os_arquivos():
    conexao = sqlite3.connect('trabalho_FLask_Sha256-main\\bd\\trabalhoFlaskSha256.db')
    cursor = conexao.cursor()
    arquivos = cursor.execute(f"SELECT * FROM arquivos")

    # Verifica se há algum dado retornado da consulta
    dados = arquivos.fetchall()
    if not dados:
        conexao.close()
        return None

    arquivos = pd.read_sql_query("SELECT * FROM arquivos", conexao)
    conexao.close()
    return arquivos.to_dict(orient='records')


def verifica_existencia_hash_reg(hash, nome_arquivo):
    conexao = sqlite3.connect('trabalho_FLask_Sha256-main\\bd\\trabalhoFlaskSha256.db')
    cursor = conexao.cursor()
    cursor.execute("SELECT COUNT(*) FROM arquivos WHERE hash = ?", (hash,))
    count = cursor.fetchone()[0]

    if count > 0:
        return "já existe no banco de dados. Não é possivel adicionar novamente!"
    else:
        return adicionar_valores(hash, nome_arquivo)