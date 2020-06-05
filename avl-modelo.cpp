#include <cstdio>
#include <string>

using std::string;

class No {
  friend class AVL;
  
public:
  No(const int chave);

  void escreve(const char *sep = "");
  inline bool eh_raiz();         // devolve true se o nó é raiz
  inline bool eh_direito();      // devolve true se o nó é filho direito do seu pai
  inline bool eh_esquerdo();     // devolve true se o nó é filho esquerdo do seu pai
  inline int bal();              // calcula e devolve o fator de balanceamento
  inline void atualiza_altura(); // recalcula a altura a partir das alturas dos filhos

private:
  int chave;
  int altura; // consideramos que a altura das folhas é 0
  No *pai;
  No *esq;
  No *dir;
};


class AVL
{
public:
  AVL();
  AVL(const AVL& outro); // construtor de cópia
  ~AVL();
  AVL& operator=(const AVL& outro); // operador de atribuição
  
  void escreve_ordenado(); // escreve em percurso em-ordem
  void escreve();

  No *get_raiz();         // devolve a raiz
  No *busca(int k);       // devolve o ponteiro para um elemento, se achar, ou NULL
  No *minimo();           // devolve o menor elemento da árvore
  No *maximo();           // devolve o maior elemento da árvore
  No *sucessor(No *x);    // devolve o sucessor de um elemento
  No *predecessor(No *x); // devolve o predecessor de um elemento

  void insere(int chave); // insere uma chave
  bool remove(int chave); // remove uma árvore

  void limpa(); // remove todos elementos da árvore
  
private:
  No *raiz;

  void escreve_ordenado(No *x); // escreve em percurso em-ordem
  void escreve(const string& prefixo, No *x);
  
  No *busca(No *x, int k); // chamada recursiva inicial para a raiz
  No *minimo(No *x);       // devolve o mínimo a partir de um nó
  No *maximo(No *x);       // devolve o máximo a partir de um nó
  
  // recalcula balanceamento e rotaciona, se necessário, devolvendo o
  // ponteiro para o nó que ocupou a posição de p na árvore (caso uma
  // rotação tenha sido realizada)
  No *ajusta_balanceamento(No *p);

  void transplante(No *u, No *v);  // transplante de v para u, não altera filhos
  void insere(No *z);              // insere um nó z na árvore
  void remove(No *z);              // remove um nó z da árvore
  
  void limpa(No *x); // dado um nó x, remove recursivamente todos elementos abaixo e deleta x
  
  void copia(const AVL& T); // copia uma árvore T para a atual a partir da raiz,
                            // usada no construtor de cópia e no operador de atribuição
  void copia(No *dest, No *orig); // copia um nó e os descendentes recursivamente
  
 // TODO: remover esta linha após testar rotações na main
  void rotacao_dir(No *p); // Rotação à direita: p e p->esq
  void rotacao_esq(No *p); // Rotação à esquerda: p e p->dir
  void rotacao_dupla_dir(No *p); // Rotação dupla à direita: p->esq e p->esq->dir à esquerda, então p e p->esq à direita
  void rotacao_dupla_esq(No *p); // Rotação dupla à esquerda: p->dir e p->dir->esq à direita, então p e p->dir à esquerda
};


int main(void)
{
  AVL T; // construtor AVL()
  int v[] = {10, 25, 0, 16, 20, 9, 15, 6, 14, 7, 18, 12, 22, 19, 3, 13, 28};

  printf("##TESTE DE INSERÇÃO##\n\n");
  printf("T:\n");
  for (const auto &x : v){
    T.insere(x); // Inicialmente, sem balanceamento
  }
  printf("\n");
  T.escreve();

  //return 0; // TODO: implemente as rotações esquerda e direita e remova esta linha

  /*printf("\n##TESTE DE ROTAÇÕES##\n\n");
  No *raiz = T.get_raiz();
  T.rotacao_esq(raiz);
  T.escreve();
  raiz = T.get_raiz(); // a raiz agora é outra, precisamos obtê-la novamente
  T.rotacao_dir(raiz);
  T.escreve();*/
  
  // TODO: após testar rotações, remova a linha "return" indicada na
  // função "void AVL::insere(No *z)", torne as funções de rotação
  // *privadas*, remova as linhas de rotação acima e teste novamente
  // a main para verificar se o balanceamento está correto
  

  // TODO: implemente a remoção e remova esta linha

  printf("\n##TESTE DE REMOÇÃO##\n\n");
  printf("T:\n");
  T.escreve();
  for (const auto &x : v) {
    printf("Removendo %d...\n", x);
    T.remove(x);
    printf("T:\n");
    T.escreve();
  }
  
  return 0;
}


//***********************************
//*** IMPLEMENTAÇÕES DA CLASSE NO ***
//***********************************

No::No(const int chave) :
  chave(chave),
  altura(0),
  pai(NULL),
  esq(NULL),
  dir(NULL) {
}

void No::escreve(const char *sep) {
  printf("%2d(%d)%s", chave, bal(), sep);
}

bool No::eh_raiz() {
  return pai == NULL;
}

bool No::eh_direito() {
  return !eh_raiz() && pai->dir == this;
}

bool No::eh_esquerdo() {
  return !eh_raiz() && pai->esq == this;
}

int No::bal() {
  // TODO: implementar
  /*if(esq == NULL && dir == NULL) return 0;
  if(esq == NULL && dir != NULL) return -1;
  if(esq != NULL && dir == NULL) return 1;
  int bal = esq->altura - dir->altura;*/

  int count=0;
  if(esq!=NULL){
    count+=(esq->altura+1);
  }
  if(dir!=NULL){
    count-=(dir->altura+1);
  }
  return count;
}

void No::atualiza_altura() {
  // TODO: implementar
  //int hesq, hdir;
  if(esq == NULL && dir==NULL){
    altura = 0;
    
  }
  else if(esq == NULL) altura = dir->altura+1;
  else if(dir==NULL) altura = esq->altura+1;

  else{
    if(esq->altura > dir->altura) altura = esq->altura+1;
    else altura = dir->altura+1;
    //altura = max(esq->altura, dir->altura)+1;
  }
}


//************************************
//*** IMPLEMENTAÇÕES DA CLASSE AVL ***
//************************************

AVL::AVL() {
  raiz = NULL;
}

AVL::AVL(const AVL& outro) {
  copia(outro);
}

AVL::~AVL() {
  limpa();
}

AVL& AVL::operator=(const AVL& outro) {
  limpa();
  copia(outro);
  return *this;
}

void AVL::escreve_ordenado() {
  escreve_ordenado(raiz);
  putchar('\n');
}

void AVL::escreve_ordenado(No *x) {
  // percurso em-ordem
  if (x == NULL)
    return;
  
  escreve_ordenado(x->esq);
  x->escreve(" ");
  escreve_ordenado(x->dir);
}

void AVL::escreve() {
  escreve("", raiz);
}

void AVL::escreve(const string& prefixo, No *x) {
  if (x == NULL)
    return;

  bool ehDireito = x->pai && x->pai->dir == x;
  bool temIrmaoEsq = x->pai && x->pai->esq;
  
  printf("%s", prefixo.c_str());
  printf(ehDireito && temIrmaoEsq ? "├──" : "└──" );

  if (x->pai == NULL) // raiz
    x->escreve("\n");
  else
    x->escreve(ehDireito ? "d\n" : "e\n");
  
  escreve(prefixo + (ehDireito && temIrmaoEsq ? "│   " : "    "), x->dir);
  escreve(prefixo + (ehDireito && temIrmaoEsq ? "│   " : "    "), x->esq);
}

No *AVL::get_raiz() {
  return raiz;
}

No *AVL::busca(int k) {
  return busca(raiz, k);
}

No *AVL::busca(No *x, int k) {
  if (x == NULL || k == x->chave)
    return x;

  if (k < x->chave)
    return busca(x->esq, k);
  else
    return busca(x->dir, k);
}

No *AVL::minimo() {
  return raiz ? minimo(raiz) : NULL;
}

No *AVL::minimo(No *x) {
  while (x->esq != NULL)
    x = x->esq;
  return x;
}

No *AVL::maximo() {
  return raiz ? maximo(raiz) : NULL;
}

No *AVL::maximo(No *x) {
  while (x->dir != NULL)
    x = x->dir;
  return x;
}

No *AVL::sucessor(No *x) {
  if (x->dir != NULL)
    return minimo(x->dir);

  No *y = x->pai;
  while (y != NULL && x == y->dir) {
    x = y;
    y = y->pai;
  }
  return y;
}

No *AVL::predecessor(No *x) {
  if (x->esq != NULL)
    return maximo(x->esq);

  No *y = x->pai;
  while (y != NULL && x == y->esq) {
    x = y;
    y = y->pai;
  }
  return y;
}

No *AVL::ajusta_balanceamento(No *p) {
  
  p->atualiza_altura();
  //printf("chave = %d bal= %d\n", p->chave, p->bal());

  if (p->bal() == 2) {       // Caso 1
    if (p->esq->bal() >= 0){  // Caso 1.1
      //printf("entrou caso 1.1\n");
      rotacao_dir(p);
    }
    else {                   // Caso 1.2
      //printf("entrou caso 1.2\n");
      rotacao_dupla_dir(p);
    }
    p = p->pai;              // após a rotação p desceu 1 nível, precisamos subir de volta
  }
  else if (p->bal() == -2) { // Caso 2
    if (p->dir->bal() <= 0){  // Caso 2.1
      //printf("entrou caso 2.1\n");
      rotacao_esq(p);
    }
    else {                    // Caso 2.2
      //printf("entrou caso 2.2\n");
      rotacao_dupla_esq(p);
    }
    p = p->pai;              // após a rotação p desceu 1 nível, precisamos subir de volta
  }
  
  return p;
}

void AVL::insere(int chave) {
  No *z = new No(chave);
  insere(z);
}

void AVL::insere(No *z) {
  //printf("nó inserido: %d\n", z->chave);
  No *y = NULL;
  No *x = raiz;

  while (x != NULL) {
    y = x;
    x = z->chave < x->chave ? x->esq : x->dir;
  }
  z->pai = y;
  if (y == NULL)
    raiz = z;
  else
    if (z->chave < y->chave)
      y->esq = z;
    else
      y->dir = z;


  // TODO: remova esta linha após testar as rotações na main,
          // o código abaixo só vai funcionar corretamente após a
          // implementação das rotações

  // Atualização das alturas/fatores de balanceamento
  if (z->eh_raiz())
    return;
  
  do { // o laço inicia subindo imediatamente para o pai do nó inserido
    z = z->pai;
    //printf("olhando para: %d\n", z->chave);
    z = ajusta_balanceamento(z);
    //printf("altura do pai do nó inserido: %d chave %d bal: %d\n", z->altura, z->chave, z->bal());
    //escreve();
  } while (!z->eh_raiz() && z->bal() != 0);
}

void AVL::transplante(No *u, No *v) {
  if (u->eh_raiz())
    raiz = v;
  else
    if (u->eh_esquerdo())
      u->pai->esq = v;
    else
      u->pai->dir = v;

  if (v != NULL)
    v->pai = u->pai;
}

bool AVL::remove(int chave) {
  No *z = busca(raiz, chave);
  if (z == NULL)
    return false;

  remove(z);
  delete z;
  return true;
}

void AVL::remove(No *z) {
  // TODO: remoção da árvore binária de busca, falta implementar toda a parte de balanceamento
  //printf("entrou remocao z=%d\n", z->chave);
  No *p = NULL;
  
  if (z->esq == NULL) { // 1o caso
    //p = ???; // p pode receber nulo?
    //printf("entrou caso 1\n");
    p = z->pai;  //p pode ser NULL só se for raiz
    //p->atualiza_altura();
    transplante(z, z->dir);
  }
  else {
    if (z->dir == NULL) { // 2o caso
      //printf("entrou caso 2\n");
      p = z->pai;
      transplante(z, z->esq);
      if(z->esq != NULL) p = z->esq;
      //p = ajusta_balanceamento(p);
    }
    else { // 3o caso  COM DOIS FILHOS
      //printf("entrou caso 3\n");
      No *y = minimo(z->dir);
      //p = y->pai;
      if (y->pai != z) { // (b)
        //p = ???; // p pode receber nulo?
        p = y->pai;
        //printf("entrou caso 3 B y = %d\n", y->chave);
        //p = ajusta_balanceamento(p);
        transplante(y, y->dir); 
        y->dir = z->dir;
        y->dir->pai = y;
        y->altura = z->altura;
      }
      //printf("entrou caso 3 A y->pai = %d\n", p->pai->chave);
      
      transplante(z, y); // (a)
      
      
      //p->atualiza_altura();
      y->esq = z->esq;
      y->esq->pai = y;
      

      if (p == NULL) // se fomos direto pro caso 3(a) sem entrar no caso 3(b)
        p = y;//p = ???; // p pode receber nulo?
        //p->atualiza_altura();
    }


  }
  if(p){
    //printf("p = %d\n", p->chave);
    p = ajusta_balanceamento(p);
  }else{
    //printf("p null\n");
    //escreve();
    return;
  }
  
  while(!p->eh_raiz() && (p->bal() != -1 && p->bal() != 1)){
    p = p->pai;
    //printf("olhando para: %d\n", z->chave);
    p = ajusta_balanceamento(p);
  }
}

void AVL::limpa() {
  limpa(raiz);
  raiz = NULL;
}

void AVL::limpa(No *x) {
  if (x == NULL)
    return;

  limpa(x->esq);
  limpa(x->dir);
  delete x;
}

void AVL::copia(const AVL& T) {
  if (T.raiz == NULL)
    raiz = NULL;
  else {
    raiz = new No(T.raiz->chave);
    copia(raiz, T.raiz);
  }
}

void AVL::copia(No *dest, No *orig) {
  if (orig->esq) {
    dest->esq = new No(orig->esq->chave);
    dest->esq->pai = dest;
    copia(dest->esq, orig->esq);
  }
  
  if (orig->dir) {
    dest->dir = new No(orig->dir->chave);
    dest->dir->pai = dest;
    copia(dest->dir, orig->dir);
  }
}

void AVL::rotacao_dir(No *p) { //LL
  
  No *u = p->esq;
  p->esq = u->dir;
  //No *Tx = u->dir;
  
  // TODO: implementar
  if(u->dir != NULL) u->dir->pai = p;
  u->pai = p->pai;
  if(p->pai == NULL) raiz = u;
  else if(p == p->pai->dir) p->pai->dir = u;
  else p->pai->esq = u;

  u->dir = p;
  p->pai = u;

  //u->dir = p;
  //p->esq = Tx;
  
  // no final, atualiza alturas
  p->atualiza_altura();
  u->atualiza_altura();
}

void AVL::rotacao_esq(No *p) {
  
  No *u = p->dir;
  p->dir = u->esq;
  //No *Ty = u->esq;
  // TODO: implementar
  
  if(u->esq != NULL) u->esq->pai = p;
  u->pai = p->pai;
  if(p->pai == NULL) raiz = u;
  else if(p == p->pai->esq) p->pai->esq = u;
  else p->pai->dir = u;

  u->esq = p;
  p->pai = u;


  //u->esq = p;
  //p->dir = Ty;
  // no final, atualiza alturas
  p->atualiza_altura();
  u->atualiza_altura();
}

void AVL::rotacao_dupla_dir(No *p) {
  rotacao_esq(p->esq);
  rotacao_dir(p);
}

void AVL::rotacao_dupla_esq(No *p) {
  rotacao_dir(p->dir);
  rotacao_esq(p);
}
