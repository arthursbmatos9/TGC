//@author: Arthur de Sá Braz de Matos
//@author: Vitória Símil Araújo

#include <iostream>
#include <list>
#include <vector>
#include <set>

using namespace std;

class Graph {
    private:
        //data
        list<int> *relations;
        int vertex;

    public:
        /**
         * @brief Constructs a Graph with a specified number of vertices.
         * @param vertex Number of vertices in the graph.
         */
        Graph(int vertex) : vertex(vertex), relations(new list<int>[vertex]){}

        /**
         * @brief Adds an edge between two vertices in the graph.
         * @param v1 First vertex.
         * @param v2 Second vertex.
         */
        void addEdge(int v1, int v2) {
            relations[v1].push_back(v2);
            relations[v2].push_back(v1);
        }
        
        /**
         * @brief Builds a complete graph by connecting every vertex to all others.
         */
        void build() {
            for ( int i = 0; i < vertex; i++ ) {
                for ( int j = 0; j < vertex; j++ ) {
                    if ( i != j )
                        relations[i].push_back(j);
                }
            }
        }

        /**
         * @brief Checks if a value is present in a vector.
         * @param subset The vector to search in.
         * @param element The value to search for.
         * @return true if the value is found, false otherwise.
         */
        bool contains(vector<int> subset, int element) {
            return find(subset.begin(), subset.end(), element) != subset.end();
        }

        /**
         * @brief Displays the subgraph formed by a subset of vertices.
         * @param subset A vector containing the vertices of the subgraph.
         */
        void show(vector<int> subset) {
            for (int i : subset) {
                cout << i << " -> ";
                for (int element : relations[i]) {
                    if (contains(subset, element)) {
                        cout << element << " ";
                    }
                }
                cout << endl;
            }
        }

        /**
         * @brief Displays the entire graph with all vertices and their connections.
         */
        void show() {
            cout << "Resulting Graph: \n";
            for ( int i = 0; i < vertex; i++ ) {
                cout << i << " -> ";
                for ( int element : relations[i] ) {
                    cout << element << " ";
                }
                cout << endl;
            }
            cout << endl;
        }

        /**
         * @brief Generates and displays all possible subgraphs of the complete graph.
         */
        void showSubgraphs() {
            //data
            int noEdgesSubgraphs = pow(2, vertex);
            int countSubgraphs = 0; 

            for (int i = 1; i < noEdgesSubgraphs; i++) { 
                vector<int> subset;

                //getting the subset within the set 
                for (int j = 0; j < vertex; j++) { 
                    if (i & (1 << j)) {
                        subset.push_back(j);
                    }
                }

                //getting all combination of edges for the subset 
                int subVertex = subset.size();
                int subEdges = subVertex * (subVertex - 1) / 2;
                int totalCombinations = 1 << subEdges;

                for (int i = 0; i < totalCombinations; ++i) {
                    cout << "Resulting Subgraph " << countSubgraphs + 1 << ":\n";
                    Graph subgraph(vertex);

                    int edgeIndex = 0;
                    for (int j = 0; j < subVertex; ++j) {
                        for (int k = j + 1; k < subVertex; ++k) {
                            if ((i & (1 << edgeIndex))) {
                                subgraph.addEdge(j, k);
                            }
                            ++edgeIndex;
                        }
                    }
                    subgraph.show(subset);
                    cout << endl;
                    countSubgraphs++;
                }
            }
            cout << "Total subgraphs for a " << vertex << "-vertex complete graph: " << countSubgraphs;
        }
};

int main(void) {
    //data
    int v = 0;
    do {
        cout << "Please digit the number of vertices: ";
        cin >> v;
        if ( v <= 0 ) cout << "Vertex number cannot be <= 0\n";
    } while ( v <= 0 );

    Graph g(v);
    g.build();
    g.show();
    g.showSubgraphs();
}