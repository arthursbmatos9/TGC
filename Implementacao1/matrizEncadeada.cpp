#include <iostream>
using namespace std;

class Celula {
public:
  int elemento;
  Celula *sup, *inf, *dir, *esq;

  Celula() { Celula(-1); };

  Celula(int valor) {
    elemento = valor;
    sup = nullptr;
    inf = nullptr;
    dir = nullptr;
    esq = nullptr;
  }
};

class Matriz {
public:
  Celula *inicio;
  int linha, coluna;

  Matriz(int lin, int col) {
    linha = lin;
    coluna = col;
    alocaCelulas();
  }

  // cria a estrutura da matriz, alocando as celulas
  void alocaCelulas() {
    inicio = new Celula();
    Celula *temp = inicio;
    for (int i = 1; i < coluna; i++) {
      temp->dir = new Celula();
      temp->dir->esq = temp;
      temp = temp->dir;
    }

    Celula *temp2 = inicio;

    for (int l = 1; l < linha; l++, temp2 = temp2->inf) {
      Celula *i = temp2;
      i->inf = new Celula();
      i->inf->sup = i;
      Celula *j = i->inf;

      for (int c = 1; c < coluna; c++, j = j->dir) {
        i = i->dir;
        i->inf = new Celula();
        i->inf->sup = i;
        i->inf->esq = j;
        j->dir = i->inf;
      }
    }
  }

  // insere na celula o elemento
  void inserirMatriz(int l, int c, int valor) {
    Celula *atual = inicio;
    int i, j;

    for (i = 0; i < c; i++) {
      atual = atual->dir;
    }
    for (j = 0; j < l; j++) {
      atual = atual->inf;
    }
    atual->elemento = valor;
  }

  // remove um elemento da matriz
  bool removerMatriz(int valor) {
    Celula *linhaAtual = inicio;
    for (int i = 0; i < 5; i++) {
      Celula *celula = linhaAtual;

      for (int j = 0; j < coluna; j++) {
        if (celula->elemento == valor) {
          cout << "Elemento " << valor << " removido da matriz!";
          celula->elemento = -1;
          return true;
        }
        celula = celula->dir;
      }
      cout << endl;
      linhaAtual = linhaAtual->inf;
    }
    cout << "Elemento " << valor << " nao encontrado na matriz!";
    return false;
  }

  // procura um elemento na matriz
  bool buscarMatriz(int valor) {
    Celula *linhaAtual = inicio;
    for (int i = 0; i < 5; i++) {
      Celula *celula = linhaAtual;

      for (int j = 0; j < coluna; j++) {
        if (celula->elemento == valor) {
          cout << "Elemento " << valor << " encontrado na matriz!\n";
          return true;
        }
        celula = celula->dir;
      }
      cout << endl;
      linhaAtual = linhaAtual->inf;
    }
    cout << "Elemento " << valor << " nao encontrado na matriz!\n";
    return false;
  }

  // imprime a matriz
  void mostrar() {
    Celula *linhaAtual = inicio;
    for (int i = 0; i < 5; i++) {
      Celula *celula = linhaAtual;

      for (int j = 0; j < coluna; j++) {
        cout << celula->elemento << " ";
        celula = celula->dir;
      }
      cout << endl;
      linhaAtual = linhaAtual->inf;
    }
  }
};

int main() {
  Matriz *matriz = new Matriz(5, 5);
  int valorAdd = 1;

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      matriz->inserirMatriz(i, j, valorAdd++);
    }
  }

  matriz->buscarMatriz(9);
  for (int k = 7; k < 11; k++) {
    matriz->removerMatriz(k);
  }
  matriz->buscarMatriz(9);
}