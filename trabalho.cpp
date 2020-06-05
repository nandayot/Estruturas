/**********************************
 * 
 * FERNANDA TIEMI DE SOUZA TASO
 * Trabalho 1
 * Professor: Diego Rubert
 * 
 */




#include <cstdio>
#include <cctype>
#include <cstring>
#include <stdlib.h>

#define MIN 3
#define MAX 20

//atributo estático para a contagem do número de chaves
static double chave = 0.0;

/* Célula de uma lista encadeada de ocorrências, armazena um inteiro */
class Ocorrencia {
  friend class ListaO;

public:
  Ocorrencia(int linha); // Construtor padrão
  ~Ocorrencia();         // Destrutor padrão
private:
  int linha;
  Ocorrencia *prox;
};


/* Lista encadeada de ocorrências, inserção realizada no fim da lista.
   Tenha o cuidado de não inserir duplicatas */
class ListaO {
public:
  ListaO();               // Construtor padrão
  ~ListaO();              // Destrutor padrão
  void insere(int linha); // Recebe um inteiro e insere na lista
  void escreve();         // Escreve os elementos da lista
  void limpa();
private:
  void limpa(Ocorrencia* o);
  Ocorrencia *inicio; // Início da lista
  Ocorrencia *fim;    // Fim da lista
};


/* Célula de uma lista encadeada de palavras, armazena uma cadeia de caracteres */
class Palavra {
  friend class ListaP;

public:
  Palavra(char p[]);      // Construtor padrão
  ~Palavra();             // Destrutor padrão
  void insere(int linha); // Insere uma ocorrência na lista de ocorrências
  void escreve();         // Escreve a palavra e suas ocorrências

private:
  char p[MAX];        // A palavra em si
  ListaO ocorrencias; // Lista de ocorrências
  Palavra *prox;      // Próxima Palavra na lista
};


/* Lista encadeada de palavras, inserção realizada no fim da lista.
   Tenha o cuidado de não inserir duplicatas */
class ListaP {
public:
  ListaP();                  // Construtor padrão
  ~ListaP();                 // Destrutor padrão
  Palavra *insere(char p[]); // Recebe uma palavra e insere na lista, devolvendo um ponteiro para o objeto inserido
  Palavra *busca(char p[]);  // Busca e devolve o objeto de uma palavra na lista
  void escreve();            // Escreve os elementos da lista
  void limpa();

private:
  void limpa(Palavra* p);
  Palavra *inicio; // Início da lista
  Palavra *fim;    // Fim da lista
};


/* Tabela de dispersão que guarda um índice de palavras */
class TabelaDispersao {
public:
  TabelaDispersao();  // Construtor padrão
  ~TabelaDispersao(); // Destrutor padrão
  void escreve();     // Escreve a tabela (incluindo fator de carga)
  void novaOcorrencia(char p[], int linha); // Registra uma nova ocorrência de uma palavra
  
private:
  static const int TAMANHO = 47; // Tamanho da tabela
  ListaP tabela[TAMANHO];        // Listas da tabela

  int valor(char p[]); // Valor da palavra p
  int h(char p[]);     // Função de hash
};



/******  CONSTRUTORES   ******/

//Inicialização dos atributos nos construtores

Ocorrencia::Ocorrencia(int linha){
  this->linha = linha;
  prox = NULL;
}

ListaO::ListaO(){
  inicio = fim = NULL;
}

//É preciso usar strcpy pois não é possível atribuir
// um char para outro utilizando o operador =.
Palavra::Palavra(char p[]): ocorrencias(){
  strcpy(this->p, p);
  this->prox = NULL;
}

ListaP::ListaP(){
  inicio = fim = NULL;
}

TabelaDispersao::TabelaDispersao(){
}

/******  FUNÇÕES   ******/

//Insere linha na lista de ocorrencias
void ListaO::insere(int linha){ 
  //verifica se já existe UMA ocorrencia
  if(inicio == NULL){
    //senão, ele atribui uma nova ocorrencia
    //faz o inicio e o fim receber a primeira ocorrencia
    //e o ponteiro para o próximo recebe NULL
    Ocorrencia *ocurr = new Ocorrencia(linha);
    inicio = ocurr;
    fim = ocurr;
    ocurr->prox = NULL;

  }

  //Verifica se a linha já existe na lista
  Ocorrencia *ptr, *p,*q;
  ptr = inicio;
  bool existe = false;
  //percorre a lista e caso encontre faz o atributo bool
  //receber true
  while (ptr != NULL)
	{
    if(ptr->linha == linha){
      existe = true;
    }
		ptr = ptr->prox;
	}
  //se a linha não existir, insere no final da lista
  if(existe == false){
    Ocorrencia *ocurr = new Ocorrencia(linha);
    p = inicio;
    q = inicio->prox;
    
    //ponteiros para percorrer a lista até chegar no final
    while(q != NULL)
    {
      p = q;
      q = q->prox;
    }
    
    //atribui o ponteiro prox da nova ocorrencia para NULL
    //faz o prox do ultimo elemento e o ponteiro fim
    //apontar para a nova ocorrencia
    
    ocurr->prox = q;
    p->prox = ocurr;
    fim = ocurr;
    ocurr->prox = NULL;

  }

} // Recebe um inteiro e insere na lista


//Escreve as ocorrencias no formato especificado
void ListaO::escreve(){
  Ocorrencia *o;
	for (o = inicio; o != NULL; o = o->prox){
    //a ultima ocorrencia é impressa sem a vírgula
    if(o->prox == NULL){
      printf("%d", o->linha);
    }else{
      printf("%d,", o->linha);
    }
  }
} // Escreve os elementos da lista


//Insere a linha na lista de ocorrencias
void Palavra::insere(int linha){ 
  ocorrencias.insere(linha);
} //Insere uma ocorrência na lista de ocorrências

//Escreve a palavra. Foi feita a transformação do char 
//para o número correspondente na tabela ASCII
void Palavra::escreve(){
  int tam = sizeof(p);
  for(int i = 0; i < tam ; i++){
    printf("%c", p[i]+32);
  }

} // Escreve a palavra e suas ocorrências


//Insere a palavra na lista de palavras
Palavra* ListaP::insere(char p[]){
  Palavra *ptr, *pp, *q;
  if(inicio == NULL){
    //Insere a primeira palavra da lista
    ptr = new Palavra(p);
    inicio = ptr;
    fim = ptr;
    ptr->prox = NULL;
    //faz os ponteiros inicio e fim apontar para ela
    return ptr;
  }else{
    //caso não seja a primeira palavra da lista
    //insere no final
    ptr = new Palavra(p);
    pp = inicio;
    q = inicio->prox;
    
    //ponteiros auxiliares para chegar no final da lista
    while(q != NULL)
    {
      pp = q;
      q = q->prox;
    }
    
    //Insere no final da lista semelhante a lista de ocorrencias
    ptr->prox = q;
    pp->prox = ptr;
    fim = ptr;
    ptr->prox = NULL;
    
    return ptr;
  }
  
} // Recebe uma palavra e insere na lista, devolvendo um ponteiro para o objeto inserido


//Função que busca uma palavra existente na lista
Palavra* ListaP::busca(char p[]){
  Palavra* ptr;

  ptr = inicio;

  //percorre a lista até o final
  while (ptr != NULL)
	{
    //compara a palavra da lista com a chave do parametro
    if(strcmp(ptr->p, p) == 0){
      //retorna o ponteiro da chave existente caso exista
      return ptr;
    }
		ptr = ptr->prox;
	}

  //Senão retorna NULL
  return ptr;
}  // Busca e devolve o objeto de uma palavra na lista

//Escreve as palavras no formato desejado
void ListaP::escreve(){

  Palavra* ptr;
  for(ptr = inicio; ptr != NULL; ptr = ptr->prox){
    //imprime com um espaço em branco na frente da
    //primeira palavra da lista
    if(ptr == inicio) printf(" %s",ptr->p);
    else printf("%s",ptr->p);
    printf("("); //parenteses para o início das ocorrencias
    ptr->ocorrencias.escreve();
    if(ptr->prox == NULL) printf(")"); //se for a ultima ocorrencia da ultima palavra da lista não tem espaço no final
    else printf(") ");

  }
  //printf("\n");
} // Escreve os elementos da lista


//Escreve a tabela
void TabelaDispersao::escreve(){
  for(int i=0; i < TAMANHO; i++){
    printf("%d:", i);
    tabela[i].escreve();
    printf("\n");
  }

} // Escreve a tabela (incluindo fator de carga)


//Insere novas ocorrencias na tabela
void TabelaDispersao::novaOcorrencia(char p[], int linha){
  Palavra *ptrword;
  int indice = h(p);
  //busca o parametro p caso exista na tabela
  ptrword = tabela[indice].busca(p);

  //caso não exista, insere na tabela conforme a função hash especificada
  if(ptrword == NULL){

    ptrword = tabela[indice].insere(p);
    chave++; //incremente o atributo estático para a contagem de chaves distintas

    ptrword->insere(linha); //insere a linha

  }else{
    ptrword->insere(linha); //insere somente a linha caso a palavra já exista
  }


} // Registra uma nova ocorrência de uma palavra
  

//Função hash que calcula o indice conforme a soma dos caracteres
//com base na tabela ASCII
int TabelaDispersao::h(char p[]){
    int ascii=0;
    for(int i = 0; p[i]; i++){
        ascii += p[i];
    }
    int h_p = ascii % 47;
    return h_p;
}

/******  DESTRUTORES   ******/

//Limpa as classes onde existe ponteiros de inicio e fim

Ocorrencia::~Ocorrencia(){
}

ListaO::~ListaO(){
  limpa();
}

Palavra::~Palavra(){
}

ListaP::~ListaP(){
  limpa();
}
TabelaDispersao::~TabelaDispersao(){
}

/***** LIMPADORES ******/

//Limpa a lista e atribui NULL para os ponteiros

void ListaO::limpa() {
  limpa(inicio);
  inicio = NULL;
  fim = NULL;
}

//Chama recursivamente a função limpa para limpar até o final

void ListaO::limpa(Ocorrencia *x) {
  if(x == NULL) return;
  limpa(x->prox);
  delete x;
}

void ListaP::limpa() {
  limpa(inicio);
  inicio = NULL;
  fim = NULL;
}

void ListaP::limpa(Palavra *p) {
  if(p == NULL) return;
  limpa(p->prox);
  delete p;
}

//Função que transforma os caracteres em minúsculo
void transformaMinusculo(char *palavra){
    for(int i = 0; palavra[i]; i++){ //verifica cada letra
        //verifica caracteres especiais, se estiver entre À e Ü, siginica que é um caracter especial
        if(strncmp(palavra + i, "À", 2) >= 0 && strncmp(palavra + i, "Ü", 2) <= 0){
            palavra[i+1] += 32; //faz o caracter receber seu respectivo valor minúsculo.
        }
        palavra[i] = tolower(palavra[i]); //transforma para minúsculo com tolower
    }
}

int main(int argc, char* argv[]){
  //Cria um ponteiro para o arquivo de entrada
  FILE *arq = fopen(argv[1], "r");;

  //Cria a tabela de dispersão
  TabelaDispersao tDisp = TabelaDispersao();

  //Especifica um limite para a quantidade de caracteres em cada linha
  //declara as variáveis necessárias
  char line[80+2];
  char palavra[MAX+1];
  int tam = MAX+1;
  int count = 1; //count para contagem das linhas

  //Utiliza o método fgets para pegar cada linha
  while(fgets(line, sizeof(line), arq) != NULL){

    //utiliza o sscanf para ler palavra por palavra sem a leitura dos pontos no final
    for(int i=0; 1==sscanf(line + i, "%29[A-Za-zÀ-ú]%*[^A-Za-zÀ-ú]%n", palavra, &tam); i = i + tam){
      //transforma cada palavra em minúsuclo
      transformaMinusculo(palavra);

      //atribui a uma variável o tamanho da palavra
      int lenword = strlen(palavra);

      //verifica se a palavra tem um tamanho maior ou igual a 3 para inserir na tabela
      if(lenword >= 3){

        //Insere um ocorrencia passando a palavra e a linha
        tDisp.novaOcorrencia(palavra, count);
      }
    }
    //Incrementa o count para identificar a linha seguinte
    count++;
  }

  //Escreve o resultado com o fator de carga
  tDisp.escreve();
  printf("Fator de carga: %g\n", chave/47);

  //fecha o arquivo
  fclose(arq);
  return 0;
}

//ATUALIZADO DIA 13/04/2020 às 20:18