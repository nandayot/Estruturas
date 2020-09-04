/**************************************************
*
* FERNANDA TIEMI DE SOUZA TASO
* Trabalho 2
* Professor(a): DIEGO PADILHA RUBERT
*
*/
#include <cstdio>
#include <climits>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include <iostream>
#include <bitset>
#include <fstream> 
#include <sstream>
#include <numeric>

using std::string;
using std::vector;
typedef std::pair<unsigned char, string>hentry;
typedef std::map<unsigned char, string> htable;

//ABRIR ARQUIVO HUFFMAN 
//xxd -c 5 -b huffcompac.huff


/***
 * Classe No utilizada para criação e reconstrução
 * da árvore de Huffman
*/
class No {
  friend class Heap;
  
public:
  No(const int chave, const int freq);
  No();

  void escreve(const char *sep = "");
  void escreveNo(const char *sep);

  int chave; //SIMBOLOS DA TABELA ASCII
  int freq; //FREQUENCIA DOS SIMBOLOS -> USADO COMO CHAVE
  No *esq;
  No *dir;
};

/***
 * Classe No utilizada para criação e reconstrução da árvore de Huffman
 * Além disso foi utilizada para armazenar vetores auxiliares usados em
 * diversas partes do código.
*/
class Heap
{
  friend class No;
public:
  Heap();
  Heap(int n, int dados[]);
  ~Heap();

  std::map<unsigned char, string> tabela;
  vector<No*> S;
  vector<char> temp;
  vector<string> tofile;

  void huffmanToMap(No *x, bool ehDireito, vector<char>temp); // Método que mapeia o percurso pré-ordem da árvore para adicionar na estrutura Map (Tabela de Símbolos do arquivo)
  void escreve(const string& prefixo = "", int i = 0); // Função para testes
  void percursoPreOrdem(No *x, bool ehDireito, vector<char>temp); // Método que armazena o percurso pré-ordem
  void escreveHuffman(const string& prefixo, No* x, bool ehDireito, bool temIrmaoEsq); // Função para testes
  void escreveHuffmanBits(No* x, bool ehDireito); // Método responsável por armazenar o binário dos símbolos do arquivo
  void insereHuffmanBits(string s); // Usado em escreveHuffmanBits
  void inserePreOrdem(char sep); // Usado no percursoPreOrdem
  No* criaArvoreHuffman(Heap h); // Método que cria a árvore Huffman a partir a Min-heap
  No* extrai_minimo(); // Usado em criaArvoreHuffman

  /**
   * Métodos auxiliares para construção da Min-heap
  */
  void insere(No* p);
  int pai(int i);
  int esquerdo(int i);
  int direito(int i);
  void troca(int i, int j);
  void desce(int d);
  void sobe(int i);
};


/** prototipação **/
void limpa(No* x); 
No* reconstroiArvoreHuffman(vector<int>, vector<char>&); // Método que reconstroi a arvore Huffman a partir do percurso pré-ordem
void insereReconstrucaoHuffman(No*, vector<char>&, vector<int>&); // Usado em reconstroiArvoreHuffman
void escreveHuffmanRecons(const string&, No*, bool, bool); // Função para testes

//Função que converte Big Endian para Little Endian
//Usada para conversão do K byte.
std::bitset<16> reverse(std::bitset<16> &set)
{
  unsigned short n = static_cast<unsigned short>(set.to_ulong());
  //printf("bit %ld\n", set.to_ulong());
  return std::bitset<16>((n >> 8) | (n << 8));
}


// Função auxiliar que receber um vetor com dois inteiros
// combina-os e transforma para binário. Usada para devolver
// o Big Endian do K (2 bytes) em inteiro.
int converteBinario(int vetor[]){

  std::bitset<16> p3( vetor[0] + vetor[1] );
  //printf("bit %s ", p3.to_string().c_str());
  //int convertido = (int) p3.to_ulong();
  //printf("%d\n", (int) p3.to_ulong());
  return (int) p3.to_ulong();;
}

/**
 * Função específica que corrige o erro de escrita no
 * arquivo binário. Tranforma o valor string do vector
 * para unsigned char que é adicionado em outro vector
*/
unsigned char stringToBinary(char tmpSubjectStatus[])
{
  int tmpInteger = 0;
  while (*tmpSubjectStatus)
  {
    tmpInteger <<= 1;
    if (*tmpSubjectStatus == '1')
      tmpInteger += 1;
    tmpSubjectStatus++;
  }
  //printf("%d", tmpInteger);
  return (unsigned char) tmpInteger;
}

int main(int argc, char* argv[])
{

  //PARTE DE COMPACTAÇÃO
  if(strcmp(argv[1], "c") == 0){ 
    Heap h; // construtor Heap()
    int simbolosDistintos = 0;

    int freq[256] = {0}; //vetor para armazenar as frequencias dos símbolos do arquivo
    char* arquivoEscrita = argv[3]; 
    //printf("compactacao\n");

    FILE * pFile;
    wchar_t mystring [100];
    int ind1;
    const int SIZE = 1024*1024*3;
    unsigned char palavra1[SIZE];
    unsigned char cc = 130;
    pFile = fopen (argv[2] , "r");
    if (pFile != NULL)
    {
      ind1 = fscanf(pFile, "%[^\n]", palavra1);
      while(ind1 != EOF){
          printf("%s\n", palavra1);
          int lenword = strlen((const char*) palavra1);
          for (int i = 0; i < lenword; i++)
          {
            printf("%u ", palavra1[i]);
          }
          

          //lê a proxima palavra
          ind1 = fscanf(pFile, "%[^\n]", palavra1);
      }
      fclose (pFile);
    }
    return 0;

    for(int i = 0; i < 256; i++)
    {
        if(freq[i] != 0 && (i > 31 || i == 10 || i == 13))
        {
          //printf("%c %d\n", i, freq[i]);

          //identifica simbolos distintos e suas frequencias
          No* s = new No(i, freq[i]);
          h.insere( s );
          simbolosDistintos++;
          s = NULL;
        }
    }

    //return 0;

    //Tranforma para little endian como exigido
    std::bitset<16> sDistintosBits(simbolosDistintos);
    std::bitset<16> sDistintosBitsLittleEndian = reverse(sDistintosBits);
    //printf("bits dos simbolos: %s\n", sDistintosBitsLittleEndian.to_string().c_str());

    //primeiro 2 bytes do arquivo compactado colocado no vetor temporário
    h.tofile.push_back(sDistintosBitsLittleEndian.to_string().c_str());

    /*********** CRIA HUFFMAN E TABELA HASH ************/

    No* raiz = h.criaArvoreHuffman(h);
    //h.escreveHuffman("",raiz, false, false);
    h.escreveHuffmanBits(raiz, false);
    h.huffmanToMap(raiz, false, h.temp);

    /**************** COMPACTAÇÃO ********************/

    vector<char> temp;
    //printf("percurso pre-ordem\n");

    //Função que insere os bits do percurso pre ordem e transforma numa string
    h.percursoPreOrdem(raiz, false, temp);
    std::string str(h.temp.begin(), h.temp.end());

    //Logo em seguida é adicionada no vetor
    h.tofile.push_back(str.c_str());
    limpa(raiz);
    /******** MAP DE CADA SIMBOLO DO TEXTO *********/

    std::ifstream file2(argv[2]);
    char d;
    while (file2.get(d))  
      if(d != 13){
        //printf("d = %d\n", d);
        if(d == 10){
          //printf("esta pegando o \\n sim\n");
          //printf("%s\n", h.tabela.find(d)->second.c_str());
        }
        //printf("%s", h.tabela.find(d)->second.c_str());

        //Os símbolos em binário são adicionados no vetor com a ajuda do mapeamento
        h.tofile.push_back(h.tabela.find(d)->second.c_str());
      }
    //printf("\n");

    //return 0;

    /**** Nesta parte todos os simbolos estão transformados
     * em números binários dentro de um vetor de string 
     * abaixo eu implodi o vetor e uma única variável string 
     * para depois eu separar por byte. ****/
    std::string aux;
    aux = std::accumulate(h.tofile.begin(), h.tofile.end(), std::string(""));
    //printf("%s\n", aux.c_str());
    //printf("\n");
    
    //return 0;


    /***** Nesta parte eu vou separar os caracteres em bytes
     * e agrupar dentro de um vetor para depois fazer a escrita
     * em um arquivo binário **/
    int cont = 0;
    vector<char>bitbybit; //vetor para inserir bit por bit
    vector<string>bytebybyte; //vetor que insere byte por byte
    auto &last = *(--aux.end()); //armazena o ultimo bit
    for(char& c : aux) {
      if(cont == 8){
        cont=0;
        std::string bits(bitbybit.begin(), bitbybit.end());
        //printf("\n%s\n", bits.c_str());
        bytebybyte.push_back(bits.c_str());
        bitbybit.clear();
      }
      cont++;
      bitbybit.push_back(c);
      if (&c == &last){
        std::string bits(bitbybit.begin(), bitbybit.end());
        //Condição usada para quando o bit for menor que um byte inteiro
        //Nesse caso eu faço o processo de complementar o bit até virar 1 byte
        if(strlen(bits.c_str()) < 8){
          int complemento = 8 - strlen(bits.c_str()); //armazeno o complemento
          string zeros = std::string(complemento, '0').c_str(); //transformo o complemento em uma string '0'*complemento
          string valor = bits + zeros; //adiciono no valor do bit, formando 1 byte
          bytebybyte.push_back(valor);

          std::bitset<8> sobra(complemento); //O complemento também é transformado em binário como exigido e se torna o ultimo byte do arquivo
          bytebybyte.push_back(sobra.to_string().c_str());
        }
      }
      //printf("%c ", c);
    }
    //printf("\n");

    aux.clear();
    bitbybit.clear();

    /**
     * Parte que transforma o vetor de bytes (string)
     * em vetor de (unsigned char) para que o arquivo binário consiga ler corretamente
    */
    vector<string>::iterator bit;
    vector<unsigned char>byteConvert;
    for (bit = bytebybyte.begin(); bit != bytebybyte.end(); ++bit) {
      byteConvert.push_back(stringToBinary((char*)(*bit).c_str()));
      //printf("%s\n", (*bit).c_str());
    }

    bytebybyte.clear();
    //printf("\n");
    //return 0;
    
    /**
     * Escrita do vetor no arquivo
    */
    std::ofstream outfile (arquivoEscrita, std::ofstream::binary);
    outfile.write((const char*)(&byteConvert[0]), byteConvert.size());
    outfile.close();

    byteConvert.clear();
    //printf("depois de compac\n");
  }
  else {
    /*** PARTE DESCOMPACTAÇÃO ***/

    //printf("descompactacao\n");

    std::ifstream ifs(argv[2], std::ios::binary); //ARQUIVO BINÁRIO
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(ifs), {}); //essa linha coloca todos os dados do arquivo em um vecto buffer
    
    /**
     * Primeira tarefa que faço é pegar o byte de sobre e
     * armazená-lo. Logo em seguida eu apago ele d buffer
    */
    int sobra = buffer.back(); 
    buffer.pop_back();

    int posicaoArq = 0, numeroLetrasDistintas=0;
    int letrasDistintas[2];
    vector<int>letrasDoArquivo;
    vector<string>simbolosEmBinario; 
    std::vector<unsigned char>::iterator rd;
    for(rd = buffer.begin(); rd != buffer.end(); ++rd){ //PERCORRENDO O ARQUIVO
      //std::cout << static_cast<unsigned>(*rd) << "\n";

      if(posicaoArq < 2){  //condição para ler os dois primeiros bytes
        if(posicaoArq == 0){ //Nessa parte já armazeno em big endiam os 2 primeiros bytes do arquivo
          letrasDistintas[1] = (*rd); 
          //printf("%d\n", letrasDistintas);
        } else {
          letrasDistintas[0] = (*rd);
        }
      } 
      else {
        if(posicaoArq == 2){
          numeroLetrasDistintas = converteBinario(letrasDistintas);
        }
        /* lendo as letras do alfabeto */
        //printf("%d\n", numeroLetrasDistintas);
        if(numeroLetrasDistintas > 0){ //condição que lê os N símbolos do arquivo. O N depende do tamanho do K (letrasDistintas)
          letrasDoArquivo.push_back((*rd));
          numeroLetrasDistintas--;
          //printf("%c\n", (*rd));
        } else {
          /**
           * Essa parte vai ser responsável por transformar de volta o arquivo em numeros binários
           * para depois fazer a leitura novamente e conseguir reconstruir a árvore de huffman
          */
          std::bitset<8> bin_x((*rd));
          simbolosEmBinario.push_back(bin_x.to_string().c_str());
          //break;
        }
        
      }
      posicaoArq++;
    }

    /**
     * Função que transforma os bytes em bits e armazena no vetor
     * para conseguir reconstruir a árvore de huffman
     **/
    vector<char>simbolosEmBits; 
    std::vector<string>::iterator st;
    for(st = simbolosEmBinario.begin(); st != simbolosEmBinario.end(); ++st){
      //printf("%s\n", (*st).c_str());
      for(char& c : (*st)){
        simbolosEmBits.push_back(c);
      }
    }
   
    simbolosEmBinario.clear();
    //Reconstrução
    No* raiz;
    raiz = reconstroiArvoreHuffman(letrasDoArquivo, simbolosEmBits);
    //escreveHuffmanRecons("", raiz, false, false);

    //Faz uma cópia da raiz para ser usada na decodificação
    No* aux = raiz;
    letrasDoArquivo.clear();

    /** Essa parte eu juntei o vetor de bits em uma única string já tirando 
     *  os bits de sobre que previamente eu tinha coletado **/
    std::string bitsFundido, dadosArq;
    bitsFundido = std::accumulate(simbolosEmBits.begin(), simbolosEmBits.end()-sobra, std::string(""));
    
    simbolosEmBits.clear();

    for(char& c : bitsFundido){
      if (c == '0') // anda na arvore;
        aux = aux->esq; 
      else
        aux = aux->dir; 

      // é folha
      if (aux->esq==NULL && aux->dir==NULL) { 
        //printf("%c", aux->chave);
        dadosArq += aux->chave; //armazena o símbolo numa string e vai somando até acabar o vetor de bits
        aux = raiz; 
      } 
    }

    limpa(raiz);
    bitsFundido.clear(); 
    //printf("%s", dadosArq.c_str());
    //printf("\n");

    //Escrita da string no arquivo
    std::ofstream out(argv[3]);
    out << dadosArq;
    out.close();
    return 0;
 
  }
  return 0;
  

}

No::No() {
}

No::No(const int chave, const int freq) :
  chave(chave),
  freq(freq),
  esq(NULL),
  dir(NULL) {
}

void No::escreve(const char *sep) {
  printf("%2d%s", chave, sep);
}

Heap::Heap() {
}

/**
 * Destrutor da classe Heap que libera todos os vetores usados
*/
Heap::~Heap() {
  S.clear();
  temp.clear();
  tofile.clear();
  tabela.clear();
}

/**
 * Função auxiliar que limpa um nó recursivamente
 * Usada para liberar memória depois que a utiliza
*/
void limpa(No *x) {
  if(x == NULL) return;
  limpa(x->esq);
  limpa(x->dir);
  delete x;
}


/**
 * Funções para criação da árvore huffman
*/

No* Heap::criaArvoreHuffman(Heap h){
  while(h.S.size() > 1){
    No* no = new No(0,0);
    no->esq = no->dir = NULL;

    no->esq = h.extrai_minimo();
    no->dir = h.extrai_minimo();
    no->freq = no->dir->freq + no->esq->freq;
    //printf("frequencia do no : %d\n", no->freq);
    h.insere(no);
    no = NULL;
  }
  return h.extrai_minimo();
}

No* Heap::extrai_minimo() {
  troca(0, S.size()-1);
  No* menor = S[S.size()-1];
  S.pop_back();
  desce(0);
  return menor;
}

/**
 * Função que transforma os símbolos do arquivo em 
 * número binário correspondente
*/
void Heap::escreveHuffmanBits(No *x, bool ehDireito) {
  if (x == NULL){
    return;
  }

  if(x->chave){
    std::bitset<8> bin_x(x->chave);
    insereHuffmanBits(bin_x.to_string().c_str());
    //printf("%s", bin_x.to_string().c_str());
  }
  else{
    ehDireito = false;
  }
  
  //std::string str(temp.begin(), temp.end());
  //printf("%s\n", str.c_str());
  //x->escreveNo(ehDireito ? "1\n" : "0\n");
  escreveHuffmanBits(x->esq, ehDireito);
  escreveHuffmanBits(x->dir, ehDireito);
}

/**
 * Mapeamento dos símbolos do arquivo levando em conta os caracteres especiais
 * espaçamento e quebras de linhas.
*/
void Heap::huffmanToMap(No *x, bool ehDireito, vector<char>temp) {

  if (x == NULL){
    return;
  }
  if(x->chave){
    std::string str(temp.begin(), temp.end());
    //tabela.insert(hentry(x->chave, str));
    if(x->chave == '\n') {
      tabela.insert(hentry(10, str));
      //printf("\\n e bits %s\n", str.c_str()); //INSERE CHAVE E VECTOR NA MAP
    } else if(x->chave == ' '){
      tabela.insert(hentry(32, str));
    } else if(x->chave == 13) {
      tabela.insert(hentry(13, str));
    } else {
      tabela.insert(hentry(x->chave, str));
      //printf("chave %c e bits %s\n", x->chave, str.c_str()); //INSERE CHAVE E VECTOR NA MAP
    }
  }
 
  //x->escreveNo(ehDireito ? "1\n" : "0\n");
  if(!x->chave){
    temp.push_back('0');
    //printf("inseriu 0\n"); //INSERE ZERO NO VECTOR
  }
  huffmanToMap(x->esq, false, temp);
  if(!x->chave){
    temp.pop_back();
    temp.push_back('1');
    //printf("inseriu 1\n"); //INSERE UM NO VECTOR
  }
  huffmanToMap(x->dir, true, temp);
  temp.pop_back();
  //printf("tirou ultimo elemento\n"); //TIRA ULTIMO ELEMENTO

}

/**
 * Função que caminha na árvore Huffman em pré-ordem
 * e armazena o caminho num vetor temporário
*/
void Heap::percursoPreOrdem(No *x, bool ehDireito, vector<char>temp) {
  // percurso pre-ordem
  if (x == NULL){
    return;
  }

  if(x->chave)
    ehDireito = true;
  else
    ehDireito = false;
  
  //std::string str(temp.begin(), temp.end());
  //printf("%s\n", str.c_str());
  //x->escreveNo(ehDireito ? "1\n" : "0\n");
  inserePreOrdem(ehDireito ? '1' : '0');
  percursoPreOrdem(x->esq, ehDireito, temp);
  percursoPreOrdem(x->dir, ehDireito, temp);

}

void Heap::inserePreOrdem(const char sep) {
  temp.push_back(sep);
}

void Heap::insereHuffmanBits(string s){
  //printf("inserindo %s\n", s.c_str());
  tofile.push_back(s);
}

No* reconstroiArvoreHuffman(vector<int> letrasDoArquivo, vector<char>&alfaBits){
  No* no = new No(0, 0);
  insereReconstrucaoHuffman(no, alfaBits, letrasDoArquivo);  
  //escreveHuffmanRecons("", no, false, false);
  return no;
}

/**
 * Função que reconstroi a arvore huffman a partir do percurso pre-ordem
 * É passado por referência dois vetores. O vetor contendo os símbolos do 
 * alfabeto já na ordem da árvore e o vetor contendo todos os bits do arquivo
 * Assim cada bit lido é apagado para a próxima chamada o mesmo vale para o vetor de símbolos
*/
void insereReconstrucaoHuffman(No* no, vector<char>& it, vector<int>& lt){
  //printf("ch = %c \n", it.front());

  if(it.front() == '1'){
    no->chave = lt.front();
    lt.erase(lt.begin());
    it.erase(it.begin());
  }
  else {
    //No* nesq = new No(0, 0);
    //No* ndir = new No(0, 0);
    no->esq = new No(0, 0);
    no->dir = new No(0, 0);
    //nesq = NULL;
    //ndir = NULL;
    it.erase(it.begin());
    insereReconstrucaoHuffman(no->esq, it, lt);
    insereReconstrucaoHuffman(no->dir, it, lt);
  }
}

/**
 * Funções auxiliares para construção da Min-heap
*/

void Heap::escreve(const string& prefixo, int i) {
  if (i < (int) S.size()) {
    bool ehEsquerdo = i % 2 != 0;
    bool temIrmao = i < (int) S.size()-1;
    
    printf("%s", prefixo.c_str());
    printf(ehEsquerdo && temIrmao ? "├──" : "└──" );

    printf("%d %c\n", S[i]->freq, S[i]->chave);
    
    escreve(prefixo + (ehEsquerdo ? "│   " : "    "), esquerdo(i));
    escreve(prefixo + (ehEsquerdo ? "│   " : "    "), direito(i));
  }
}

void Heap::insere(No* p) {
  //printf("inserindo.. %d\n", p->chave);
  S.push_back(p);
  sobe(S.size()-1);
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
  No* aux = S[i];
  S[i] = S[j];
  S[j] = aux;
}

void Heap::desce(int d) {
  int esq, dir, maior;
  int n = S.size();
  esq = esquerdo(d);
  dir = direito(d);
  if (esq < n && S[esq]->freq < S[d]->freq)
    maior = esq;
  else
    maior = d;
  if (dir < n && S[dir]->freq < S[maior]->freq)
    maior = dir;
  if( maior != d){
    troca(d, maior);
    desce(maior);
  }

}

void Heap::sobe(int i) {
  while (S[pai(i)]->freq > S[i]->freq) {
    troca(i, pai(i));
    i = pai(i);
  }
}


/**
 * 
 * 
 * Funções para Testes
 * 
 * 
*/


void No::escreveNo(const char *sep) {
  if (chave == '\n'){
    printf("%d \\n %s", freq, sep);
  } else{
    printf("%d %c %s", freq, chave, sep);
  }
}

void Heap::escreveHuffman(const string& prefixo, No *x, bool ehDireito, bool temIrmaoEsq) {
  if (x == NULL)
    return;

  printf("%s", prefixo.c_str());
  printf(ehDireito && temIrmaoEsq ? "├──" : "└──" );

  if (prefixo.length() == 0) // raiz
    x->escreveNo("\n");
  else
    x->escreveNo(ehDireito ? "d\n" : "e\n");

  escreveHuffman(prefixo + (ehDireito && temIrmaoEsq ? "│   " : "    "), x->dir, true, x->esq != NULL);
  escreveHuffman(prefixo + (ehDireito && temIrmaoEsq ? "│   " : "    "), x->esq, false, false);
}

void escreveHuffmanRecons(const string& prefixo, No *x, bool ehDireito, bool temIrmaoEsq) {
  if (x == NULL)
    return;

  printf("%s", prefixo.c_str());
  printf(ehDireito && temIrmaoEsq ? "├──" : "└──" );

  if (prefixo.length() == 0) // raiz
    x->escreveNo("\n");
  else
    x->escreveNo(ehDireito ? "d\n" : "e\n");

  escreveHuffmanRecons(prefixo + (ehDireito && temIrmaoEsq ? "│   " : "    "), x->dir, true, x->esq != NULL);
  escreveHuffmanRecons(prefixo + (ehDireito && temIrmaoEsq ? "│   " : "    "), x->esq, false, false);
}