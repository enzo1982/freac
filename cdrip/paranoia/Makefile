OBJECTS = gap.o isort.o overlap.o p_block.o paranoia.o

all : libparanoia.a

libparanoia.a : $(OBJECTS)
	ar cru libparanoia.a $(OBJECTS)

clean :
	rm -f libparanoia.a $(OBJECTS)

gap.o: gap.c
	gcc -O6 -march=i586 -c gap.c

isort.o: isort.c
	gcc -O6 -march=i586 -c isort.c

overlap.o: overlap.c
	gcc -O6 -march=i586 -c overlap.c

p_block.o: p_block.c
	gcc -O6 -march=i586 -c p_block.c

paranoia.o: paranoia.c
	gcc -O6 -march=i586 -c paranoia.c
