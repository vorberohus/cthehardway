CFLAGS=-Wall -g -std=c11
progs=ex1 ex2 ex8 ex12 ex15 ex16 ex17 ex17_1 ex18 ex19

all: $(progs)

.PHONY: clean
clean:
	rm -f $(progs)
