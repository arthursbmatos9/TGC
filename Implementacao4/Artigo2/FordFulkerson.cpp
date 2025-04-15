#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <limits>

/**
 * @brief Representa um pixel com componentes de cores RGB.
 * 
 * @param r Componente de cor vermelha.
 * @param g Componente de cor verde.
 * @param b Componente de cor azul.
 */
struct Pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    
    Pixel() : r(0), g(0), b(0) {}
    Pixel(unsigned char red, unsigned char green, unsigned char blue) 
        : r(red), g(green), b(blue) {}
};

/**
 * @brief Implementa um grafo para análise de fluxo com suporte ao algoritmo de Ford-Fulkerson e segmentação de imagem.
 * 
 * @param V Número de vértices no grafo.
 * @param graph Matriz de adjacência representando o grafo original.
 * @param residualGraph Matriz de adjacência representando a capacidade residual.
 * @param vertices Vetor de vértices com informações associadas (pixels).
 */
class Graph {
private:
    int V;
    std::vector<std::vector<int> > graph;
    std::vector<std::vector<int> > residualGraph;
    std::vector<Pixel> vertices;

    /**
     * @brief Realiza uma busca em largura no grafo residual para encontrar um caminho de aumento.
     * 
     * @param source Vértice de origem.
     * @param sink Vértice de destino.
     * @param parent Vetor que armazena o caminho de aumento.
     * @return true se existir um caminho de origem a destino; caso contrário, false.
     */
    bool bfs(int source, int sink, std::vector<int>& parent) {
        std::vector<bool> visited(V, false);
        std::queue<int> q;

        q.push(source);
        visited[source] = true;
        parent[source] = -1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < V; ++v) {
                if (!visited[v] && residualGraph[u][v] > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }

        return visited[sink];
    }

public:
    Graph(int vertexCount) : V(vertexCount) {
        graph.resize(V, std::vector<int>(V, 0));
        residualGraph.resize(V, std::vector<int>(V, 0));
        vertices.resize(V);
    }

    /**
     * @brief Adiciona um vértice ao grafo com suas informações associadas.
     * 
     * @param id Identificador do vértice.
     * @param pixel Informações do pixel associadas ao vértice.
     */
    void addVertex(int id, const Pixel& pixel) {
        vertices[id] = pixel;
    }

    /**
     * @brief Adiciona uma aresta ao grafo com uma capacidade especificada.
     * 
     * @param u Vértice de origem.
     * @param v Vértice de destino.
     * @param capacity Capacidade da aresta.
     */
    void addEdge(int u, int v, int capacity) {
        graph[u][v] += capacity;
        residualGraph[u][v] += capacity;
    }

    /**
     * @brief Calcula o fluxo máximo entre dois vértices utilizando o algoritmo de Ford-Fulkerson.
     * 
     * @param source Vértice de origem.
     * @param sink Vértice de destino.
     * @return Fluxo máximo entre origem e destino.
     */
    int fordFulkerson(int source, int sink) {
        int maxFlow = 0;
        std::vector<int> parent(V);

        while (bfs(source, sink, parent)) {
            int pathFlow = std::numeric_limits<int>::max();

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = std::min(pathFlow, residualGraph[u][v]);
            }

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residualGraph[u][v] -= pathFlow;
                residualGraph[v][u] += pathFlow;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }

    /**
     * @brief Realiza a segmentação do grafo usando o corte mínimo com base no fluxo máximo.
     * 
     * @param source Vértice de origem.
     * @param sink Vértice de destino.
     * @return Vetor de dois conjuntos, representando os vértices no corte mínimo.
     */
    std::vector<std::vector<int> > minCutSegmentation(int source, int sink) {
        fordFulkerson(source, sink);
        
        std::vector<bool> visited(V, false);
        std::queue<int> q;
        
        q.push(source);
        visited[source] = true;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (int v = 0; v < V; ++v) {
                if (!visited[v] && residualGraph[u][v] > 0) {
                    q.push(v);
                    visited[v] = true;
                }
            }
        }
        
        
        std::vector<std::vector<int> > segmentation;
        std::vector<int> foreground, background;
        
        for (int i = 0; i < V - 2; ++i) {
            if (visited[i]) {
                foreground.push_back(i);
            } else {
                background.push_back(i);
            }
        }
        
        segmentation.push_back(foreground);
        segmentation.push_back(background);
        
        return segmentation;
    }

    /**
     * @brief Retorna o vetor de vértices do grafo.
     * 
     * @return Referência constante ao vetor de vértices.
     */
    const std::vector<Pixel>& getVertices() const {
        return vertices;
    }
};

/**
 * @brief Calcula a diferença entre dois pixels com base na distância euclidiana das cores RGB.
 * 
 * @param p1 Primeiro pixel.
 * @param p2 Segundo pixel.
 * @return Diferença entre os pixels.
 */
double calculatePixelDifference(const Pixel& p1, const Pixel& p2) {
    return std::sqrt(
        std::pow(static_cast<int>(p1.r) - p2.r, 2) +
        std::pow(static_cast<int>(p1.g) - p2.g, 2) +
        std::pow(static_cast<int>(p1.b) - p2.b, 2)
    );
}

/**
 * @brief Classe para leitura de arquivos de imagem no formato PPM.
 */
class ImageReader {
public:
    /**
     * @brief Lê uma imagem no formato PPM e retorna os pixels e as dimensões.
     * 
     * @param filename Caminho do arquivo de imagem.
     * @return Par contendo os pixels da imagem e as dimensões (largura e altura).
     * @throws std::runtime_error Se o arquivo não puder ser aberto ou lido.
     */
    static std::pair<std::vector<Pixel>, std::pair<int, int> > readPPM(const std::string& filename) {
        std::ifstream file(filename.c_str(), std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        std::string magic;
        int width, height, maxVal;
        file >> magic >> width >> height >> maxVal;
        file.ignore(1); 

        std::vector<Pixel> pixels;
        pixels.reserve(width * height);

        for (int i = 0; i < width * height; ++i) {
            unsigned char r = 0, g = 0, b = 0;
            file.read(reinterpret_cast<char*>(&r), 1);
            file.read(reinterpret_cast<char*>(&g), 1);
            file.read(reinterpret_cast<char*>(&b), 1);
            pixels.push_back(Pixel(r, g, b));
        }

        return std::make_pair(pixels, std::make_pair(width, height));
    }
};
/**
 * @brief Classe para salvar imagens segmentadas no formato PPM.
 */
class ImageWriter {
public:
    /**
     * @function ImageWriter::saveSegmentationImage
     * @brief Salva a segmentação de uma imagem no formato PPM.
     * 
     * @param segmentation Segmentação resultante (conjuntos de índices de pixels).
     * @param vertices Vetor de pixels do grafo.
     * @param width Largura da imagem.
     * @param height Altura da imagem.
     * @param outputPath Caminho do arquivo de saída.
     * @throws std::runtime_error Se o arquivo de saída não puder ser criado.
     */
    static void saveSegmentationImage(
        const std::vector<std::vector<int> >& segmentation, 
        const std::vector<Pixel>& vertices,
        int width, int height,
        const std::string& outputPath
    ) {
        std::vector<Pixel> outputPixels(width * height, Pixel());
        
        std::vector<Pixel> componentColors;
        std::srand(static_cast<unsigned int>(std::time(NULL))); 
        
        for (size_t i = 0; i < segmentation.size(); ++i) {
            componentColors.push_back(Pixel(
                static_cast<unsigned char>(std::rand() % 256),
                static_cast<unsigned char>(std::rand() % 256),
                static_cast<unsigned char>(std::rand() % 256)
            ));
        }
        
        for (size_t colorIndex = 0; colorIndex < segmentation.size(); ++colorIndex) {
            for (size_t j = 0; j < segmentation[colorIndex].size(); ++j) {
                int pixel = segmentation[colorIndex][j];
                outputPixels[pixel] = componentColors[colorIndex];
            }
        }
        
        std::ofstream outputFile(outputPath.c_str(), std::ios::binary);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Error creating output file.");
        }
        
        outputFile << "P6\n" << width << " " << height << "\n255\n";
        
        for (size_t i = 0; i < outputPixels.size(); ++i) {
            outputFile.put(outputPixels[i].r);
            outputFile.put(outputPixels[i].g);
            outputFile.put(outputPixels[i].b);
        }
        
        outputFile.close();
    }
};
/**
 * @brief Classe para realizar segmentação de imagem utilizando um grafo de fluxo.
 * 
 * @param flowNetwork Grafo de fluxo associado à imagem.
 * @param width Largura da imagem.
 * @param height Altura da imagem.
 * @param pixels Vetor de pixels da imagem.
 */
class ImageSegmentation {
private:
    Graph flowNetwork;
    int width, height;
    std::vector<Pixel> pixels;

public:
    /**
     * @brief Construtor que inicializa o grafo de fluxo e armazena os pixels da imagem.
     * 
     * @param w Largura da imagem.
     * @param h Altura da imagem.
     * @param imgPixels Vetor de pixels da imagem.
     */
    ImageSegmentation(int w, int h, const std::vector<Pixel>& imgPixels) 
        : flowNetwork(w * h + 2), width(w), height(h), pixels(imgPixels) {
        
        
        for (int i = 0; i < w * h; ++i) {
            flowNetwork.addVertex(i, imgPixels[i]);
        }
    }
    /**
     * @brief Configura a rede de fluxo com base nos pixels da imagem e nos limiares de intensidade.
     * 
     * @param foregroundThreshold Limiar para determinar pixels do primeiro plano.
     * @param backgroundThreshold Limiar para determinar pixels do fundo.
     */
    void setupFlowNetwork(double foregroundThreshold, double backgroundThreshold) {
        int source = width * height;
        int sink = width * height + 1;

        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int pixelNode = y * width + x;
                Pixel pixel = pixels[pixelNode];
                
                
                double intensity = (pixel.r + pixel.g + pixel.b) / 3.0;

                
                if (intensity >= foregroundThreshold) {
                    flowNetwork.addEdge(source, pixelNode, 1000);
                } else if (intensity <= backgroundThreshold) {
                    flowNetwork.addEdge(pixelNode, sink, 1000);
                }

                
                if (x > 0) {
                    int leftPixel = y * width + (x - 1);
                    double weight = calculatePixelDifference(pixel, pixels[leftPixel]);
                    flowNetwork.addEdge(pixelNode, leftPixel, static_cast<int>(100 - weight));
                }

                if (y > 0) {
                    int topPixel = (y - 1) * width + x;
                    double weight = calculatePixelDifference(pixel, pixels[topPixel]);
                    flowNetwork.addEdge(pixelNode, topPixel, static_cast<int>(100 - weight));
                }
            }
        }
    }
    /**
     * @brief Realiza a segmentação da imagem utilizando corte mínimo.
     * 
     * @param foregroundThreshold Limiar para pixels do primeiro plano.
     * @param backgroundThreshold Limiar para pixels do fundo.
     * @return Segmentação resultante como dois conjuntos de pixels.
     */
    std::vector<std::vector<int> > segment(double foregroundThreshold, double backgroundThreshold) {
        setupFlowNetwork(foregroundThreshold, backgroundThreshold);
        
        int source = width * height;
        int sink = width * height + 1;
        
        return flowNetwork.minCutSegmentation(source, sink);
    }

    const Graph& getGraph() const {
        return flowNetwork;
    }
};

int main() {
    try {
        std::pair<std::vector<Pixel>, std::pair<int, int> > imageData 
            = ImageReader::readPPM("imagem.ppm");
        
        std::vector<Pixel> pixels = imageData.first;
        int width = imageData.second.first;
        int height = imageData.second.second;
        
        ImageSegmentation segmenter(width, height, pixels);

        std::vector<std::vector<int>> segmentation = segmenter.segment(180, 150);
        
        ImageWriter::saveSegmentationImage(
            segmentation, 
            segmenter.getGraph().getVertices(), 
            width, 
            height, 
            "./segments/output_segmented.ppm"
        );

        std::cout << "Segmentação concluída com sucesso!" << std::endl;
    } 
    catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}