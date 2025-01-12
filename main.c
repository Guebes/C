#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "Lista.header.h"

int main()
{

    setlocale(LC_ALL, "Portuguese");

    tipo_pessoa lista[3];
    int i;

    for(i = 0; i < 3; i++){

        printf("Escreva os dados da %d pessoa\n", i+1);

        printf("Nome: ");
        scanf("%50[^\n]", &lista[i].nome);
        fflush(stdin); // Tempo de execução de um tipo de dado para outro, assim o teclado não vai bugar.

        printf("Idade: ");
        scanf("%d", &lista[i].idade);
        fflush(stdin);

        printf("Peso: ");
        scanf("%f", &lista[i].peso);
        fflush(stdin);

        system("cls");
    }

    for(i = 0; i < 3; i++){
        printf("\nDados da pessoa de número: %d\n", i + 1);

        printf("Nome: %s\n", lista[i].nome);
        printf("Nome: %d\n", lista[i].idade);
        printf("Nome: %1f\n", lista[i].peso);
    }

    return 0;
}
