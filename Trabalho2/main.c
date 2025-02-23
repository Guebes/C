//Nome: Gabriel Alves Bermond
//RA: 2612143

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define ArqTam_max 200

typedef struct NO{

char arq_txt[ArqTam_max]; //texto da pasta 
struct NO *aponta_principal; //aponta para o no atual
struct NO **aponta_aq; //sub pastas ou sub arquivos
int pasta_arq; // contador de pasta e arquivo
}NO;

NO *search(NO *acessa_diretorio, const char *nome_dir);
void mkdir(NO *acessa_diretorio, const char *nome_pasta);


//1 - Criar um nó que vai apontar para o arquivo

NO *criar_no(const char *arq_nome, int past_arqu, NO *principal){ //NO *A_p (VAI APONTAR PARA O NO_PAI)

    NO *novo_no = malloc(sizeof(NO));

    strncpy(novo_no->arq_txt, arq_nome, ArqTam_max); // [strncpy] copia uma qunatidade expecifica de caracteres
    novo_no->pasta_arq = past_arqu;
    novo_no->aponta_principal = principal;
    novo_no->aponta_aq = NULL;
    novo_no->pasta_arq = 0;

    return novo_no;
}

//Criar relação entre pastas e arquivos(add um filho ao nó(arvore))

void add_subArq(NO *principal, NO *CriaPasta_Arq) {
    principal->pasta_arq++; 

    NO **temp = realloc(principal->aponta_aq, principal->pasta_arq * sizeof(NO*));
    if (temp == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    principal->aponta_aq = temp;
    principal->aponta_aq[principal->pasta_arq - 1] = CriaPasta_Arq; 
}


void LiberaMem_PastArq(NO *No){

    for(int i = 0; i < No->pasta_arq; i++){

        LiberaMem_PastArq(No->aponta_aq[i]);
    }
    free(No->aponta_aq);
    free(No);
}


NO *cd(NO *acessa_diretorio, const char *txt_diretorio){

    if(strcmp(txt_diretorio, "") == 0){

        if(acessa_diretorio->aponta_principal != NULL){

            printf("A pasta atual esta dentro de outra pasta");

            return acessa_diretorio->aponta_principal;
        
        }else if(acessa_diretorio->aponta_principal == NULL){

            printf("A pasta atual não esta dentro de outra pasta");

            return acessa_diretorio;
        }else{

            printf("Diretorio não pode ser encontrado");

            return 0;
        }
    }
}


void ler_arquivo(NO *dir_prin, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[ArqTam_max];
    while (fgets(linha, ArqTam_max, arquivo)) {
        linha[strcspn(linha, "\n")] = 0; 

        char *t = strtok(linha, "/");
        NO *atual = dir_prin;

        while (t) {
            NO *proximo = search(atual, t);

            if (proximo == NULL) {
                mkdir(atual, t);
                proximo = search(atual, t);
            }
            atual = proximo;
            t = strtok(NULL, "/");
        }
    }
    fclose(arquivo);
}


NO *search(NO *acessa_diretorio, const char *nome_dir){

    if(strcmp(acessa_diretorio->arq_txt, nome_dir) == 0) {
        return acessa_diretorio;
    }


    for(int i = 0; i < acessa_diretorio->pasta_arq; i++) {
        NO *result = search(acessa_diretorio->aponta_aq[i], nome_dir);
        if(result != NULL) {
            return result;
        }
    }

    printf("Nao foi possivel encontrar um diretorio ou arquivo com esse nome\n");
    return NULL;
}


void mkdir(NO *acessa_diretorio, const char *nome_pasta) {

    for (int i = 0; i < acessa_diretorio->pasta_arq; i++) {

        if (strcmp(acessa_diretorio->aponta_aq[i]->arq_txt, nome_pasta) == 0) {

            printf("Pasta já existe\n");

            return;
        }
    }

    NO *nova_pasta = criar_no(nome_pasta, 1, acessa_diretorio);
    add_subArq(acessa_diretorio, nova_pasta); 
    printf("Pasta '%s' criada com sucesso\n", nome_pasta);
}

void list(NO *acessa_diretorio) {

    if (acessa_diretorio->pasta_arq == 0) {

        printf("A pasta está vazia\n");

        return;
    } else {

        for (int i = 0; i < acessa_diretorio->pasta_arq; i++) {
            
            printf("%s\n", acessa_diretorio->aponta_aq[i]->arq_txt);
        }
    }
}

//Janela de ajuda

void help(){

    printf("cd : entrar em uma pasta");
    printf("search : busca uma pasta ou arquivo ");
    printf("rm : remover uma pasta ");
    printf("list : lista os componentes dentro da pasta em questão ");
    printf("mkdir : cria uma nova pasta ");
    printf("clear : limpa o contéudo da tela");
    printf("help : exibe a relação completa dos comandos ");
    printf("exit : fechar o programa");

}
//System('cls') usado para limpar o conteudo impresso do windows

void limpar_tela(){

    system("cls");
}
//remover pasta

void remover_pasta(NO *acessa_diretorio, const char *nome_pasta){

    for(int i = 0; i < acessa_diretorio->pasta_arq; i++){

        free(acessa_diretorio->aponta_aq[i]); 

        printf("Pasta removida");
    }
}

void fechar_janela(){

    printf("O programa foi fechado\n");
    exit(0);
}

#define MAX_HISTORICO 50
#define MAX_COMANDO 200
char historico[MAX_HISTORICO][MAX_COMANDO];
int indice_historico = 0;

void adcionar_historico(const char *comando) { //ELEMENTO ADICIONAL!! // Aqui você adiciona os comandos que ja pressionou manualmente

    if (indice_historico < MAX_HISTORICO) {

        strncpy(historico[indice_historico], comando, MAX_COMANDO);
        indice_historico++;

    } else {

        for (int i = 1; i < MAX_HISTORICO; i++) {

            strncpy(historico[i-1], historico[i], MAX_COMANDO);
        }

        strncpy(historico[MAX_HISTORICO - 1], comando, MAX_COMANDO);
    }
}
void imprimir_historico() { //ELEMENTO ADICIONAL!!
    printf("\nHistórico de comandos:\n");
    for (int i = 0; i < indice_historico; i++) {
        printf("%d: %s\n", i+1, historico[i]);
    }
}



int main(){

    int opcao = 0;
    NO *dir_prin = criar_no("principal", 0, NULL);  
    char nome[200];
    char nome_diretorio[200];
    char nome_pasta[200];
    char comando[MAX_COMANDO];
    
    ler_arquivo(dir_prin, "in.txt");

    printf("\nDigite uma das opcoes\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("1 : entrar em uma pasta");
    printf("\n");
    printf("2 : busca uma pasta ou arquivo");
    printf("\n");
    printf("3 : remover uma pasta");
    printf("\n");
    printf("4 : lista os componentes dentro da pasta em questão");
    printf("\n");
    printf("5 : cria uma nova pasta");
    printf("\n");
    printf("6 : limpa o contéudo da tela");
    printf("\n");
    printf("7 : exibe a relação completa dos comandos");
    printf("\n");
    printf("8 : adicionar comando ao historico"); 
    printf("\n");
    printf("9 : imprimir historico\n");
    printf("\n");
    printf("10 : fechar janela\n");
    printf("-------------------------------------------------------------------------------\n");


do {
    // Leitura da opção dentro do loop
    scanf("%d", &opcao);  

    switch (opcao) {
        case 1:
            printf("Nome do diretorio para entrar na pasta: ");
            scanf("%s", nome_diretorio);

            if (cd(dir_prin, nome_diretorio) == NULL) {
                printf("Diretório '%s' não encontrado.\n", nome_diretorio);
            } else {
                printf("Entrou no diretório '%s'.\n", nome_diretorio);
            }
            break;

        case 2:
            printf("Buscar pasta ou arquivo pelo nome: ");
            scanf("%s", nome_pasta);

            // Chama a função search
            if (search(dir_prin, nome_pasta) == NULL) {
                printf("Pasta ou arquivo '%s' não encontrado.\n", nome_pasta);
            } else {
                printf("Pasta ou arquivo '%s' encontrado.\n", nome_pasta);
            }
            break;

        case 3:
            printf("Remover pasta pelo nome: ");
            scanf("%s", nome_pasta);

            // Chama a função remover_pasta
            remover_pasta(dir_prin, nome_pasta);
            break;

        case 4:
            printf("Lista dos itens da pasta atual:\n");
            list(dir_prin);
            break;

        case 5:
            printf("Escreva o nome da pasta que deseja criar: ");
            scanf("%s", nome);

            mkdir(dir_prin, nome);
            break;

        case 6:
            printf("Limpar janela\n");
            limpar_tela();
            break;

        case 7:
            printf("Opções de comandos do cmd:\n");
            help();
            break;

            case 8:
            printf("Adicionar ao historico\n"); //ELEMENTO ADICIONAL!! // Aqui você adiciona os comandos que ja pressionou manualmente
            scanf("%s", comando);
            opcao = atoi(comando); 

            if (opcao != 10) {

            adcionar_historico(comando);
        }
            break;
        
            case 9:
            printf("Historico\n"); //ELEMENTO ADICIONAL!!
            imprimir_historico(); 
            break;

        case 10:
            fechar_janela();
            break;

        default:
            printf("Opção inválida\n");
            break;
    }

} while (opcao != 10);  

return 0;
}