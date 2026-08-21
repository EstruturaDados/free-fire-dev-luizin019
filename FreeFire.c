#include <stdio.h>
#include <stdlib.h>  
#include <string.h>

#define STRING_MAX 100
#define SLOTES_MAX 10


typedef struct Item {
    char nome[STRING_MAX];
    char tipo[STRING_MAX];
    int quantidade;
}Item;

typedef struct No{
    Item dados;
    struct No* proximo;//isso indica que dentro do No havera um ponteiro apontando para o no seguinte
}No;

Item Itens[SLOTES_MAX];

// Prototipação das funções utilizadas
No* criar_novo_item(char* nome, char* tipo, int quantidade, int* total_itens);
void adiciona_item_lista(No** cabeca, char* nome, char* tipo, int quantidade, int* total_itens);
No* busca_por_item(No* cabeca, char* nome, int* cont_sequencial, int* cont_binario, int* total_itens);
int busca_array_nome(Item* Itens, char* nome, int* cont_binario, int* total_itens);
void ordenar_nomes(int* total_itens);
void exibe_todos(No* cabeca);
void remover_item(No** cabeca, char* nome, int* total_itens);
void remove_item_array_nome(Item Itens[], int* total_itens, char* nome);
void libera_todos(No** cabeca, int* total_itens);
void limpar_buffer(void);


No* criar_novo_item(char* nome, char* tipo, int quantidade, int* total_itens){
    No* novoNo = (No*) malloc(sizeof(No));
// aloca memoria para novoNo com sizeof de acordo coma tipagem definida
// como cada No aponta para proximo item da lista, eles podem ficar espalhados de forma 
// individual na memoria, diferente de um Array que fica com instens agrupados lada a lado
if(novoNo == NULL){
        printf("Não há espaço na memória!\n");
        exit(1);
    }
// valida se alocação foi bem sucedida
    strcpy(novoNo->dados.nome, nome);
    strcpy(novoNo->dados.tipo, tipo); 
    novoNo->dados.quantidade = quantidade;
    novoNo->proximo = NULL;

    strcpy(Itens[*total_itens].nome, nome);
    strcpy(Itens[*total_itens].tipo, tipo);
    Itens[*total_itens].quantidade = quantidade;

    (*total_itens)++;

// passa os valores vindos como parametros para o novoNo
    return novoNo;
}
// cabeça seria como a sequencia de No atuais, como se fosse a lista
void adiciona_item_lista(No** cabeca, char* nome, char* tipo, int quantidade, int* total_itens){
    if(*total_itens >= SLOTES_MAX){
        printf("O total de espaços ja esta no limite");
        exit(1);
    }

    // cabela seria como a lista atual sendo passado como parametro
    No* novo = criar_novo_item(nome,tipo, quantidade, total_itens);
// aqui a "lista atual é passada como proximo  para novo No do item ou seja ela sera a sequencia depois do novo item"
    novo->proximo = *cabeca;
// aqui ja com novo Item ja com a lista anteria adicionada como proximo, passa a ser a "nova lista" (itemNovo+lista antiga = lista Nova)
    *cabeca = novo;

    printf("Novo item %s adicionado na mochila\n", nome);
}

// obrigatoriamente para que seja realizada busca binaria no array ele precisa ser ordenado
// no caso o atributo nome tera que estar ordenado 
// isso ja conta como uma operação na verdade até varias pois dificilmente um array sempre estara ordenado desde a origem
void ordenar_nomes(int* total_itens){
// variavel temporaria
    Item temp;
// strcmp faz -> c = c ? =>  0
// strcmp faz -> c = e ? =>  2 
// strcmp faz -> c = a ? => -2
    for(int i = 0 ; i < *total_itens - 1 ;i++){
        for(int j = 0; j < (*total_itens) - 1 - i; j++){
            if(strcmp(Itens[j].nome, Itens[j+1].nome) > 0){
                temp = Itens[j];
                Itens[j] = Itens[j+1];
                Itens[j+1] = temp;
            }
        }
    }
// o primeiro for pega um item, o segundo para cada item vindo do primeiro roda o array -i 
// comparando nome por nome sem jogando os resultado > 0 como ultimo iten do temp
}

int busca_array_nome(Item* Itens, char* nome, int* cont_binario, int* total_itens){
    int inicio = 0;
    int fim = *total_itens -1;
    *cont_binario = 0;
    
    ordenar_nomes(total_itens);

    while(inicio <= fim){
        (*cont_binario)++;
        int meio = inicio + (fim - inicio)/2;
        int comparacao = strcmp(Itens[meio].nome, nome);

        if(comparacao == 0){
            return meio;
        }

        if(comparacao < 0 ){
            inicio = meio + 1;
        } else{
            fim = meio -1;
        }
    }
    return -1;
}

No* busca_por_item(No* cabeca, char* nome, int* cont_sequencial, int* cont_binario, int* total_itens){
    No* atual = cabeca;
    *cont_sequencial = 0;
    No* encontrado = NULL;

  while(atual != NULL){
        (*cont_sequencial)++;
        if(strcmp(atual->dados.nome, nome) == 0){
            encontrado = atual; 
            break;             
        }
        atual = atual->proximo;
    }

    int array_encontrado = busca_array_nome(Itens, nome, cont_binario, total_itens);

    return encontrado;
}

void exibe_todos(No* cabeca){
    if(cabeca == NULL){
        printf("Nenhum item encontrado na lista\n");
    }

   printf("\n--- TODOS OS ITENS DA MOCHILA ---\n");
    printf("---------------------------------------------------\n");
    printf("| %-4s | %-18s | %-12s | %-5s |\n", "ID", "NOME", "TIPO", "QTD");
    printf("---------------------------------------------------\n");
    int i = 1;
    No* atual = cabeca;
// while vai percorrer toda lista
// a cada item encontrado vai printar na tela (nome e tipo)
// depois disso o atual passa a ser os priximos itens da lista, que o ponteiro "proximo" do item atual aponta
// até que No[dados, proximo = null], com isso na proxima rodagem do while o atual passa ser null

    while(atual != NULL){
        printf("| %-4d | %-18s | %-12s | %-5d |\n", 
            i,
            atual->dados.nome,
            atual->dados.tipo,
            atual->dados.quantidade);
        atual = atual->proximo;
        i++;
    }
    printf("----------------------------------------------------\n");   
}

void remove_item_array_nome(Item Itens[], int* total_itens, char* nome){
    int encontrado = -1;

    for(int i=0;i< *total_itens; i++){
// o strcmp reaulta em 0 caso ache o nome da igualde
        if(strcmp(Itens[i].nome, nome) == 0){
            encontrado = i;
            break;
        }
    }
    if(encontrado == -1){
        printf("Item %s não encontrado na mochila\n", nome);
        return;
    }

    for(int i = encontrado;i<(*total_itens) -1; i++){
        Itens[i] = Itens[i+1];
    }
    return;
}


void remover_item(No** cabeca, char* nome, int* total_itens){
    No* atual = *cabeca;
    No* anterior = NULL;
// while vai rodar a lista de ponteiros ate que encontre o ponteiro que aponte para item como nome a ser removido
// atual sera os ponteiros antes do item a ser removido
// anterior sera o item depois do ser removido
// lista [item5, proximo],[item4, proximo],[item3, proximo], remover[item2, proximo], [item1, proximo]
// while processando na x3...
// anterior = ...[item3, proximo], remover[item2, proximo], [item1, proximo]
// atual = [item2, proximo], [item1, proximo]
// depois 
// lista [item5, proximo->4],[item4, proximo->3],[item3, proximo->1], [item1, proximo=null]
// isso acontece por no if depoius do while remove cabeca do atual 
// logo a lista de ponteiro passa a ignorar o item 2
    while(atual != NULL && strcmp(atual->dados.nome, nome) != 0){
        anterior = atual;
        atual = anterior->proximo;
    }
// não há itens na lista
    if(atual == NULL){
        printf("Não há itens na lista\n");
        return;
    }
// se anterior ficou como nulo quer dizer que o primeiro i da lista é oque deve ser removido
// logo a lista passa ser apenas os proximos ponteiros
    if(anterior == NULL){
        *cabeca = atual->proximo;
// se a definiu ainda anterior dentro do while, quer dizer que a nova lista vai ser
// ponteiros antes do anterior + cabeça do anterior + proximos do atual
// quer dizer cabeça do atual deixa de existir
    } else {
        anterior->proximo = atual->proximo;
    }
    (*total_itens)--;
// limpa memoria contendo os ponteiro do item removido

    remove_item_array_nome(Itens, total_itens, nome);

    free(atual);
    printf("Item removido\n");
}

// limpeza total da memória
void libera_todos(No** cabeca, int* total_itens){
    No* atual = *cabeca;

    (*total_itens) = 0;

    while(atual != NULL){
        No* temp = atual;
        atual = atual->proximo;
        free(temp);
    }

    *cabeca = NULL;
    printf("Todos itens foram removidos\n");
}

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main (){
    No* lista_itens = NULL;
    int opcao;
    int quantidade;
    char nome[STRING_MAX], tipo[STRING_MAX];
    int total_itens = 0;
    int cont_sequencial = 0;
    int cont_binario = 0;

    do{ 
        printf("\n\n===========================================\n");
        printf("====== BEM VINDO A MOCHILA DE ITENS ======");
        printf("\n============================================\n\n");
        printf("%d/%d\n", total_itens, SLOTES_MAX);
        // adiciona no array e na lista encadeada
        printf("(1) - Adicionar itens na mochila;\n");
        printf("(2) - Listar itens da mochila;\n");
        // aqui é realiza a busca sequencial na lista encadeada e a busca binária no vetor ordenado
        printf("(3) - Ver 1 item da mochila(busca sequencial e binária);\n");
        // remove do array e da lista encadeada
        printf("(4) - Remover 1 item da mochila;\n");
        printf("(0) - Sair e limpar o mochila;\n");
        printf("Opção: \n");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao)
        {
        case 1:
            printf("Qual será nome do item?\n");
            fgets(nome, STRING_MAX, stdin );
            nome[strcspn(nome, "\n")] = '\0';

            printf("Qual será tipo do item?\n");
            fgets(tipo, STRING_MAX, stdin );
            tipo[strcspn(tipo, "\n")] = '\0';

            printf("Qual será a quantidade do item?\n");
            scanf("%d", &quantidade);
            limpar_buffer();

            adiciona_item_lista(&lista_itens, nome, tipo ,quantidade, &total_itens);
            break;

        case 2:
            exibe_todos(lista_itens);
            break;

        case 3:
            printf("Nome do item\n");
            fgets(nome,STRING_MAX , stdin);
            nome[strcspn(nome, "\n")] = '\0';

            No* encontrado = busca_por_item(lista_itens, nome, &cont_sequencial, &cont_binario, &total_itens);
            if(encontrado !=NULL){
                printf("Item encontrado:\nNome: %s \nTipo: %s \nQuantidade: %d\n",
                encontrado->dados.nome,
                encontrado->dados.tipo,
                encontrado->dados.quantidade
                );
                printf("Para encontra-lo foi necessárias:\n %d operações no vetor (busca binária)\n %d operações na lista encadeada(busca sequencial)",cont_binario,cont_sequencial);
            } else{
                printf("Item não encontrado\n");
            }
            break;

        case 4:
             printf("Nome do item a remover: ");
                fgets(nome, STRING_MAX, stdin);
                nome[strcspn(nome, "\n")] = '\0';

            remover_item(&lista_itens, nome, &total_itens);
            break;

        case 0:
            libera_todos(&lista_itens, &total_itens);
            printf("Encerrando....");
            break;

        default:
            printf("Opção inválida!\n");
            break;

        }
    } while(opcao != 0);
    return 0;
}