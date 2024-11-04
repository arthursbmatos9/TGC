#include <iostream>
#include <list>
#include <vector>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

class Graph
{
    private:
        //o algoritmo trabalha com um vetor de pesos, uma lista de V visitados, o valor minMax,
        //uma matriz contendo o peso das arestas que saem da linha e chegam na coluna correspondetes aos vertices
        list<int> vertices;
        list<int> visitados;
        vector<float> pesos;
        vector<vector<float>> matriz;
        float valorMinMax;
    
    public:
        //iniciamos o algoritmo com todas os pesos sendo infinito e o valor minMax como 0
        Graph(list<int> v, vector<vector<float>> m):
            vertices(v),
            matriz(m),
            visitados(),
            pesos(v.size(), numeric_limits<float>::infinity()),
            valorMinMax(0)
        {}

        float minMaxValue(int u){
            //o peso de U inicia-se em menos infinito
            pesos[u] = -(numeric_limits<float>::infinity());
            int vMenorPeso;
            
            float temp;
            
            //realizamos o algoritmo enquanto todos os vertices ainda nao forem visitados
            while(vertices.size() != visitados.size()){
                vMenorPeso = menorPeso();  
                if (vMenorPeso == -1) break; 

                //marcamos tal V como visitado
                visitados.push_back(vMenorPeso);

                //dentre os vertices vizinhos do V de menor peso, fazemos:
                //comparamos o peso do V que estamos com o peso da aresta que liga ao vizinho e pegamos o maior
                //logo apos comparamos o valor obtido com o peso do V que estamos alcancando e pegamos o menor        
                for(int v : vertices){
                    if((matriz[vMenorPeso][v] != 0.0)){
                        temp = max(pesos.at(vMenorPeso), matriz[vMenorPeso][v]);
                        valorMinMax = min(temp, pesos.at(v));

                        //atualizamos o peso do V alcancado, que corresponde ao valor de minMax
                        pesos.at(v) = valorMinMax;
                    }
                }
            }

            return valorMinMax;
        }

        //retorna entre os vertices ainda nao visitados o que possui menor peso
        int menorPeso(){
            list<int> naoVisitados;

            //pega os vertices que ainda nao foram visitados
            for(int v : vertices){
                if(find(visitados.begin(), visitados.end(), v) == visitados.end())
                    naoVisitados.push_back(v);
            }
            
            //se nao tiver vertices vizinhos nao visitados, nao fazer nada
            if(naoVisitados.empty()) 
              return -1;

            int vMenorPeso = -1;
            float menorValor = numeric_limits<float>::infinity();

            //verifica os pesos dos vertices nao visitados e pega o menor vertice
            for(int v : naoVisitados){
                if (pesos.at(v) < menorValor) {
                    menorValor = pesos.at(v);
                    vMenorPeso = v;
                }
            }
            return vMenorPeso;
        }

        float getvalorMinMax(){
            return valorMinMax;
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

    int inicio;
    cout << "--Valor MinMax de um grafo--\nVertices disponiveis:\n\n";
    for(int v : vertices){
        cout << v << " | ";
    }
    cout << "\n\nQuer iniciar o algoritmo a partir de qual vertice? ";
    cin >> inicio;

    //chamando o algoritmo
    g->minMaxValue(inicio);
    
    cout << "O menor valor maximo de um caminho a partir do vertice " << inicio << " corresponde a: " << g->getvalorMinMax() << ".\n";
    system("pause"); 
}