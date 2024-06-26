from flask import Flask, render_template, request, url_for, redirect, send_file
from flask_wtf import FlaskForm
from wtforms import FileField, SubmitField
from werkzeug.utils import secure_filename
from wtforms.validators import InputRequired
import bancoDeDados
import os
from calc_hash import calcular_hash_arquivo, limpar_diretorio_verificacao


app = Flask(__name__)
app.config['SECRET_KEY'] = 'v6Fre$zu7ZJHfcMn356#8c'
app.config['UPLOAD_FOLDER'] = 'trabalho_FLask_Sha256-main\\uploads'
app.config['VERIF_FOLDER'] = 'trabalho_FLask_Sha256-main\\uploads_verif'

# Verifica se o diretório de upload existe, se não, cria
if not os.path.exists(app.config['UPLOAD_FOLDER']):
    os.makedirs(app.config['UPLOAD_FOLDER'])
    
if not os.path.exists(app.config['VERIF_FOLDER']):
    os.makedirs(app.config['VERIF_FOLDER'])
    
bancoDeDados.criar_banco_de_dados()

class UploadFileForm(FlaskForm):
    file = FileField("File", validators=[InputRequired()])
    submit = SubmitField("Upload File")

@app.route('/', methods=['GET',"POST"])
@app.route('/home', methods=['GET',"POST"])
def home():
    return render_template('index.html')







@app.route('/registro', methods=['GET',"POST"])
def registro():
    form = UploadFileForm()
    if form.validate_on_submit():
        file = form.file.data   # Variável que vai armazenar o arquivo
        print(f'NOMEEEEEEEEEEEEEEEEEE: {file.filename}')    # Só pra testar se tá chegando certo... E talvez pra verificar que tipo de arquivo é
        filepath = os.path.join(app.config['UPLOAD_FOLDER'], secure_filename(file.filename))
        file.save(filepath)
        print(f'pathhhhhhhhhhhhhhhh: {filepath}')    # Só pra testar se tá chegando certo... E talvez pra verificar que tipo de arquivo é

        # A instância do método para a transformação do arquivo em Hash deve ser colocado aqui!
        hash = calcular_hash_arquivo(filepath) # arquivo já transformado em hash

        resultado  = bancoDeDados.verifica_existencia_hash_reg(hash, secure_filename(file.filename)) #método que adiciona o hash ao banco de dados
        return render_template('registrado.html', resultado = resultado)
    return render_template('registro.html', form=form)
    
    
    
    
    
    
@app.route('/validacao', methods=['GET',"POST"])
def validacao():
    form = UploadFileForm()
    if form.validate_on_submit():
        file = form.file.data   # Variável que vai armazenar o arquivo
        print(file.filename)    # Só pra testar se tá chegando certo... E talvez pra verificar que tipo de arquivo é
        filepath = os.path.join(app.config['VERIF_FOLDER'], secure_filename(file.filename))
        file.save(filepath)
        # A instância do método para a transformação do arquivo em Hash deve ser colocado aqui!
        hash = calcular_hash_arquivo(filepath) # arquivo já transformado em hash

        resultadoDoBanco =  bancoDeDados.selecionar_hash_do_banco(hash) #método para verificar se o hash existe ou não no banco, caso não exista retorna um None
        resultado = "Válido" if resultadoDoBanco != None else "Inválido" #verifica se o valor retornado do banco é válido ou não

        limpar_diretorio_verificacao()

        return render_template('resultado.html', resultado=resultado)
    return render_template('validacao.html', form=form)

@app.route('/arquivos')
def arquivos():
    arquivos = bancoDeDados.selecionar_todos_os_arquivos() # Recebe um dicionário contendo todos os arquivos registrados no banco de dados
    print(arquivos)
    return render_template('arquivos.html', arquivos=arquivos)

@app.route('/baixar_arquivo/<arquivo>')
def baixar_arquivo(arquivo):
    diretorio = 'uploads' #pasta na qual estão os arquivos a serem baixados
    # hash = calcular_hash_arquivo(arquivo)
    # arquivo = bancoDeDados.selecionar_hash_do_banco(hash)
    print(f'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA: {diretorio}')
    print(f'BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB: {arquivo}')
    return send_file(os.path.join(diretorio, arquivo), as_attachment=True) # envia para o usuario o arquivo selecionado




if __name__ == '__main__':
    app.run(debug=True)
    
    
