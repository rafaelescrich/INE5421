#Configurações do compilador
COMPILER = g++
FLAGS = -std=c++0x -Wall -pedantic -Wextra -ggdb
LIBS := -I./

DFLAGS =
ifndef NO_CONCEPT
	DFLAGS += -D_GLIBCXX_CONCEPT_CHECKS
endif
# DFLAGS - Define flags
# Sempre que a variável NO_CONCEPT estiver definida, não adicionaremos a flag
# 	-D_GLIBCXX_CONCEPT_CHECKS
# Para definir a variável NO_CONCEPT, basta invocar
# 	make NO_CONCEPT=1

#Lista de object files e dependências

SOURCES = $(wildcard *.cpp */*.cpp */*/*.cpp)
#A função wildcard força a expansão dos * até dois níveis de subdiretório. \
	TODO: achar algum jeito de fazer isso sem usar esta gambiarra.

OBJ = $(SOURCES:.cpp=.o)
#Substitui .cpp para .o no fim das palavras

OBJDEPS = $(SOURCES:.cpp=.d)

#Agora, temos: \
	SOURCES contém a lista de todos os .cpp do diretório. \
	OBJ contém a mesma lista, mas com .o no final. \
	OBJDEPS contém a mesma lista, mas com .d no final. \
\
A ideia é usar os .d como alvos do g++ -MM, que dá a lista de \
dependências do .cpp em questão. Então incluímos todos os .d \
no final.

a.out: $(OBJ)
	$(COMPILER) $(FLAGS) $^
# $^ retorna todas as dependências, sem repetição.

#"Metarregra": para cada palavra em OBJDEPS que case com %.o, defina \
	a regra %.d : %.cpp \
			g++ -MM -MF $@ $<

$(OBJDEPS): %.d : %.cpp
	g++ -std=c++0x -MM $< -MF $@ -MT "$*.o $*.d" $(LIBS)
#Explicação: \
$@ retorna o target \
$< retorna a primeira dependência \
$* retorna o que quer que tenha casado com o % \
Digamos que o arquivo alce.d esteja em OBJDEPS. alce.d casará com \
%.d, ficando % == alce. A regra criada é: \
	alce.d : alce.cpp \
Cado tenha ocorrido alguma modificação em alce.cpp, reconstruímos \
alce.d. A linha executada pega todas as dependências de alce.cpp \
(-MM $<) e salva-as no arquivo alce.d (-MF $@). Entretanto, usaremos \
um target diferente: queremos que o alvo seja "alce.d alce.cpp", \
para que, ao incluírmos alce.d, todas as dependências de alce.cpp \
também provoquem a recriação de alce.d. \
No final, as regras assumem esta forma: \
alce.d alce.cpp : dependencias

$(OBJ): %.o : %.cpp Makefile
	$(COMPILER) $(FLAGS) $(DFLAGS) $(LIBS) -c $< -o $@

include $(OBJDEPS)

.PHONY: clean

clean:
	-rm $(OBJ) $(OBJDEPS)
