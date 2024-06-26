import os
import shutil
import pywintypes
import win32file
import win32con
import time


def get_creation_time(file_path): # Define a data da criação
    if os.path.exists(file_path):
        creation_timestamp = os.path.getctime(file_path)
        creation_time = pywintypes.Time(creation_timestamp)
        return creation_time
    else:
        return None



def copy_file_with_creation_time(source_path, destination_path): # Faço uma cópia do arquivo original e depoois
    try:
        # Copia o arquivo normalmente
        shutil.copy2(source_path, destination_path)
        # Obtém a data de criação original do arquivo de origem
        creation_time = os.path.getctime(source_path)
        # Define a data de modificação para o arquivo de destino
        os.utime(destination_path, (creation_time, creation_time))
        return True
    except Exception as e:
        print("Erro ao copiar o arquivo:", e)
        return False
    
def get_last_modified_time(file_path):
    if os.path.exists(file_path):
        last_modified_timestamp = os.path.getmtime(file_path)
        last_modified_time = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(last_modified_timestamp))
        return last_modified_time
    else:
        return "Arquivo não encontrado"

# Exemplo de uso
file_path = 'teste.docx'
creation_time = get_creation_time(file_path)
print("O arquivo Original  foi criado em:", creation_time)


source_path = 'teste.docx'
destination_path = 'teste/teste.docx'
if copy_file_with_creation_time(source_path, destination_path):
    print("Arquivo copiado com sucesso, mantendo a data de criação original.")
else:
    print("Erro ao copiar o arquivo")


file_path = 'teste/teste.docx'
last_time_modified = get_last_modified_time(file_path)
print("ultima modificação em:", last_time_modified)

# mandamos dai a data de criação do arquivo original e a data de modificação o arquivo copiado