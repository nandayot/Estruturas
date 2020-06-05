#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <utility>

using std::vector; // Usado apenas para a impressão
using std::string; // Usado apenas para a impressão
using std::pair;   // Usado para funções devolverem um par de valores



// Declaração de classe responsável pela impressão, NÃO TOQUE!
// As funções de impressão foram agrupadas nesta classe para
// não poluir a implementação. A definição e implementação
// da classe encontram-se no final do arquivo. O código foi
// obtido da internet e adaptado à nossa implementação.
// https://stackoverflow.com/questions/36829399/want-to-print-out-a-pretty-btree
class BArvorePrinter;


// Nó de uma B-Árvore
class No {
  friend class BArvore;
  
public:
  No(int chave);
  ~No();

  void escreve(const char *sep = " ") const;
  inline bool eh_folha() const;      // devolve true se for folha
  inline int get_n() const;          // devolve a quantidade de chaves no nó
  inline int get_chave(int i) const; // devolve a chave na posição i
  inline No *get_filho(int i) const; // devolve o filho na posição i
    
private:
  int n;
  int *chave;
  No **filho;
  bool folha;

  void insere(int k); // insere a chave k na posição correta (supõe que não está cheio), move filhos se não for folha
  void remove(int i); // remove o elemento na posição i, movendo elementos após ele (e filhos, se não for folha)
};


// B-Árvore
class BArvore
{
public:
  BArvore(int t);
  BArvore(const BArvore& outro); // construtor de cópia
  ~BArvore();
  BArvore& operator=(const BArvore& outro); // operador de atribuição
  
  void escreve_ordenado(); // escreve elementos em ordem crescente
  void escreve();

  No *get_raiz() const;         // devolve a raiz
  pair<No *, int> busca(int k); // devolve o ponteiro para um elemento, se achar, ou NULL, e a posição

  pair<No *, int> minimo();                  // devolve o nó com a menor chave da árvore e a sua posição no nó
  pair<No *, int> maximo();                  // devolve o nó com a maior chave da árvore e a sua posição no nó
  pair<No *, int> sucessor(No *x, int i);    // devolve o nó que contém o sucessor do elemento i de x e sua posição no nó (assume que x não é folha)
  pair<No *, int> predecessor(No *x, int i); // devolve o nó que contém o predecessor do elemento i de x e sua posição no nó (assume que x não é folha)
  
  void insere(int k); // chamada inicial da inserção, divide raiz se necessário  
  bool remove(int k); // busca e remove uma chave k na árvore

  void limpa(); // remove todos elementos da árvore
  
private:
  int t;
  No *raiz;
  BArvorePrinter *printer;

  void escreve_ordenado(No *x); // escreve elementos em ordem crescente
  
  pair<No *, int> busca(No *x, int k); // chamada recursiva inicial para a raiz
  pair<No *, int> minimo(No *x);       // devolve o nó com a menor chave da árvore e a sua posição no nó
  pair<No *, int> maximo(No *x);       // devolve o nó com a maior chave da árvore e a sua posição no nó
  
  void divide_filho(No *x, int i);     // divide o filho (cheio) de x que se encontra na posição filho[i]
  void insere_nao_cheio(No *x, int k); // insere recursivamente na árvore, dividindo nós no caminho (então x nunca está cheio)
  void agrupa_filho(No *x, int i);     // agrupa em filho[i] seu conteúdo original + chave[i] + o conteúdo de filho[i+1] (filhos de grau mínimo)
  bool remove(No *x, int k);           // busca e remove uma chave k a partir de x
  
  void limpa(No *x); // dado um nó x, remove recursivamente todos elementos abaixo e deleta x
  
  void copia(const BArvore& T); // copia uma árvore T para a atual a partir da raiz,
                                // usada no construtor de cópia e no operador de atribuição
  void copia(No *dest, No *orig); // copia um nó e os descendentes recursivamente

  template <typename T>                                    // função auxiliar, desloca 1 pos. p/ a esquerda
  static void desloca_esq(T vetor[], int inicio, int fim); // os elementos [inicio,fim]  (início-1 é sobrescrito)  
  template <typename T>                                    // função auxiliar, desloca 1 pos. p/ a direita 
  static void desloca_dir(T vetor[], int inicio, int fim); // os elementos [inicio,fim]  (fim é movido p/ fim+1)
};

// valgrind --leak-check=full --show-reachable=yes --track-fds=yes
int main(void)
{
  BArvore T(3); // construtor BArvore()
  int v[] = {17, 43, 2, 31, 50, 14, 29, 6, 27, 7, 32, 21, 52, 36, 3, 23, 58, 72, 84, 79, 19, 9, 11};

  printf("##TESTE DE INSERÇÃO##\n\n");
  for (const auto &x : v) {
    printf("\nInserindo %d...\n", x);
    T.insere(x); // TODO: é necessário implementar a função insere_nao_cheio
    printf("T:\n");
    T.escreve();
  }
  printf("\n\nElementos ordenados: ");
  T.escreve_ordenado();

  //return 0; // TODO: remova após implementar a busca, mínimo e máximo
  //printf("e to aqui cara\n");
  pair<No *, int> min = T.minimo();
  pair<No *, int> max = T.maximo();
  printf("Mínimo: %02d\n", min.first->get_chave(min.second));
  printf("Máximo: %02d\n", max.first->get_chave(max.second));

  pair<No *, int> elem = T.busca(19);
  printf("Busca 19: %s\n", elem.first ? "Sim" : "Não");
  elem = T.busca(37);
  printf("Busca 37: %s\n", elem.first ? "Sim" : "Não");
  elem = T.busca(23);
  printf("Busca 23: %s\n", elem.first ? "Sim" : "Não");

  pair<No *, int> pred = T.predecessor(elem.first, elem.second);
  printf("Predecessor do 23: %d\n", pred.first->get_chave(pred.second));
  No *raiz = T.get_raiz();
  pair<No *, int> suc = T.sucessor(raiz, 0);
  printf("Sucessor do 1o elemento da raiz: %d\n", suc.first->get_chave(suc.second));

  //return 0; // TODO: remova após testar as funções acima, não é
            // necessário implementar nenhuma função adicional para
            // que o construtor de cópia e a remoção funcionem (MAS
            // LEIA O CÓDIGO DA CÓPIA E DA REMOÇÃO!!!)
  
  printf("\n##TESTE DE CONSTRUTOR DE CÓPIA##\n\n");
  BArvore T2(T);
  printf("T2:\n");
  T2.escreve();
  
  printf("\n##TESTE DE REMOÇÃO##\n\n");
  printf("T:\n");
  T.escreve();
  for (const auto &x : v) {
    printf("\nRemovendo %d...\n", x);
    T.remove(x);
    printf("\nT:\n");
    T.escreve();
  }

  //return 0; // TODO: remova após implementar a função limpa
  
  printf("\n##TESTE DE ATRIBUIÇÃO##\n\n");
  T = T2;
  printf("T:\n");
  T.escreve();

  return 0;
}



//*******************************************
//*** DECLARAÇÃO DA CLASSE BArvorePrinter ***
//*******************************************
// Precisa vir antes da implementação de BArvore

class BArvorePrinter
{
  struct NodeInfo
  {
    No const *node;
    unsigned text_pos, text_end; // [pos, end)
  };
  
  typedef vector<NodeInfo> LevelInfo;
  
  vector<LevelInfo> levels;
  
  string node_text(int const keys[], unsigned key_count);
  
  void before_traversal() {
    levels.resize(0);
    levels.reserve(10);   // assumimos no máximo 10 níveis
  }

  void visit(No const *node, unsigned level = 0, unsigned child_index = 0);
  
  void after_traversal();
  
  void print_blanks(unsigned n) {
    while (n--) putchar(' ');
  }

  unsigned node_strlen(No const *node) {
    // assumimos que:
    // 1. nós são impressos em 2 dígitos
    // 2. valores apenas entre 0 e 99
    // 3. impressão iniciada por [, terminada por ] e com um caractere de separador
    return node->get_n()*3 + 1;
  }

public:
  void print(BArvore const *tree);
};


//***********************************
//*** IMPLEMENTAÇÕES DA CLASSE No ***
//***********************************

No::No(int t) :
  n(0),
  folha(true) {
  // não armazenamos t, se necessário em alguma função deverá ser passado como parâmetro
  chave = new int[2*t - 1];
  filho = new No*[2*t];
}

No::~No() {
  delete[] chave;
  delete[] filho;
}

bool No::eh_folha() const {
  return folha;
}

int No::get_n() const {
  return n;
}

int No::get_chave(int i) const {
  return chave[i];
}

No *No::get_filho(int i) const {
  return filho[i];
}

void No::escreve(const char *sep) const {
  putchar('[');
  const char *end = "]";
  for (int i = 0; i < n; ++i)
    printf("%02d%s", chave[i], i < n-1 ? sep : end);
}

void No::insere(int k) {
  int i;
  for (i = n-1; i >= 0 && chave[i] > k; --i) // não usamos desloca_dir pois temos que achar a posição correta de k
    chave[i+1] = chave[i];
  chave[i+1] = k;
  ++n;
}

void No::remove(int i) {
  for (int j = i; j < n-1; ++j)
    chave[j] = chave[j+1];
  --n;
}


//*****************************************
//*** IMPLEMENTAÇÕES DA CLASSE BArvore ***
//*****************************************

BArvore::BArvore(int t) :
  t(t) {
  raiz = new No(t);
  printer = new BArvorePrinter();
}

BArvore::BArvore(const BArvore& outro) {
  printer = new BArvorePrinter();
  copia(outro);
}

BArvore::~BArvore() {
  limpa();
  delete printer;
}

BArvore& BArvore::operator=(const BArvore& outro) {
  limpa();
  copia(outro);
  return *this;
}

void BArvore::escreve_ordenado() {
  escreve_ordenado(raiz);
  putchar('\n');
}

void BArvore::escreve_ordenado(No *x) {
  // percurso similar ao em-ordem
  if (x->folha)
    x->escreve();
  else {
    for (int i = 0; i < x->n; i++) {
     escreve_ordenado(x->filho[i]);
     printf(" %02d ", x->chave[i]);
    }
    escreve_ordenado(x->filho[x->n]);
  }
}

void BArvore::escreve() {
  printer->print(this);
}

No *BArvore::get_raiz() const {
  return raiz;
}

pair<No *, int> BArvore::busca(int k) {
  if (raiz == NULL)
    return pair<No *, int>(NULL, -1);
  return busca(raiz, k);
}

pair<No *, int> BArvore::busca(No *x, int k) {
  // TODO: Implemente o algoritmo de busca. Ele deve devolver um
  // pair<No *, int>(x, i); com o nó x e a posição i onde a chave k se
  // encontra, ou pair<No *, int>(NULL, -1); se não encontrar.
  int i = 0;
  while(i < x->n && k > x->chave[i]){
    i++;
  }
  if(i < x->n && k == x->chave[i]){
    return pair<No *, int>(x, i);
  } else if(x->folha){
    return pair<No *, int>(NULL, -1);
  } else{
    return busca(x->filho[i], k);
  }

}

pair<No *, int> BArvore::minimo() {
  return (raiz->n > 0) ? minimo(raiz) : pair<No *, int>(NULL, -1);
}

pair<No *, int> BArvore::minimo(No *x) {
  // TODO: implementar o algoritmo para encontrar o mínimo na
  // subárvore com raiz em x. Não é necessária uma função recursiva.
  // A função deve devolver um pair<No *, int>(nó, posição) com o nó
  // onde está o mínimo (será uma folha) e a posição da chave mínima
  // neste nó.

  while(!x->eh_folha()){ //enquanto não for folha, continua
    x = x->filho[0];
  }
  return pair<No *, int>(x, 0);
}

pair<No *, int> BArvore::maximo() {
  return (raiz->n > 0) ? maximo(raiz) : pair<No *, int>(NULL, -1);
}

pair<No *, int> BArvore::maximo(No *x) {
  // TODO: implementar o algoritmo para encontrar o máximo na
  // subárvore com raiz em x. Não é necessária uma função recursiva.
  // A função deve devolver um pair<No *, int>(nó, posição) com o nó
  // onde está o máximo (será uma folha) e a posição da chave máxima
  // neste nó.
  //int i = 0;
  while(!x->eh_folha()){ //enquanto não for folha pega o maior filho
    x = x->filho[x->n];
  }
  return pair<No *, int>(x, x->n-1);
}

pair<No *, int> BArvore::sucessor(No *x, int i) {
  return minimo(x->filho[i+1]);
}

pair<No *, int> BArvore::predecessor(No *x, int i) {
  return maximo(x->filho[i]);
}

void BArvore::divide_filho(No *x, int i) {
  
  No *z = new No(t);
  No *y = x->filho[i];

  z->folha = y->folha;
  z->n = t-1;

  memcpy(z->chave, y->chave + t, (t-1) * sizeof(int)); // copia t-1 últimas chaves de y para z
  if (! y->folha) memcpy(z->filho, y->filho + t, t * sizeof(No *)); // copia t últimos filhos de y para z

  y->n = t-1;
  for (int j = x->n; j >= i+1; --j) // "puxa" 1 posição para a direita ponteiros para "abrir espaço"
    x->filho[j+1] = x->filho[j];
  x->filho[i+1] = z;
  for (int j = x->n-1; j >= i; --j) // "puxa" 1 posição para a direita chaves para "abrir espaço"
    x->chave[j+1] = x->chave[j];
  x->chave[i] = y->chave[t-1];
  x->n++;
}

void BArvore::insere(int k) {
  if (raiz->n == 2*t -1) { // caso onde a raiz está cheia.
    No *s = new No(t);
    s->filho[0] = raiz;
    s->folha = false;
    raiz = s;
    divide_filho(s, 0);
  }
  insere_nao_cheio(raiz, k);
}

void BArvore::insere_nao_cheio(No *x, int k) {
  // TODO: implementar o algoritmo de inserção em nó não cheio.
  
  int i = (x->n)-1; 
  if (x->eh_folha()) { 
    //printf("x é folha\n");
    while (i >= 0 && x->chave[i] > k) 
    { 
      x->chave[i+1] = x->chave[i]; 
      i--; 
    } 

    x->chave[i+1] = k; 
    x->n++; 
  } 
  else { 
    //printf("nao eh folha i = %d n = %d\n", i, x->filho[i]->n);
    
    while (i >= 0 && k < x->chave[i]) {
      i--; 
    }
      
    if (x->filho[i+1]->n == 2*t-1) 
    {  
      //printf("entrou para dividir os filhos x->filho[i+1] = %d i+1 %d\n", *(x->filho[i+1]->chave), (i+1));
      divide_filho(x, i+1); 

      if (x->chave[i+1] < k) 
        i++; 
    } 
    insere_nao_cheio(x->filho[i+1], k);
  } 

}

void BArvore::agrupa_filho(No *x, int i) { // agrupa em filho[i] (=y) seu conteúdo original + chave[i] (=k) + o conteúdo de filho[i+1] (=z)
  No* y = x->filho[i];
  No* z = x->filho[i+1];

  // copia elementos p/ y
  y->chave[y->n++] = x->chave[i];
  for (int j = 0; j < z->n; j++) {
    y->chave[y->n] = z->chave[j];
    y->filho[y->n] = z->filho[j];
    y->n++;
  }
  y->filho[y->n] = z->filho[z->n];

  // arruma x, removendo sua chave[i] e seu filho[i+1]
  desloca_esq(x->chave, i+1, x->n-1);
  desloca_esq(x->filho, i+2, x->n);
  x->n--;

  // apaga o filho[i+1] de x
  delete z;
}

bool BArvore::remove(int k) {
  // tentamos remover k
  bool result = remove(raiz, k);

  // se removemos e a raiz ficou vazia, o seu único filho passa a ser a raiz
  // (a menos que raiz seja folha, ou seja, removemos o último elemento)
  if (result && raiz->n == 0 && ! raiz->folha) {
    No *filho = raiz->filho[0];
    delete raiz;
    raiz = filho;
  }
  return result;
}
  
bool BArvore::remove(No *x, int k) {
  int i = 0;
  
  while (i < x->n && x->chave[i] < k) // encontra a posição que está ou que deve descer
    ++i;

  if (i < x->n && k == x->chave[i]) { // encontramos k

    // CASO 1: encontramos k em uma folha
    if (x->folha) { printf("CASO 1 (k=%d)...", k);
      x->remove(i); // removemos o elemento da posição i
      return true;
    }
    // CASO 2: encontramos k em um nó interno
    else { printf("CASO 2 (k=%d)...", k);
      No *y = x->filho[i];   // filho que precede k
      No *z = x->filho[i+1]; // filho que sucede k
      
      // CASO 2a
      if (y->n >= t) { printf("CASO 2a (k=%d)...", k);
        // iremos aqui descer 2 vezes até uma folha, não é o mais eficiente mas em termos didáticos simplifica nosso código
        pair<No *, int> aux = predecessor(x, i); // encontramos o nó e posição do predecessor
        int pre = aux.first->chave[aux.second];
        x->chave[i] = pre;                       // copiamos o predecessor no lugar de k
        return remove(y, pre);                   // removemos o predecessor, que será a última chave de uma folha da árvore
      }
      // CASO 2b
      else if (z->n >= t) { printf("CASO 2b (k=%d)...", k);
        // iremos aqui descer 2 vezes até uma folha, não é o mais eficiente mas em termos didáticos simplifica nosso código
        pair<No *, int> aux = sucessor(x, i);   // encontramos o nó e posição do sucessor
        int suc = aux.first->chave[aux.second];
        x->chave[i] = suc;                      // copiamos o sucessor no lugar de k
        return remove(z, suc);                  // removemos o sucessor, que será a primeira chave de uma folha da árvore
      }
      // CASO 2c
      else { printf("CASO 2c (k=%d)...", k);
        agrupa_filho(x, i); // agrupa em filho[i] (=y) seu conteúdo original + chave[i] (=k) + o conteúdo de filho[i+1] (=z)
        return remove(y, k);
      }
    }
  }
  
  // k ∉ T: não encontramos k e estamos em uma folha, então k não está na árvore
  else if (x->folha)
    return false;

  // CASO 3: não encontramos k, mas ainda estamos em um nó interno
  else { printf("CASO 3 (k=%d)...", k);

    // Se formos descer para o filho i e este que possui grau mínimo, precisamos fazer algo
    if (x->filho[i]->n == t-1) {
      No *filho = x->filho[i];                     // filho i
      No *esq = i > 0 ? x->filho[i-1] : NULL;      // irmão esquerdo do filho i
      No *dir = i < x->n-1 ? x->filho[i+1] : NULL; // irmão direito do filho i
      
      // CASO 3a: algum irmão tem pelo menos t chaves
      if (esq != NULL && esq->n >= t) { printf("CASO 3a esq (k=%d)...", k); // esquerdo
        desloca_dir(filho->chave, 0, filho->n-1);
        desloca_dir(filho->filho, 0, filho->n);
        filho->n++;
        
        filho->chave[0] = x->chave[i-1];
        x->chave[i-1] = esq->chave[esq->n-1];
        filho->filho[0] = esq->filho[esq->n];
        esq->n--;
      }
      else if (dir != NULL && dir->n >= t) { printf("CASO 3a dir (k=%d)...", k); // direito
        filho->chave[filho->n] = x->chave[i];
        filho->filho[filho->n+1] = dir->filho[0];
        filho->n++;
        
        x->chave[i] = dir->chave[0];
        desloca_esq(dir->chave, 1, dir->n-1);
        desloca_esq(dir->filho, 1, dir->n);
        dir->n--;
      }
      
      // CASO 3b: seus irmãos também possuem grau mínimo (t-1 chaves)
      else { printf("CASO 3b (k=%d)...", k);
        if (esq != NULL)
          agrupa_filho(x, --i); // agora é para o filho[i-1] que devemos descer
        else // dir != NULL
          agrupa_filho(x, i);
      }
    }

    return remove(x->filho[i], k);
  }
  
}

void BArvore::limpa() {
  limpa(raiz);
  raiz = NULL;
}

void BArvore::limpa(No *x) {
  // TODO: implementar a deleção recursiva de x e tudo que estiver abaixo
    
  for (int i = 0; x && i <= x->n; i++) {
    if(!x->eh_folha())
      limpa(x->filho[i]);
  }
  delete x;
  
}

void BArvore::copia(const BArvore& T) {
  if (T.raiz == NULL)
    raiz = NULL;
  else {
    t = T.t;
    raiz = new No(T.t);
    copia(raiz, T.raiz);
    //memcpy(raiz->chave, T.raiz->chave, T.raiz->n * sizeof(int));
    //copia(raiz, T.raiz);
  }
}

void BArvore::copia(No *dest, No *orig) {
  memcpy(dest->chave, orig->chave, orig->n * sizeof(int));
  dest->folha = orig->folha;
  dest->n = orig->n;
  if (! orig->folha)
    for (int i = 0; i <= orig->n; ++i) {
      dest->filho[i] = new No(t);
      copia(dest->filho[i], orig->filho[i]);
    }
}

template <typename T>
void BArvore::desloca_esq(T vetor[], int inicio, int fim) {
  for (int i = inicio-1; i < fim; ++i)
    vetor[i] = vetor[i+1];
}

template <typename T>
void BArvore::desloca_dir(T vetor[], int inicio, int fim) {
  for (int i = fim; i >= inicio; --i)
    vetor[i+1] = vetor[i];
}


//***********************************************
//*** IMPLEMENTAÇÕES DA CLASSE BArvorePrinter ***
//***********************************************

void BArvorePrinter::visit(No const *node, unsigned level, unsigned child_index) {
  if (level >= levels.size())
    levels.resize(level + 1);
  
  LevelInfo &level_info = levels[level];
  NodeInfo info;
  
  info.text_pos = 0;
  if (!level_info.empty())  // one blank between nodes, one extra blank if left-most child
    info.text_pos = level_info.back().text_end + (child_index == 0 ? 2 : 1);
  
  info.node = node;
  
  if (node->eh_folha()) {
    info.text_end = info.text_pos + node_strlen(info.node);
  }
  else { // non-leaf -> do all children so that .text_end for the right-most child becomes known
    for (int i = 0, e = node->get_n(); i <= e; ++i)  // one more pointer than there are keys
      visit(node->get_filho(i), level + 1, i);
    
    info.text_end = levels[level + 1].back().text_end;
  }
  
  levels[level].push_back(info);
}

void BArvorePrinter::after_traversal() {
  for (size_t l = 0, level_count = levels.size(); ; ) {    
    auto const &level = levels[l];
    unsigned prev_end = 0;
    
    for (auto const &info: level) {         
      unsigned total = info.text_end - info.text_pos;
      unsigned slack = total - node_strlen(info.node);
      unsigned blanks_before = info.text_pos - prev_end;
      
      print_blanks(blanks_before + slack / 2);
      info.node->escreve();
      
      if (&info == &level.back())
        break;
      
      print_blanks(slack - slack / 2);
      
      prev_end += blanks_before + total;
    }
    
    if (++l == level_count)
      break;
    
    printf("\n\n");
  }
  
  putchar('\n');
}

void BArvorePrinter::print(BArvore const *tree) {
  No *r = tree->get_raiz();
  if (r->get_n() > 0) {
    before_traversal();
    visit(r);
    after_traversal();
  }
  else
    printf("[]\n");
}
