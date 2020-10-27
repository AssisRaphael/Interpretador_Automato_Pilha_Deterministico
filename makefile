CC=gcc
OPCOES_COMPILADOR =
TODOS_ARQUIVOS_PONTOH = Pilha.h Parser.h
TODOS_ARQUIVOS_OBJ = apd.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o apd $^ $(OPCOES_COMPILADOR)

run: all
	./apd

clean:
	rm *.o apd