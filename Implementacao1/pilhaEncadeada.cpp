#include <iostream>
using namespace std;

class Celula {
public:
  int elemento;
  Celula *prox;

  // construtores (vazio e com valor definido)
  Celula() { Celula(-1); }

  Celula(int valor) {
    elemento = valor;
    prox = nullptr;
  }
};

class Pilha {
public:
  Celula *topo;

  // OBS: POSSUI CELULA CABECA (topo)
  //  construtor da Pilha
  Pilha() { topo = new Celula(); }

  // metodo para inserir (topo->prox)
  void inserirPilha(int valor) {
    Celula *temp = new Celula(valor);
    temp->prox = topo->prox;
    topo->prox = temp;

    cout << "Inseriu: " << valor << endl;
  }

  // metodo para remover (topo->prox)
  void removerPilha() {
    if (topo->prox == nullptr)
      throw std::logic_error("Não ha elementos na pilha!");

    Celula *removida = topo->prox;
    topo->prox = removida->prox;

    cout << "Removeu: " << removida->elemento << endl;
    delete removida; // deletando a celula efetivamente
  }

  // metodo para buscar um elemento na Pilha
  bool buscarPilha(int valor) {
    Celula *aux = topo->prox;

    while (aux != nullptr) {
      if (aux->elemento == valor) {
        cout << "Elemento " << valor << " encontrado!\n";
        return true;
      }
      aux = aux->prox;
    }

    cout << "Elemento " << valor << " não encontrado!\n";
    return false;
  }

  // metodo para mostrar os elementos da Lista
  void mostrar() {
    Celula *aux = topo;

    while (aux->prox != nullptr) {
      cout << aux->prox->elemento << ", ";
      aux = aux->prox;
    }
  }
};

int main() {
  Pilha *pilha = new Pilha();

  for (int n = 1; n < 16; n++) {
    pilha->inserirPilha(n);
  }

  pilha->buscarPilha(13);

  for (int k = 0; k < 5; k++) {
    pilha->removerPilha();
  }

  pilha->buscarPilha(13);
}