#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char* elementos;
    int qtd;
} Pilha;

void iniciarPilha(Pilha *p){
    p->qtd = 0;
    p->elementos = (char *)malloc(p->qtd * sizeof(char));
}

void push(Pilha *p, char e){
    if(e=='#')
        return;

    p->qtd++;
    p->elementos = (char *)realloc(p->elementos, p->qtd * sizeof(char));
    p->elementos[p->qtd-1] = e;
}

char pop(Pilha *p){
    char r = 0;
    if(p->qtd == 0)
        printf("pilha vazia.\n");
    else{
        p->qtd--;
        r = p->elementos[p->qtd];
        p->elementos = (char *)realloc(p->elementos, p->qtd * sizeof(char));
    }
    return r;

}


void printPilha(Pilha p){
    if(p.qtd == 0)
        printf("[ lambda [\n");
    else{
        printf("[ ");
        printf("%c", p.elementos[0]);
        for(int i = 1; i < p.qtd; i++){
            printf(", %c", p.elementos[i]);
        }
        printf(" [\n");
    }
    return;
}
