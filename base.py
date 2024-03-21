def codificar_base64(texto):
    """
- Os dados que vão ser codificados são divididos em blocos de 3 bytes, cada bloco de 3 bytes é dividido em quatro grupos de 6 bits.
- Cada grupo de 6 bits é convertido em um caractere ASCII de um conjunto predefinido de 64 caracteres (ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/).
- Padding: Se o número de bytes na entrada não for um múltiplo de 3, adicionamos um ou dois bytes de preenchimento para garantir que o número total de bytes seja um múltiplo de 3. Esses bytes de preenchimento são representados pelo " = ".
    """
    base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
    texto_bytes = texto.encode('utf-8')
    resultado = ""
    i = 0

    while i < len(texto_bytes):
        bloco = texto_bytes[i:i+3]

        if len(bloco) < 3:
            bloco += b'\x00' * (3 - len(bloco))

        indice1 = bloco[0] >> 2
        indice2 = ((bloco[0] & 0x03) << 4) | (bloco[1] >> 4)
        indice3 = ((bloco[1] & 0x0F) << 2) | (bloco[2] >> 6)
        indice4 = bloco[2] & 0x3F

        resultado += base64_chars[indice1] + base64_chars[indice2] + base64_chars[indice3] + base64_chars[indice4]

        i += 3

    return resultado

def decodificar_base64(base64_string):
    """
- Os caracteres ASCII são convertidos de volta para valores de 6 bits e os valores de 6 bits são combinados para formar blocos de 24 bits.
- Esses blocos de 24 bits são agrupados novamente e os bytes originais são reconstruídos.
- Caso bytes de preenchimento tenham sido adicionados durante a codificação, eles são removidos durante a decodificação.
    """
    base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
    resultado = bytearray()
    i = 0

    while i < len(base64_string):
        bloco = [base64_chars.index(base64_string[i]),
                 base64_chars.index(base64_string[i + 1]),
                 base64_chars.index(base64_string[i + 2]),
                 base64_chars.index(base64_string[i + 3])]

        byte1 = (bloco[0] << 2) | (bloco[1] >> 4)
        byte2 = ((bloco[1] & 0x0F) << 4) | (bloco[2] >> 2)
        byte3 = ((bloco[2] & 0x03) << 6) | bloco[3]

        resultado.extend([byte1, byte2, byte3])

        i += 4

    return resultado.decode('utf-8').rstrip('\x00')

def main():
    """A Base64 é um método que converte dados binários em uma representação ASCII, usando um conjunto de 64 caracteres imprimíveis. """
    # Teste da função de codificação
    texto_original = "Man"
    texto_codificado = codificar_base64(texto_original)
    print("Texto original:", texto_original)
    print("Texto codificado em Base64:", texto_codificado)

    # Teste da função de decodificação
    texto_decodificado = decodificar_base64(texto_codificado)
    print("Texto decodificado:", texto_decodificado)

if __name__ == "__main__":
    main()