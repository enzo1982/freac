OBJECTS = api.o codec.o utility.o

all : BonkEnc.dll

BonkEnc.dll : $(OBJECTS)
	dllwrap -mwindows -o BonkEnc.dll --output-lib libbonkenc.a $(OBJECTS) -L../../../smooth/lib -liolib -lwsock32
	strip --strip-all BonkEnc.dll
	upx -9 -q BonkEnc.dll

clean :
	rm -f BonkEnc.dll libbonkenc.a $(OBJECTS)

api.o: api.cpp
	gcc -O6 -march=i586 -I../../../smooth/include -c api.cpp

codec.o: codec.cpp
	gcc -O6 -march=i586 -I../../../smooth/include -c codec.cpp

utility.o: utility.cpp
	gcc -O6 -march=i586 -I../../../smooth/include -c utility.cpp
