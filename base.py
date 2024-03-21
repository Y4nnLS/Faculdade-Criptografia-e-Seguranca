def codificar_base64(texto):
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


# Teste da função de codificação
texto_original = "Man"
texto_codificado = codificar_base64(texto_original)
print("Texto original:", texto_original)
print("Texto codificado em Base64:", texto_codificado)

# Teste da função de decodificação
texto_decodificado = decodificar_base64(texto_codificado)
print("Texto decodificado:", texto_decodificado)
