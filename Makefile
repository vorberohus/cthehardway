CFLAGS=-Wall -g
progs=ex1 ex2 ex8 ex12 ex15 ex16 ex17 ex17_1

all: $(progs)

.PHONY: clean
clean:
	rm -f $(progs)
