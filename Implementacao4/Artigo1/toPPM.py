import sys
from PIL import Image

def converter_para_ppm(caminho_imagem_entrada, caminho_imagem_saida):
    # Carregar a imagem
    imagem = Image.open(caminho_imagem_entrada)
    
    # Converter para o modo RGB se necessário
    if imagem.mode != 'RGB':
        imagem = imagem.convert('RGB')
    
    # Salvar como PPM
    imagem.save(caminho_imagem_saida, 'PPM')

    print(f'Imagem salva como PPM em: {caminho_imagem_saida}')

if __name__ == "__main__":
    # Verificar se o argumento foi fornecido
    if len(sys.argv) < 2:
        print("Uso: python converter_para_ppm.py <caminho_da_imagem_entrada>")
        sys.exit(1)
    
    # Obter o caminho da imagem de entrada do argumento
    caminho_imagem_entrada = sys.argv[1]
    
    # Definir o nome da imagem de saída
    caminho_imagem_saida = "imagem.ppm"
    
    # Converter a imagem para PPM
    converter_para_ppm(caminho_imagem_entrada, caminho_imagem_saida)
