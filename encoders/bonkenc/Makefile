OBJECTS = api.o codec.o utility.o

all : Bonk.dll

Bonk.dll : $(OBJECTS)
	gcc -mwindows --shared -o Bonk.dll $(OBJECTS) -L../../../smooth/lib -lsmooth -lstdc++
	strip --strip-all Bonk.dll
	upx -9 -q Bonk.dll

clean :
	rm -f Bonk.dll libbonk.a $(OBJECTS)

api.o: api.cpp
	gcc -O6 -march=i586 -I../../../smooth/include -c api.cpp

codec.o: codec.cpp
	gcc -O6 -march=i586 -I../../../smooth/include -c codec.cpp

utility.o: utility.cpp
	gcc -O6 -march=i586 -I../../../smooth/include -c utility.cpp
