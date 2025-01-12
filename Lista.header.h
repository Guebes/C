#ifndef LISTA_HEADER_H_INCLUDED
#define LISTA_HEADER_H_INCLUDED


struct tipo_pessoa{

    int idade;
    char nome[50];
    float peso;
};

typedef struct tipo_pessoa tipo_pessoa; // typedef redefine a estrutura tipo_pessoa para apenas tipo_pessoa.


#endif // LISTA_HEADER_H_INCLUDED
