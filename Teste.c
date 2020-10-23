#include <stdio.h>
#include <stdlib.h>
#include "Pilha.h"
#include "Parser.h"

char estadoAtual;
Pilha pilha;
APD apd;

void teste1(){

    Pilha teste;
    char palavra[30];

    scanf(" %s" , palavra );
    getchar();

    iniciarPilha(&teste);
    printPilha(teste);

    for(int i = 0; i < 30 && palavra[i] != 0; i++){
        push(&teste, palavra[i]);
        printPilha(teste);
        getchar();
    }
}

void teste2(){
    Pilha p;
    APD test;
    iniciarAPD(&test);
    iniciarPilha(&p);
    FILE * entrada = fopen("C:/Users/Meu Computador/Desktop/Programas/APDs/test.txt","r");

    if(entrada == NULL){
        printf("wtf!\n");
        return;
    }
    lerEstados(entrada,&test);
    for(int i = 0; i < test.qtdEstados; i++)
        push(&p,test.estados[i]);
    printPilha(p);
    return;
}
void teste3(){
    Pilha p;
    APD test;
    char * s;
    iniciarAPD(&test);
    iniciarPilha(&p);
    FILE * entrada = fopen("C:/Users/Meu Computador/Desktop/Programas/APDs/test.txt","r");

    if(entrada == NULL){
        printf("wtf!\n");
        return;
    }
    printf("|%c|\n",fgetc(entrada));
    s = lerString(entrada);
    for(int i = 0; i < 10 && s[i] != '\0'; i++)
        push(&p,s[i]);
    printPilha(p);
    return;
}

void teste4(){
    Pilha p;
    APD test;
    Transicao t;

    iniciarAPD(&test);
    iniciarPilha(&p);
    FILE * entrada = fopen("C:/Users/Meu Computador/Desktop/Programas/APDs/test.txt","r");

    if(entrada == NULL){
        printf("wtf!\n");
        return;
    }
    printf("|%c|\n",fgetc(entrada));
    t = lerTransicao(entrada);
    printTransicao(t);
    return;
}

char * inverter_string(char * p){

    char * s;
    s = malloc((strlen(p) + 1)*sizeof(char));
    strcpy(s,p);

    int length = strlen(s);
    int c, i, j;

    for (i = 0, j = length - 1; i < j; i++, j--){
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }

    return s;
}

Transicao escolheTransicaoLambda(char estadoInicial){
    Transicao erro;
    erro.caracter = '\0';

    printf("..Transicao lambda\n");

    for(int i=0; i < apd.qtdTransicoes; i++){
        if(apd.transicoes[i].estadoInicio == estadoInicial){
            if(apd.transicoes[i].caracter == '#'){

                if(apd.transicoes[i].desempilhar == '#'){
                    return apd.transicoes[i];
                }

                if(pilha.qtd > 0){
                    char topo_pilha = pop(&pilha);
                    push(&pilha,topo_pilha);
                    if(apd.transicoes[i].desempilhar == topo_pilha){
                        return apd.transicoes[i];
                    }
                }
            }
        }
    }
    printf("+...erro\n");
    return erro;
}


Transicao escolheTransicaoNormal(char estadoInicial, char letra){
    Transicao erro;
    erro.caracter = '\0';

    printf("..Transicao normal\n");

    for(int i=0; i < apd.qtdTransicoes; i++){
        if(apd.transicoes[i].estadoInicio == estadoInicial){
            if(apd.transicoes[i].caracter == letra){

                if(apd.transicoes[i].desempilhar == '#'){
                    return apd.transicoes[i];
                }

                if(pilha.qtd > 0){
                    char topo_pilha = pop(&pilha);
                    push(&pilha,topo_pilha);
                    if(apd.transicoes[i].desempilhar == topo_pilha){
                        return apd.transicoes[i];
                    }
                }
            }
        }
    }
    printf("+...erro\n");
    return erro; //Nenhuma transição possivel
}




void TestarPalavra(char * palavra){

    char estadoAtual = apd.estadoInicial;
    Transicao transicao;
    char * caracteres_empilhados;

    for(int i = 0; palavra[i] != '\0' ;i++){//Percorre a palavra

        printf("----- i: %d ------\n",i);

        transicao = escolheTransicaoNormal(estadoAtual, palavra[i]); //Escolhe a transição do estado a partir da letra da palavra

        printf("_____________________________\n");

        printf("estadoInicial[%d]: %c\n",i,transicao.estadoInicio);
        printf("estadoDestino[%d]: %c\n",i,transicao.estadoDestino);
        printf("caracter[%d]: %c\n",i,transicao.caracter);
        printf("desempilhar[%d]: %c\n",i,transicao.desempilhar);
        printf("empilhar[%d]: %s\n\n",i,transicao.empilhar);

        if(transicao.caracter != '\0'){//Transição encontrada

            estadoAtual = transicao.estadoDestino;// Atualiza o estado da apd

            if(transicao.desempilhar != '#'){ //Desempilha pilha
                if(pilha.qtd > 0)
                    pop(&pilha);
                else{
                    printf("Nao - retirando da pilha vazia\n");
                    return;
                }
            }

            caracteres_empilhados = inverter_string(transicao.empilhar);

            for(int j = 0; j<strlen(caracteres_empilhados);j++){ //Empilha pilha
                push(&pilha,caracteres_empilhados[j]);
            }

            free(caracteres_empilhados);
        }
        else{//Transição normal não existente

            transicao = escolheTransicaoLambda(estadoAtual);

            printf(".estadoInicial[%d]: %c\n",i,transicao.estadoInicio);
            printf(".estadoDestino[%d]: %c\n",i,transicao.estadoDestino);
            printf(".caracter[%d]: %c\n",i,transicao.caracter);
            printf(".desempilhar[%d]: %c\n",i,transicao.desempilhar);
            printf(".empilhar[%d]: %s\n\n",i,transicao.empilhar);


            if(transicao.caracter == '\0'){
                for(int k = 0; k<strlen(apd.estadosFinais);k++){
                    if(apd.estadosFinais[k] == estadoAtual){
                        if(pilha.qtd == 0 && palavra[k] == '#'){
                            printf("Sim\n");
                            return;
                        }
                    }
                }
                printf("Nao - transicao inexistente\n");
                return;
            }

            estadoAtual = transicao.estadoDestino;// Atualiza o estado da apd

            if(transicao.desempilhar != '#'){ //Desempilha pilha
                if(pilha.qtd > 0)
                    pop(&pilha);
                else{
                    printf("Nao - retirando da pilha vazia\n");
                    return;
                }
            }

            caracteres_empilhados = inverter_string(transicao.empilhar);

            for(int j = 0; j<strlen(caracteres_empilhados);j++){ //Empilha pilha
                push(&pilha,caracteres_empilhados[j]);
            }

            free(caracteres_empilhados);

            i--;
        }
        char topo_pilha = pop(&pilha);
        push(&pilha,topo_pilha);
        printf("Topo da pilha = %c\n",topo_pilha);
    }//fim for

    for(int i = 0; i<strlen(apd.estadosFinais);i++){
        if(apd.estadosFinais[i] == estadoAtual){
            if(pilha.qtd == 0){
                printf("Sim\n");
                return;
            }
        }
    }

    do{
        printf(".....do while\n");
        transicao = escolheTransicaoLambda(estadoAtual);

        printf(".estadoInicial[]: %c\n",transicao.estadoInicio);
        printf(".estadoDestino[]: %c\n",transicao.estadoDestino);
        printf(".caracter[]: %c\n",transicao.caracter);
        printf(".desempilhar[]: %c\n",transicao.desempilhar);
        printf(".empilhar[]: %s\n\n",transicao.empilhar);


        if(transicao.caracter == '\0'){
            char topo_pilha = pop(&pilha);
            printf("Topo da pilha = %c\n",topo_pilha);
            break;
        }


        estadoAtual = transicao.estadoDestino;// Atualiza o estado da apd

        if(transicao.desempilhar != '#'){ //Desempilha pilha
            if(pilha.qtd > 0)
                pop(&pilha);
            else{
                printf("Nao - retirando da pilha vazia\n");
                return;
            }
        }

        caracteres_empilhados = inverter_string(transicao.empilhar);

        for(int j = 0; j<strlen(caracteres_empilhados);j++){ //Empilha pilha
            push(&pilha,caracteres_empilhados[j]);
        }

        free(caracteres_empilhados);

    } while(transicao.caracter != '\0');



    for(int i = 0; i < strlen(apd.estadosFinais); i++){
        if(apd.estadosFinais[i] == estadoAtual){
            if(pilha.qtd == 0){
                printf("Sim\n");
                return;
            }
        }
    }

    printf("Nao - nenhuma das anteriores @@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    return;
}








int main(){

    char palavra[100];

    iniciarAPD(&apd);
    iniciarPilha(&pilha);
    FILE * entrada = fopen("test.txt","r");

    if(entrada == NULL){
        printf("Arquivo nao encontrado\n");
        return 0;
    }
    apd = lerAPD(entrada);

    /*printf("Estrutura:\n");
    printf("estados: %s\n",apd.estados);
    printf("estados finais: %s\n",apd.estadosFinais);
    printf("alfabetoL: %s\n",apd.alfabetoL);
    printf("alfabetoP: %s\n",apd.alfabetoP);
    printf("estadoInicial: %c\n",apd.estadoInicial);

    printf("qtdEstados: %d\n",apd.qtdEstados);
    printf("qtdAlfabetoL: %d\n",apd.qtdAlfabetoL);
    printf("qtdAlfabetoP: %d\n",apd.qtdAlfabetoP);
    printf("qtdTransicoes: %d\n",apd.qtdTransicoes);

    printf("\nTransicoes:\n");

    for(int i = 0; i < apd.qtdTransicoes; i++){
        printf("estadoInicial[%d]: %c\n",i,apd.transicoes[i].estadoInicio);
        printf("estadoDestino[%d]: %c\n",i,apd.transicoes[i].estadoDestino);
        printf("caracter[%d]: %c\n",i,apd.transicoes[i].caracter);
        printf("desempilhar[%d]: %c\n",i,apd.transicoes[i].desempilhar);
        printf("empilhar[%d]: %s\n\n",i,apd.transicoes[i].empilhar);
    }*/



    /*printf("[ %c", apd.estados[0]);
    for(int i = 1; i < apd.qtdEstados; i++)
        printf(", %c", apd.estados[i]);
    printf("]\n");

    printf("[ %c", apd.alfabetoL[0]);
    for(int i = 1; i < apd.qtdAlfabetoL; i++)
        printf(", %c", apd.alfabetoL[i]);
    printf("]\n");

    printf("[ %c", apd.alfabetoP[0]);
    for(int i = 1; i < apd.qtdAlfabetoP; i++)
        printf(", %c", apd.alfabetoP[i]);
    printf("]\n");

    for(int i = 0; i < apd.qtdTransicoes; i++){
        printTransicao(apd.transicoes[i]);
    }

    printf("estado inicial: %c\n", apd.estadoInicial);

    printf("[ %c", apd.estadosFinais[0]);
    for(int i = 1; i < apd.qtdEstadosFinais; i++)
        printf(", %c", apd.estadosFinais[i]);
    printf("]\n");*/

    while(1){
        printf("palavra: ");
        scanf(" %s", palavra);
        TestarPalavra(palavra);

        iniciarPilha(&pilha);

        printf("############################################################\n");
    }
}


