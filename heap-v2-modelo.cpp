#include <cstdio>
#include <climits>
#include <string>
#include <algorithm>

using std::string;

class Heap
{
public:
  Heap();
  Heap(const int n, const int dados[]); ///// JÁ
  Heap(const Heap& outro); //HEAP NOVO CONSTRUTOR DE COPIA  outro.capacidade... outro.n... é tipo obj
  ~Heap();
  Heap& operator=(const Heap& outro){
    delete [] S;
    S = NULL;
    capacidade = outro.capacidade;
    n = outro.n;
    S = new int[n];
    for(int i = 0; i < n; i++){
      S[i] = outro.S[i];
    }
    return *this;
  } 
  //SUBSTITUIÇÃO DE HEAP h1 = h2  primeiro faz o free() depois copia
  void escreve_niveis(); ///// JÁ
  void escreve(const string& prefixo = "", int i = 0); ///// JÁ
  void insere(int p); ///// JÁ
  int consulta_maxima(); ///// JÁ
  int extrai_maxima(); ///// JÁ
  void altera_prioridade(int i, int p); ///// JÁ
  
private:
  int *S;
  int n; // n = 0; quando insere aumente o n
  int capacidade; // inicialmente , capacidade = TAMANHO_INICIAL 
  static const int TAMANHO_INICIAL = 4;
  
  int pai(int i); ///// JÁ
  int esquerdo(int i); ///// JÁ
  int direito(int i); ///// JÁ
  void troca(int i, int j); ///// JÁ
  void desce(int i); ///// JÁ
  void sobe(int i);  ///// JÁ
}; 



// valgrind --leak-check=full --show-reachable=yes --track-fds=yes ./prog
int main(void)
{
  //int v[] = {12,20,31};
  
  //Heap h2(3, v);
  //h2.escreve();
  Heap h;
  h.insere(2);
  h.insere(1);
  h.insere(7);
  h.insere(4);
  h.insere(12);
  h.escreve();

  //printf("Max: %d\n", h.consulta_maxima());
  //printf("Maior extraido: %d\n", h.extrai_maxima());
  //h.escreve();
  //h.altera_prioridade(4,15);
  //h.escreve();
  //printf("Extraí o maior: %d\n", h.extrai_maxima());
  //h.escreve();

  //h.altera_prioridade(1, 15);
  //h.escreve();

  //printf("---- Escreve Niveis ---- \n");
  //h.escreve_niveis();

  //Heap construtor copia
  //Heap h2(h);
  //OPERATOR
  //Heap h2;
  //h2 = h;
  //printf("h2:\n");
  //h2.escreve();
  return 0;
}


Heap::Heap() {
  capacidade = TAMANHO_INICIAL;
  n=0;
  S = new int[capacidade];
}

Heap::Heap(const int num, const int dados[]){
  //S(dados, dados + n) {
  //TODO: implementar (constroi_max_heap)
  int *temp = (int*)dados;
  n = num;
  S = new int [n];
  capacidade = TAMANHO_INICIAL;  
  int i;

  for(i = 0; i < n; i++){
    S[i] = temp[i];
  }
  /// ADICIONAR NO VETOR DEPOIS DEIXAR EM HEAP 
  
  for(i = n/2 - 1; i >= 0; i--){
    desce(i);    
  }

}

Heap::Heap(const Heap& outro){
  capacidade = outro.capacidade;
  n = outro.n;
  S = new int[n];
  for(int i = 0; i < n; i++){
    S[i] = outro.S[i];
  }
}

Heap::~Heap() {
  delete [] S;
}




void Heap::escreve(const string& prefixo, int i) {
  if (i < (int) n) {
    bool ehEsquerdo = i % 2 != 0;
    bool temIrmao = i < (int) capacidade-1;

    printf(prefixo.c_str());
    printf(ehEsquerdo && temIrmao ? "├──" : "└──" );

    printf("%d\n", S[i]);
      
    escreve(prefixo + (ehEsquerdo ? "│   " : "    "), esquerdo(i));
    escreve(prefixo + (ehEsquerdo ? "│   " : "    "), direito(i));
  }
}

int Heap::pai(int i) {
  return (i - 1) / 2;
}

int Heap::esquerdo(int i) {
  return 2 * (i + 1) - 1;
}

int Heap::direito(int i) {
  return 2 * (i + 1);
}

void Heap::troca(int i, int j) {
  int aux = S[i];
  S[i] = S[j];
  S[j] = aux;
}

void Heap::desce(int i) {
  int e, d, maior;
  //int n = capacidade;
  e = esquerdo(i);
  d = direito(i);
  if (e < n && S[e] > S[i])
    maior = e;
  else
    maior = i;
  if (d < n && S[d] > S[maior])
    maior = d;
  if( maior != i){
    troca(i, maior);
    desce(maior);
  }
  //TODO: implementar

}

void Heap::sobe(int i) {
  while (S[pai(i)] < S[i]) {
    troca(i, pai(i));
    i = pai(i);
  }
}

void Heap::insere(int p) {
  if(n == capacidade){
    printf("Atingiu capacidade. Alocando mais espaço...\n");
    int *vetoraux = new int[2*capacidade];
    capacidade *= 2;

    for(int i=0; i<n;i++){
      vetoraux[i] = S[i];
    }
    delete [] S;
    S = vetoraux;
  }
  S[n] = p;
  //printf("S[n] = p; %d %d\n", n, p);
  n++;
  
  /*for(int i=0; i<n;i++){
    printf("%d ", S[i]);
  }*/
  //printf("\n");
  sobe(n-1);

  
}

int Heap::consulta_maxima() {
  return S[0];
}

int Heap::extrai_maxima() {
  //TODO: implementar
  troca(0, n-1);
  int maior = S[n-1];
  n--;
  desce(0);
  return maior;
}

void Heap::altera_prioridade(int i, int p) {
  int papis = pai(i);
  int e = esquerdo(i);
  int d = direito(i);
  int maior;
  if(n == capacidade){
    printf("Atingiu capacidade (altera). Alocando mais espaço...\n");
    int *vetoraux = new int[2*capacidade];
    capacidade *= 2;

    for(int i=0; i<n;i++){
      vetoraux[i] = S[i];
    }
    delete [] S;
    S = vetoraux;
  }
  n++;
  S[n] = p;
  troca(i, n);
  printf("S[n] = p; %d %d\n", n, p);
  n--;
  
  if (e < n-1 && S[e] > S[i])
    maior = e;
  else
    maior = i;
  if (d < n-1 && S[d] > S[maior])
    maior = d;
  
  

  if(p > S[papis]){
    printf("entrou no sobe i: %d\n", i);
    sobe(i);
  }
  if(p < S[maior]){
    printf("entrou no desce\n");
    desce(i);
  }
  //TODO: implementar

}

void Heap::escreve_niveis() {
  int escritos = 0, fim_nivel = 1;

  for (int i = 0; i < n ;i++) { 
    printf("%d ", S[i]);
    if (++escritos == fim_nivel) {
      putchar('\n');
      fim_nivel *= 2;
      escritos = 0;
    }
  }
  putchar('\n');
}