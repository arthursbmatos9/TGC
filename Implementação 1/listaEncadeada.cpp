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

class Lista {
public:
  Celula *primeiro;
  Celula *ultimo;

  // construtor da Lista encadeada
  Lista() {
    primeiro = new Celula();
    ultimo = primeiro;
  }

  // metodo para inserir (no inicio)
  void inserirLista(int valor) {
    Celula *temp = new Celula(valor);
    temp->prox = primeiro->prox;
    primeiro->prox = temp;

    if (primeiro == ultimo)
      ultimo = temp;

    cout << "Inseriu: " << valor << endl;
  }

  // metodo para remover (do inicio)
  void removerLista() {
    if (primeiro == ultimo)
      throw std::logic_error("Nao ha elementos na lista!");

    Celula *removida = primeiro->prox;
    primeiro->prox = removida->prox;

    if (removida == ultimo)
      ultimo = primeiro;

    cout << "Removeu: " << removida->elemento << endl;
    delete removida; // deletando a celula efetivamente
  }

  // metodo para buscar um elemento na Lista
  bool buscarLista(int valor) {
    Celula *aux = primeiro;

    while (aux != ultimo) {
      if (aux->prox->elemento == valor) {
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
    Celula *aux = primeiro;

    while (aux != ultimo) {
      cout << aux->prox->elemento << ", ";
      aux = aux->prox;
    }
  }
};

int main() {
  Lista *listaEncadeada = new Lista();
  
  for (int n = 15; n > 0; n--) {
    listaEncadeada->inserirLista(n);
  }

  listaEncadeada->buscarLista(3);

  for (int k = 0; k < 5; k++) {
    listaEncadeada->removerLista();
  }

  listaEncadeada->buscarLista(3);
}