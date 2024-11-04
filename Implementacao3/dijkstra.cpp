#include <iostream>
#include <list>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

class Graph
{
    private:
        //o algoritmo trabalha com um vetor de distancias, uma lista de V visitados, 
        //uma matriz contendo o peso das arestas que saem da linha e chegam na coluna correspondetes aos vertices
        list<int> vertices;
        list<int> visitados;
        vector<float> distancias;
        vector<vector<float>> matriz;
    
    public:
        //iniciamos o algoritmo com todas as distancias sendo infinito
        Graph(list<int> v, vector<vector<float>> m):
            vertices(v),
            matriz(m),
            visitados(),
            distancias(v.size(), numeric_limits<float>::infinity())
        {}

        vector<float> shortestPath(int u){
            //a distancia de U para si mesmo é 0
            distancias[u] = 0;
            int vMenorDistancia;
            
            //realizamos o algoritmo enquanto todos os vertices ainda nao forem visitados
            while(vertices.size() != visitados.size()){
                //pegamos aquele V que possui menor distancia acumulada
                vMenorDistancia = menorDistancia();
                if(vMenorDistancia == -1) 
                    break;

                //marcamos tal V como visitado
                visitados.push_back(vMenorDistancia);

                //atualizamos a distancia entre o V de menor caminho e seus vizinhos, sendo:
                //o menor valor entre a atual distancia ou o valor da atual distancia + o peso da aresta que os liga
                for (int v = 0; v < vertices.size(); ++v) {
                    if (matriz[vMenorDistancia][v] != 0.0) {
                        float novaDistancia = distancias[vMenorDistancia] + matriz[vMenorDistancia][v];
                        if (distancias[v] > novaDistancia) {
                            distancias[v] = novaDistancia;
                        }
                    }
                }
            }

            return distancias;
        }

        //retorna entre os vertices ainda nao visitados o que possui menor distancia acumulada
        int menorDistancia(){
            list<int> naoVisitados;

            //pega os vertices que ainda nao foram visitados
            for(int v : vertices){
                if(find(visitados.begin(), visitados.end(), v) == visitados.end())
                    naoVisitados.push_back(v);
            }
            
            //se nao tiver vertices vizinhos nao visitados, nao fazer nada
            if(naoVisitados.empty()) 
              return -1;

            int vMenorDistancia = -1;
            float menorValor = numeric_limits<float>::infinity();

            //verifica as distancias dos vertices nao visitados e pega o menor vertice
            for(int v : naoVisitados){
                if (distancias.at(v) < menorValor) {
                    menorValor = distancias.at(v);
                    vMenorDistancia = v;
                }
            }
            return vMenorDistancia;
        }

};


int main(void){
    list<int> vertices;
    
    ifstream arq("graph1.graph");
    string linhaLida;
    
    //lendo os vertices do grafo
    while(getline(arq, linhaLida)){
        if(linhaLida == "end") break;
        vertices.push_back(stoi(linhaLida));
    }

    //criando a matriz de pesos e setando os pesos como 0.0 (padrao)
    vector<vector<float>> arestas(vertices.size(), vector<float>(vertices.size()));
    for(int i = 0; i < arestas.size(); i++){
        for(int j = 0; j< arestas.size(); j++){
            arestas[i][j] = 0.0;
        }
    }

    //lendo os pesos do arquivo e salvando na matriz nas posicoes adequadas  
    while(getline(arq, linhaLida)){
        if(linhaLida == "end") break;
        auto parte1 = linhaLida.find("-");
        auto parte2 = linhaLida.find("-", parte1 + 1);

        int vOrigem = stoi(linhaLida.substr(0, parte1));
        int vDestino = stoi(linhaLida.substr(parte1 + 1, parte2 - parte1 - 1));
        float peso = stof(linhaLida.substr(parte2 + 1));

        arestas[vOrigem][vDestino] = peso;
    }

    Graph *g = new Graph(vertices, arestas);

    int inicio, fim;
    cout << "A partir de que vertice voce quer encontrar o menor caminho? Opcoes: \n| ";
    for(int v : vertices){
        cout << v << " | ";
    }
    cout << "\n";
    cin >> inicio;
    cout << "\nE quer encontrar o menor caminho ate qual vertice?\n";
    cin >> fim;

    //chamando metodo Dijkstra
    vector<float> dist = g->shortestPath(inicio);
    cout << "A menor distancia entre " << inicio << " e " << fim << " corresponde a " 
    << (dist.at(fim) == numeric_limits<float>::infinity() ? "ERRO: nao existe caminho entre tais vertices" : to_string(dist.at(fim))) << endl;

    cout << "\n-----------------------------------------------------------\n";
    cout << "O array de distancias completo ficou:\n";
    for(float f : dist){
        cout << f << " - ";
    }
    cout << "\n";
    system("pause"); 
}