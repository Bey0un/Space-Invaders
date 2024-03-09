# Makefile Space Invaders

TARGET= SpaceInvaders.exe
CFLAGS=-g -W -Wall -Wextra
LDFLAGS= -lgraph -LGraphique -lSDL -lSDL_ttf
OBJETS=main.o ListeChainee/ListeChainee.o Graphique/libgraph.a 

default: $(TARGET)

.PHONY: all 

all: $(OBJETS)
	clang $(OBJETS) -o $(TARGET) $(LDFLAGS)	


ListeChainee/listeChainee.o: ListeChainee/ListeChainee.c ListeChainee/ListeChainee.h
	clang $(CFLAGS) -c ListeChainee/ListeChainee.c

main.o: main.c ListeChainee/ListeChainee.h Graphique/libgraph.h
	clang  $(CFLAGS) -c main.c

Graphique/libgraph.a:
	cd Graphique/ && $(MAKE)
	cd ..

$(TARGET): $(OBJETS)
	clang  $(OBJETS) -o $(TARGET) $(LDFLAGS)
	rm -f *.o

.PHONY: clean 

clean:
	rm -f *.o Graphique/*.[ao] ListeChainee/*.o $(TARGET)
	
tidy:
	clang-tidy --quiet -checks='*' main.c --
	clang-tidy --quiet -checks='*' ListeChainee/ListeChainee.c --

format:
	clang-format -i main.c

var:
	clang-tidy --checks=*"cppcoreguidelines-init-variables" main.c --

readability:
	clang-tidy --checks="readability-*" main.c --

