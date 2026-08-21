#include <stdio.h>
#include <stdlib.h>  
#include <string.h>

#define STRING_MAX 100

typedef struct Item {
    char nome[STRING_MAX];
    char tipo[STRING_MAX];
    int quantidade;
}Item;

typedef struct No{
    Item dados;
    struct No* proximo;//isso indica que dentro do No havera um ponteiro apontando para o no seguinte
}No;

No* criar_novo_item(char* nome, char* tipo, int quantidade){
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
    strcpy(novoNo->dados.nome, tipo);
    novoNo->dados.quantidade = quantidade;
    novoNo->proximo = NULL;
// passa os valores vindos como parametros para o novoNo
    return novoNo;
}
// cabeça seria como a sequencia de No atuais, como se fosse a lista
void adiciona_item_lista(No** cabeca, char* nome, char* tipo, int quantidade){
// cabela seria como a lista atual sendo passado como parametro
    No* novo = criar_novo_item(nome,tipo, quantidade);
// aqui a "lista atual é passada como proximo  para novo No do item ou seja ela sera a sequencia depois do novo item"
    novo->proximo = *cabeca;
// aqui ja com novo Item ja com a lista anteria adicionada como proximo, passa a ser a "nova lista" (itemNovo+lista antiga = lista Nova)
    *cabeca = novo;

    printf("Novo item %s adicionado ao armazem\n", nome);
}

No* busca_por_item(No* cabeca, char* nome){
    No* atual = cabeca;

    while(atual != NULL){
// o while vai prcorrer todo a lista de itens(No) atual
// strcmp faz a comparação da string do item com o nome vindo como parametro
        if(strcmp(atual->dados.nome, nome ) == 0){
            return atual;
        }
        atual = atual->proximo;
    }
// se não haver nada cadastrado retrona null
        return NULL;
}

void exibe_todos(No* cabeca){
    if(cabeca == NULL){
        printf("Nenhum item encontrado na lista\n");
    }

    printf("\n--- TODOS OS ITENS ARMAZENADOS ---\n");
    int i = 1;
    No* atual = cabeca;
// while vai percorrer toda lista
// a cada item encontrado vai printar na tela (nome e tipo)
// depois disso o atual passa a ser os priximos itens da lista, que o ponteiro "proximo" do item atual aponta
// até que No[dados, proximo = null], com isso na proxima rodagem do while o atual passa ser null
    while(atual != NULL){
        printf("\nItem: %d,\nNome: %s \nTipo: %s \nQuantidade: %d", 
            i,
            atual->dados.nome,
            atual->dados.tipo,
            atual->dados.quantidade);
        atual = atual->proximo;
        i++;
    }
}

void remover_item(No** cabeca, char* nome){
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
// limpa memoria contendo os ponteiro do item removido
    free(atual);
    printf("Item removido\n");
}
// limpeza total da memória
void libera_todos(No** cabeca){
    No* atual = *cabeca;

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

    do{ 
        printf("\n\n===========================================\n\n");
        printf("====== BEM VINDO AO ARMAZÉM DE INTENS ======");
        printf("\n\n============================================\n\n");
        printf("(1) - Adicionar itens ao armazém;\n");
        printf("(2) - Listar itens do armazém;\n");
        printf("(3) - Ver 1 item do armazém;\n");
        printf("(4) - Excluir 1 item ao armazém;\n");
        printf("(0) - Sair e limpar o armazém;\n");
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

            adiciona_item_lista(&lista_itens, nome, tipo ,quantidade);
            break;

        case 2:
            exibe_todos(lista_itens);
            break;

        case 3:
            printf("Nome do item\n");
            fgets(nome,STRING_MAX , stdin);
            nome[strcspn(nome, "\n")] = '\0';

            No* encontrado = busca_por_item(lista_itens, nome);
            if(encontrado !=NULL){
                printf("Item encontrado:\nNome: %s \nTipo: %s \nQuantidade: %d\n",
                encontrado->dados.nome,
                encontrado->dados.tipo,
                encontrado->dados.quantidade
                );
            } else{
                printf("Item não encontrado\n");
            }
            break;

        case 4:
             printf("Nome do item a remover: ");
                fgets(nome, STRING_MAX, stdin);
                nome[strcspn(nome, "\n")] = '\0';

            remover_item(&lista_itens, nome);
            break;

        case 0:
            libera_todos(&lista_itens);
            printf("Encerrando....");
            break;

        default:
            printf("Opção inválida!\n");
            break;

        }
    } while(opcao != 0);
    return 0;
}