OBJECTS = Aspi.o AspiCD.o CDExtract.o CDRip.o CDRomSettings.o Ini.o NTScsi.o resources.o

all : CDRip.dll

CDRip.dll : $(OBJECTS)
	dllwrap -mwindows -o CDRip.dll --output-lib libcdrip.a $(OBJECTS) -L../../smooth/lib -L./paranoia -lsmooth -lparanoia -lstdc++
	strip --strip-all CDRip.dll
	upx -9 -q CDRip.dll

clean :
	rm -f CDRip.dll libcdrip.a $(OBJECTS)

Aspi.o: Aspi.cpp
	gcc -O6 -march=i586 -c Aspi.cpp

AspiCD.o: AspiCD.cpp
	gcc -O6 -march=i586 -DCDEX_DLL -c AspiCD.cpp

CDExtract.o: CDExtract.cpp
	gcc -O6 -march=i586 -c CDExtract.cpp

CDRip.o: CDRip.cpp
	gcc -O6 -march=i586 -DCDEX_DLL -c CDRip.cpp

CDRomSettings.o: CDRomSettings.cpp
	gcc -O6 -march=i586 -I../../smooth/include -c CDRomSettings.cpp

Ini.o: Ini.cpp
	gcc -O6 -march=i586 -I../../smooth/include -c Ini.cpp

NTScsi.o: NTScsi.cpp
	gcc -O6 -march=i586 -c NTScsi.cpp

resources.o: CDRip.rc
	windres -O coff CDRip.rc -o resources.o
