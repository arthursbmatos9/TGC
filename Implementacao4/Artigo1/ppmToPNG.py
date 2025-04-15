import os
from PIL import Image

def converter_ppm_para_png(diretorio):
    # Obter todos os arquivos no diretório
    for arquivo in os.listdir(diretorio):
        # Verificar se o arquivo é uma imagem .ppm
        if arquivo.endswith('.ppm'):
            caminho_entrada = os.path.join(diretorio, arquivo)
            caminho_saida = os.path.join("./convertido/", arquivo.replace('.ppm', '.png'))
            
            # Abrir a imagem
            imagem = Image.open(caminho_entrada)
            
            # Salvar como PNG
            imagem.save(caminho_saida, 'PNG')
            print(f'Imagem convertida: {caminho_entrada} -> {caminho_saida}')

# Caminho do diretório contendo os arquivos .ppm
diretorio_imagens = './segments'

# Converter todas as imagens .ppm para .png
converter_ppm_para_png(diretorio_imagens)
