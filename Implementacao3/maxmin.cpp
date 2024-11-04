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
        //o algoritmo trabalha com um vetor de pesos, uma lista de V visitados, o valor maxMin,
        //uma matriz contendo o peso das arestas que saem da linha e chegam na coluna correspondetes aos vertices
        list<int> vertices;
        list<int> visitados;
        vector<float> pesos;
        vector<int> arestasPercorridas;
        vector<vector<float>> matriz;
        float valorMaxMin;
    
    public:
        //iniciamos o algoritmo com todas os pesos sendo menos infinito e o valor MaxMin como 0
        Graph(list<int> v, vector<vector<float>> m):
            vertices(v),
            visitados(),
            pesos(v.size(), -(numeric_limits<float>::infinity())),
            arestasPercorridas(v.size(), 0),
            matriz(m),            
            valorMaxMin(0)    
        {
        }

        //preenche o vetor arestasPercorridas, que comeca com o grau de cada vertice
        void preencheArestasPercorridas(){
            int qtd;
            for(int x = 0; x < matriz.size(); x++){
                qtd = 0;
                for(int y= 0; y< matriz.size(); y++){
                    if(matriz[x][y] != 0.0)
                        qtd++;
                }
                arestasPercorridas.at(x) = qtd;
            }
        }

        float maxMinValue(int u){
            //o peso de U inicia-se em infinito
            pesos[u] = numeric_limits<float>::infinity();
        
            for(int f = 0; f< matriz.size(); f++){
                if(matriz[u][f] == 0.0)
                    visitados.push_back(f);
            }
            
            int vMaiorPeso;    
            float temp;
            
            //realizamos o algoritmo enquanto todos os vertices ainda nao forem visitados
            while(vertices.size() != visitados.size()){
                //pegamos aquele vertice que tem maior peso para podermos fazer o caminhamento. caso nao haja mais vertices, acabar
                vMaiorPeso = maiorPeso();
                if (vMaiorPeso == -1) break; 

                //marcamos uma aresta do vertice como visitada (tiramos 1 do grau)
                arestasPercorridas.at(vMaiorPeso) = arestasPercorridas.at(vMaiorPeso) - 1;

                //marcamos tal V como visitado caso nao haja mais arestas adjacentes
                if(arestasPercorridas.at(vMaiorPeso) == 0)
                    visitados.push_back(vMaiorPeso);

                //dentre os vertices vizinhos do V de menor peso, fazemos:
                //comparamos o peso do V que estamos com o peso da aresta que liga ao vizinho e pegamos o menor
                //logo apos comparamos o valor obtido com o peso do V que estamos alcancando e pegamos o maior        
                for(int v : vertices){
                    if((matriz[vMaiorPeso][v] != 0.0)){
                        temp = min(pesos.at(vMaiorPeso), matriz[vMaiorPeso][v]);
                        valorMaxMin= max(temp, pesos.at(v));
                        
                        //atualizamos o peso do V alcancado, que corresponde ao valor de maxMin
                        pesos.at(v) = valorMaxMin;
                    }
                }
            }
            return valorMaxMin;
        }

        //retorna entre os vertices ainda nao visitados o que possui menor peso
        int maiorPeso(){
            list<int> naoVisitados;

            //pega os vertices que ainda nao foram visitados
            for(int v : vertices){
                if(arestasPercorridas.at(v) > 0)
                    naoVisitados.push_back(v);
            }
            
            //se nao tiver vertices vizinhos nao visitados, nao fazer nada
            if(naoVisitados.empty()) 
              return -1;

            int vMaiorPeso = -1;
            float maiorValor = -(numeric_limits<float>::infinity());

            //verifica os pesos dos vertices nao visitados e pega o menor vertice
            for(int v : naoVisitados){
                if (pesos[v] >= maiorValor) {
                    maiorValor = pesos[v];
                    vMaiorPeso = v;
                }
            }
            return vMaiorPeso;
        }

        

        float getvalorMaxMin(){
            return valorMaxMin;
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
        for(int j = 0; j < arestas.size(); j++){
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
    
    cout << "--Valor MaxMin de um grafo--\nVertices disponiveis:\n\n";
    for(int v : vertices){
        cout << v << " | ";
    }
    cout << "\n\nQuer iniciar o algoritmo a partir de qual vertice? ";
    cin >> inicio;

    g->preencheArestasPercorridas();

    //chamando o algoritmo
    g->maxMinValue(inicio);

    //reforcando que o valor maxMin seja o menor finito, caso exista
  
    
    cout << "O menor valor maximo de um caminho a partir do vertice " << inicio << " corresponde a: " << g->getvalorMaxMin() << ".\n";
    system("pause");
    
}