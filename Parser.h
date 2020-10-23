#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ESTADOS 50
#define NUM_ALFABETO_LINGUAGEM 50
#define NUM_ALFABETO_PILHA 50
#define NUM_TRANSICOES 50

typedef struct{
    char estadoInicio;
    char estadoDestino;
    char caracter;
    char * empilhar;
    char desempilhar;
}Transicao;


typedef struct{
    char estados[NUM_ESTADOS];
    int qtdEstados;

    char alfabetoL[NUM_ALFABETO_LINGUAGEM];
    int qtdAlfabetoL;

    char alfabetoP[NUM_ALFABETO_PILHA];
    int qtdAlfabetoP;

    Transicao transicoes[NUM_TRANSICOES];
    int qtdTransicoes;

    char estadoInicial;

    char estadosFinais[NUM_ESTADOS];
    int qtdEstadosFinais;
}APD;

void iniciarAPD(APD * a){
    a->qtdEstados = a->qtdAlfabetoL = a->qtdAlfabetoP = a->qtdTransicoes = a->qtdEstadosFinais = 0;
}

char lerCaractere(FILE * arq){
    char c = fgetc(arq);
    fgetc(arq);
    return c;
}
char * lerString(FILE * arq){
    char c = fgetc(arq);
    char * s = (char *)malloc(0);
    int i;

    for(i = 0; i < 10 && c != '"'; i++){
        s = (char *)realloc(s,sizeof(char)*(i+1));
        s[i] = c;
        c = fgetc(arq);
    }
    if(i < 10)
        s[i] = '\0';
    else
        c = fgetc(arq);
    return s;
}

void lerEstados(FILE * arq, APD * apd){
    char atual = '[';
    while(atual != ']'){
        if(atual == '"'){
            atual = lerCaractere(arq);
            apd->estados[apd->qtdEstados] = atual;
            apd->qtdEstados++;
        }
        else
            atual = fgetc(arq);
    }
}
void lerAlfabetoL(FILE * arq, APD * apd){
    char atual = '[';
    while(atual != ']'){
        if(atual == '"'){
            atual = lerCaractere(arq);
            apd->alfabetoL[apd->qtdAlfabetoL] = atual;
            apd->qtdAlfabetoL++;
        }
        else
            atual = fgetc(arq);
    }
}
void lerAlfabetoP(FILE * arq, APD * apd){
    char atual = '[';
    while(atual != ']'){
        if(atual == '"'){
            atual = lerCaractere(arq);
            apd->alfabetoP[apd->qtdAlfabetoP] = atual;
            apd->qtdAlfabetoP++;
        }
        else
            atual = fgetc(arq);
    }
}
Transicao lerTransicao(FILE * arq){
    Transicao t;
    int item = 0;

    char atual = fgetc(arq);
     while(atual != ']'){
         if(atual == '"'){
             switch(item){
                case 0:
                    atual = lerCaractere(arq);
                    t.estadoInicio = atual;
                    item++;
                    break;
                case 1:
                    atual = lerCaractere(arq);
                    t.caracter = atual;
                    item++;
                    break;
                case 2:
                    atual = lerCaractere(arq);
                    t.desempilhar = atual;
                    item++;
                    break;
                case 3:
                    atual = lerCaractere(arq);
                    t.estadoDestino = atual;
                    item++;
                    break;
                case 4:
                    atual ='\0';
                    t.empilhar = lerString(arq);
                    item++;
                    break;
             }
        }
        else
            atual = fgetc(arq);
     }
     return t;
}
void lerTransicoes(FILE * arq, APD * apd){
    char atual = fgetc(arq);

    while(atual != ']'){
        if(atual == '['){
            apd->transicoes[apd->qtdTransicoes] = lerTransicao(arq);
            apd->qtdTransicoes++;
            atual = fgetc(arq);
        }
        else
            atual = fgetc(arq);
    }
    return;
}
void lerEstadosFinais(FILE * arq, APD * apd){
    char atual = '[';
    while(atual != ']'){
        if(atual == '"'){
            atual = lerCaractere(arq);
            apd->estadosFinais[apd->qtdEstadosFinais] = atual;
            apd->qtdEstadosFinais++;
        }
        else
            atual = fgetc(arq);
    }
}

APD lerAPD(FILE * arq){
    APD resultado;
    iniciarAPD(&resultado);
    int item = 0;
    //0: estado
    //1: alfabetoL
    //2: alfabetoP
    //3: delta
    //4: estados finais
    char atual;
    while(!feof(arq)){
        atual = fgetc(arq);
        if(atual == '['){
            switch(item){
                case 0:
                    lerEstados(arq,&resultado);
                    item++;
                    break;
                case 1:
                    lerAlfabetoL(arq,&resultado);
                    item++;
                    break;
                case 2:
                    lerAlfabetoP(arq,&resultado);
                    item++;
                    break;
                case 3:
                    lerTransicoes(arq,&resultado);
                    item++;
                    break;
                case 4:
                    lerEstadosFinais(arq,&resultado);
                    item++;
                    break;
            }
        }
        else if(atual == '"'){
            resultado.estadoInicial = lerCaractere(arq);
        }
    }
    return resultado;
}

void printTransicao(Transicao t){
    printf("[ %c, %c, %c, %c, %s ]\n", t.estadoInicio, t.caracter, t.desempilhar, t.estadoDestino, t.empilhar);
}

