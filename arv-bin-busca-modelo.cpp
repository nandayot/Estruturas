#include <cstdio>
#include <string>

using std::string;

class No {
  friend class ArvBinBusca;
  
public:
  No(const int chave);

  void escreve(const char *sep = "");
  
private:
  int chave;
  No *pai;
  No *esq;
  No *dir;
};


class ArvBinBusca
{
public:
  ArvBinBusca();
  ArvBinBusca(const ArvBinBusca& outro); //JÁ // construtor de cópia
  ~ArvBinBusca();
  ArvBinBusca& operator=(const ArvBinBusca& outro); //JÁ // operador de atribuição
  
  void escreve_ordenado(); // escreve em percurso em-ordem
  void escreve();

  No *get_raiz(); // devolve a raiz
  No *busca(int k); // devolve o ponteiro para um elemento, se achar, ou NULL
  No *minimo(); // devolve o menor elemento da árvore
  No *maximo(); // devolve o maior elemento da árvore
  No *sucessor(No *x); //JÁ // devolve o sucessor de um elemento
  No *predecessor(No *x); //JÁ // devolve o predecessor de um elemento

  void insere(int chave); 
  bool remove(int chave); 

  void limpa(); // remove todos elementos da árvore
  
private:
  No *raiz;

  void escreve_ordenado(No *x); //JÁ // escreve em percurso em-ordem
  void escreve(const string& prefixo, No *x); 
  
  No *busca(No *x, int k);//JÁ
  No *minimo(No *x);//JÁ
  No *maximo(No *x);//JÁ
  
  void insere(No *z); //JÁ
  void transplante(No *u, No *v); //JÁ 
  void remove(No *z); //JÁ 
  
  void limpa(No *x); //JÁ // dado um nó x, remove recursivamente elementos abaixo e deleta x
  
  void copia(const ArvBinBusca& T); //JÁ  // copia uma árvore T para a atual a partir da raiz,
                                    // usada no construtor de cópia e no operador de atribuição
  void copia(No *dest, No *orig);   //JÁ   // copia um nó e os descendentes recursivamente
};


int main(void)
{
  ArvBinBusca T; // construtor ArvBinBusca()
  int v[] = {49,39,53,20,41,52,68,10,42,64,69,40};
  
  for (const auto &x : v){
    T.insere(x);
  }
  printf("T:\n");
  T.escreve();
  printf("Valores de T em ordem crescente: ");
  T.escreve_ordenado();

  //return 0; //TODO: re./nimo, maximo, sucessor, predecessor

  // No *raiz = T.get_raiz();
  // printf("Raiz: ");
  // raiz->escreve("\n");

  // No *pre = T.predecessor(raiz);
  // printf("Predecessor da raiz: ");
  // pre->escreve("\n");
  // No *suc = T.sucessor(raiz);
  // printf("Sucessor da raiz: ");
  // suc->escreve("\n");

  // printf("Sucessor do predecessor da raiz (== raiz): ");
  // T.sucessor(pre)->escreve("\n");
  // printf("Predecessor do sucessor da raiz (== raiz): ");
  // T.predecessor(suc)->escreve("\n");

  // No *minimo = T.minimo();
  // No *maximo = T.maximo();
  // printf("Mínimo: ");
  // minimo->escreve("\n");
  // printf("Máximo: ");
  // maximo->escreve("\n");

  //return 0; //TODO: remover após implementar remoção
  //printf("vai remover!\n");
  //T.remove(0); // Caso 1
  //printf("remove 0\n");
  //T.remove(13); // Caso 2
  //printf("remove 13\n");
  T.remove(49); // Caso 3b + 3a
  printf("remove 49\n");
  printf("T:\n");
  T.escreve();
  
  T.remove(53); // Caso 3b + 3a
  printf("remove 53\n");
  printf("T:\n");
  T.escreve();




  return 0;
  T.remove(52); // Caso 3b + 3a
  printf("remove 52\n");
  printf("T:\n");
  T.escreve();

  T.remove(20); // Caso 3b + 3a
  printf("remove 20\n");
  printf("T:\n");
  T.escreve();

  T.remove(68); // Caso 3b + 3a
  printf("remove 68\n");
  printf("T:\n");
  T.escreve();

  T.remove(53); // Caso 3b + 3a
  printf("remove 53\n");
  printf("T:\n");
  T.escreve();
  //return 0; //TODO: remover após implementar construtor de cópia e operador de atribuição
  
  // ArvBinBusca T2(T); // construtor de cópia
  // T2.insere(30);
  // printf("T:\n");
  // T.escreve();
  // printf("T2:\n");
  // T2.escreve();

  // //return 0;

  // ArvBinBusca T3 = T; // construtor de cópia
  // T3.insere(-8);
  // printf("T:\n");
  // T.escreve();
  // printf("T3:\n");
  // T3.escreve();

  // T3 = T; // operador de atribuição
  // T3.insere(100);
  // printf("T:\n");
  // T.escreve();
  // printf("T3:\n");
  // T3.escreve();
  
  return 0;
}


//***********************************
//*** IMPLEMENTAÇÕES DA CLASSE NO ***
//***********************************

No::No(const int chave) :
  chave(chave),
  pai(NULL),
  esq(NULL),
  dir(NULL) {
}

void No::escreve(const char *sep) {
  printf("%2d%s", chave, sep);
}


//********************************************
//*** IMPLEMENTAÇÕES DA CLASSE ARVBINBUSCA ***
//********************************************

ArvBinBusca::ArvBinBusca() {
  raiz = NULL;
}

ArvBinBusca::ArvBinBusca(const ArvBinBusca& outro) {
  copia(outro);
}

ArvBinBusca::~ArvBinBusca() {
  limpa();
}

ArvBinBusca& ArvBinBusca::operator=(const ArvBinBusca& outro) {
  limpa();
  copia(outro);
  return *this;
}

void ArvBinBusca::escreve_ordenado() {
  escreve_ordenado(raiz);
  putchar('\n');
}

void ArvBinBusca::escreve_ordenado(No *x) {
  //TODO: implementar (escrever em percurso em-ordem em uma única linha)
  if(x != NULL){
    escreve_ordenado(x->esq);
    printf("%d ", x->chave);
    escreve_ordenado(x->dir);
  }
}

void ArvBinBusca::escreve() {
  escreve("", raiz);
}

void ArvBinBusca::escreve(const string& prefixo, No *x) {
  if (x == NULL)
    return;

  bool ehDireito = x->pai && x->pai->dir == x;
  bool temIrmaoEsq = x->pai && x->pai->esq;
  
  printf(prefixo.c_str());
  printf(ehDireito && temIrmaoEsq ? "├──" : "└──" );

  if (x->pai == NULL) // raiz
    x->escreve("\n");
  else
    x->escreve(ehDireito ? "d\n" : "e\n");
  
  escreve(prefixo + (ehDireito && temIrmaoEsq ? "│   " : "    "), x->dir);
  escreve(prefixo + (ehDireito && temIrmaoEsq ? "│   " : "    "), x->esq);
}

No *ArvBinBusca::get_raiz() {
  return raiz;
}

No *ArvBinBusca::busca(int k) {
  return busca(raiz, k);
}

No *ArvBinBusca::busca(No *x, int k) {
  //TODO: implementar
  if(x == NULL || k == x->chave){
    return x;
  }
  if(k < x->chave){
    return busca(x->esq, k);
  }else{
    return busca(x->dir, k);
  }
}

No *ArvBinBusca::minimo() {
  return raiz ? minimo(raiz) : NULL;
}

No *ArvBinBusca::minimo(No *x) {
  //TODO: implementar
  while(x->esq != NULL){
    x = x->esq;
  }
  return x;
}

No *ArvBinBusca::maximo() {
  return raiz ? maximo(raiz) : NULL;
}

No *ArvBinBusca::maximo(No *x) {
  //TODO: implementar
  while(x->dir != NULL){
    x = x->dir;
  }
  return x;
}

No *ArvBinBusca::sucessor(No *x) {
  //TODO: implementar
  if(x->dir != NULL){
    return minimo(x->dir);
  }
  No* y = x->pai;
  while(y != NULL && x == y->dir){
    x = y;
    y = y->pai;
  }
  return y;
}

No *ArvBinBusca::predecessor(No *x) {
  //TODO: implementar
  if(x->esq != NULL){
    return maximo(x->esq);
  }
  No* y = x->pai;
  while(y != NULL && x == y->esq){
    x = y;
    y = y->pai;
  }
  return y;
}

void ArvBinBusca::insere(int chave) {
  No *z = new No(chave);
  insere(z);
}

void ArvBinBusca::insere(No *z) { /*************************************************/
  No* y = NULL;
  No* x;
  x = raiz;
  while(x != NULL){
    y = x;
    if(z->chave < x->chave){
      x = x->esq;
    }else{
      x = x->dir;
    }
  }
  z->pai = y;
  if(y == NULL){
    raiz = z;
  }else{
    if(z->chave < y->chave){
      y->esq = z;
    }else{
      y->dir = z;
    }
  }
}

void ArvBinBusca::transplante(No *u, No *v) {
  //TODO: implementar
  if(u->pai == NULL){
    raiz = v;
  }else{
    if(u == u->pai->esq){
      u->pai->esq = v;
    }else{
      u->pai->dir = v;
    }
  }
  if(v != NULL){
    v->pai = u->pai;
  }
}

bool ArvBinBusca::remove(int chave) {
  No *z = busca(raiz, chave);
  if (z == NULL)
    return false;

  remove(z);
  delete z;
  return true;
}

void ArvBinBusca::remove(No *z) {
  //TODO: implementar
  No* y;
  if(z->esq == NULL){
    printf("entrou caso 1\n");
    transplante(z, z->dir);
  }else{
    if(z->dir == NULL){
      printf("entrou caso 2\n");
      transplante(z, z->esq);
    }else{
      y = minimo(z->dir);
      printf("entrou 3 caso, y = minimo -> %d\n", y->chave);
      if(y->pai != z){
        printf("entrou caso b\n");
        transplante(y, y->dir);
        y->dir = z->dir;
        y->dir->pai = y;
      }
      transplante(z, y);
      y->esq = z->esq;
      y->esq->pai = y;
    }
  }
}

void ArvBinBusca::limpa() {
  limpa(raiz);
  raiz = NULL;
}

void ArvBinBusca::limpa(No *x) {
  //TODO: implementar
  if(x == NULL) return;
  limpa(x->esq);
  limpa(x->dir);
  //printf("deletou x : %d\n", x->chave);
  delete x;
}

void ArvBinBusca::copia(const ArvBinBusca& T) {
  if (T.raiz == NULL)
    raiz = NULL;
  else {
    raiz = new No(T.raiz->chave);
    copia(raiz, T.raiz);
  }
}

void ArvBinBusca::copia(No *dest, No *orig) {
  //TODO: implementar
  //No* y = NULL;
  //No* z = NULL;
  //printf("entrou copia\n");
  int chaveesq;
  if(orig->esq != NULL) chaveesq = orig->esq->chave;
  if(orig->esq != NULL){
    No* y = new No(chaveesq);
    dest->esq = y;
    y->pai = dest;
    if(dest->esq != NULL) dest->esq->pai = dest;
    copia(dest->esq, orig->esq);
  }

  int chavedir;
  if(orig->dir != NULL) chavedir = orig->dir->chave;
  if(orig->dir != NULL){
    No* z = new No(chavedir);
    dest->dir = z;
    z->pai = dest;
    if(dest->dir != NULL) dest->dir->pai = dest;
    copia(dest->dir, orig->dir);
  }
  
}
