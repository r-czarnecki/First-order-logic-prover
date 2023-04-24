.PHONY: all

CC          := gcc
CFLAGS      := -c -O3 -Wall
LFLAGS      := -O3 -Wall

all: FO-prover

FO-prover: FO-prover.hs *.hs main
	ghc -o FO-prover FO-prover.hs

main: src/CNF.o src/DPLL.o src/SNF.o src/formula.o src/herbrand.o src/main.o src/parser.o src/term.o 
	$(CC) $(LFLAGS) $? -lstdc++ -o main

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm FO-prover *.hi *.o src/*.o main