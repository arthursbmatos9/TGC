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

class Fila {
public:
  Celula *primeiro;
  Celula *ultimo;

  // construtor da Lista encadeada
  Fila() {
    primeiro = new Celula();
    ultimo = primeiro;
  }

  // metodo para inserir (no final)
  void inserirFila(int valor) {
    Celula *temp = new Celula(valor);
    ultimo->prox = temp;
    ultimo = temp;

    cout << "Inseriu: " << valor << endl;
  }

  // OBS: TEM CELULA CABECA (primeiro)
  //  metodo para remover (do inicio)
  void removerFila() {
    if (primeiro == ultimo)
      throw std::logic_error("Nao ha elementos na fila!");

    Celula *removida = primeiro->prox;
    primeiro->prox = removida->prox;

    if (removida == ultimo)
      ultimo = primeiro;

    cout << "Removeu: " << removida->elemento << endl;
    delete removida; // deletando a celula efetivamente
  }

  // metodo para buscar um elemento na Lista
  bool buscarFila(int valor) {
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
  Fila *fila = new Fila();

  for (int n = 1; n < 16; n++) {
    fila->inserirFila(n);
  }

  fila->buscarFila(2);

  for (int k = 0; k < 5; k++) {
    fila->removerFila();
  }

  fila->buscarFila(2);
}