<h1 align='center'>
    <p>Criptografia Simétrica e Assimétrica</p>
</h1>

## 🙋‍♂️ Equipe de desenvolvedores
<table align='center'>
  <tr>
    <td align="center">
        <img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/101208372?v=4" width="100px;" alt=""/><br /><sub><b><a href="https://github.com/Y4nnLS">Yann Lucas</a></b></sub></a><br />👨‍🚀</a></td>
    <td align="center">
        <img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/60533993?v=4" width="100px;" alt=""/><br /><sub><b><a href="https://github.com/Ypsiloon">Felipe Pinheiro</a></b></sub></a><br />👨‍🚀</td>
  </table>

Tabela de conteúdos
====================

- [Sobre](#📘-sobre)
- [Inicialização dos sistemas](#inicialização-dos-sistemas)
- [Criptografia Simétrica](#criptografia-simétrica)
    - [Imports](#imports)
- [Criptografia Assimétrica](#criptografia-assimétrica)
    - [Imports](#imports-1)
- [Comparação de pontos forte e fracos](#comparação-de-pontos-fortes-e-fracos)
- [Eficiência e Eficácia](#eficiência-e-eficácia)
- [Tecnologias](#🛠-tecnologias)
## 📘 Sobre

Este projeto visa desenvolver uma aplicação para comunicação segura, garantindo a confidencialidade das mensagens trocadas entre remetente e destinatário. A seguir, são apresentadas as soluções propostas para alcançar esse objetivo:

## Inicialização dos sistemas
1. Abra dois terminais.
2. No primeiro terminal, execute o arquivo destinatario.py.
3. No segundo terminal, execute o arquivo remetente.py.
4. Ao voltar para o primeiro terminal, teremos a mensagem descriptografada.
##

# Criptografia Simétrica

Nesta abordagem, a comunicação segura é alcançada através do uso de criptografia simétrica, onde remetente e destinatário compartilham uma mesma chave secreta para criptografar e descriptografar as mensagens.

- Funcionamento: O remetente utiliza a chave compartilhada para criptografar a mensagem antes de enviá-la. No destino, a mesma chave é utilizada para decifrar a mensagem recebida.
- Implementação: A aplicação consiste em um algoritmo de criptografia simétrica, como AES (Advanced Encryption Standard), onde remetente e destinatário devem possuir a mesma chave para garantir a segurança da comunicação.

### Imports:
```py
destinatario.py

    from Crypto.Cipher import AES
    from Crypto.Util.Padding import unpad
    from config import KEY_SIZE, BLOCK_SIZE
    import socket
    import pickle
```
```py
remetente.py

    from Crypto.Cipher import AES
    from Crypto.Random import get_random_bytes
    from Crypto.Util.Padding import pad
    from config import KEY_SIZE, BLOCK_SIZE
    import socket
    import pickle
```
# Criptografia Assimétrica

Nesta abordagem, a comunicação segura é alcançada através do uso de criptografia assimétrica, onde remetente e destinatário possuem chaves distintas: uma chave pública e uma chave privada.

- Funcionamento: O remetente utiliza a chave pública do destinatário para criptografar a mensagem antes de enviá-la. No destino, o destinatário utiliza sua chave privada correspondente para decifrar a mensagem recebida.
- Implementação: A aplicação consistirá em um sistema de criptografia assimétrica, utilizando algoritmos como RSA (Rivest-Shamir-Adleman), onde cada usuário possui um par de chaves pública e privada.

### Imports:
```py
destinatario.py

    import socket
    import pickle
    from rsa import decrypt, generate_keypair
    from config import p, q
```
```py
remetente.py

    import socket
    import pickle
    from rsa import generate_keypair, encrypt
    from config import p, q
```
```py
rsa.py

    import random
```
##
# Comparação de Pontos Fortes e Fracos

### Chaves Simétricas:

- Pontos Fortes: Eficiente em termos de desempenho e velocidade de processamento. Requer menos recursos computacionais.
- Pontos Fracos: Necessita de um canal seguro para compartilhar a chave simétrica. Se a chave for comprometida, todas as comunicações anteriores e futuras estão em risco.

### Chaves Assimétricas:

- Pontos Fortes: Elimina a necessidade de um canal seguro para compartilhar chaves, pois as chaves públicas podem ser distribuídas abertamente. Maior segurança em relação à exposição das chaves.
- Pontos Fracos: Mais lento em termos de desempenho em comparação com a criptografia simétrica. Requer mais recursos computacionais.


# Eficiência e Eficácia

Em termos de eficiência econômica, a criptografia simétrica é geralmente mais vantajosa devido à sua simplicidade e menor demanda por recursos computacionais. No entanto, em termos de segurança eficaz, a criptografia assimétrica oferece uma vantagem significativa, especialmente em ambientes onde o canal de comunicação não é totalmente confiável.

Conclusão: A escolha entre chaves simétricas e assimétricas dependerá das necessidades específicas de segurança, desempenho e eficiência econômica do sistema em questão.


### 🛠 Tecnologias

As seguintes ferramentas foram usadas na construção do projeto:
| Logo  | Tecnologia |
| ----- | ---------- |
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/python/python-original.svg" height='30px' align='center' /> | [Python](https://www.python.org)  |
|<img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/python/python-original.svg" height='30px' align='center'/> |[Socket](https://docs.python.org/3/library/socket.html)| 
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/python/python-original.svg" height='30px' align='center'> |[Pickle](https://docs.python.org/3/library/pickle.html)| 
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/python/python-original.svg" height='30px' align='center'> |[Random](https://docs.python.org/pt-br/3/library/random.html)| 
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/python/python-original.svg" height='30px' align='center'> |[PyCryptoDome](https://www.pycryptodome.org)| 
