OBJECTS = Aspi.o AspiCD.o CDExtract.o CDRip.o CDRomSettings.o NTScsi.o resources.o

all : CDRip.dll

CDRip.dll : $(OBJECTS)
	dllwrap -mwindows -o CDRip.dll --output-lib libcdrip.a $(OBJECTS) -L./paranoia -lparanoia -lstdc++
	strip --strip-all CDRip.dll
	upx -9 -q CDRip.dll

clean :
	rm -f CDRip.dll libcdrip.a $(OBJECTS)

Aspi.o: Aspi.cpp
	gcc -march=i586 -Os -g0 -fno-exceptions -c Aspi.cpp

AspiCD.o: AspiCD.cpp
	gcc -march=i586 -Os -g0 -fno-exceptions -DCDEX_DLL -c AspiCD.cpp

CDExtract.o: CDExtract.cpp
	gcc -march=i586 -Os -g0 -fno-exceptions -c CDExtract.cpp

CDRip.o: CDRip.cpp
	gcc -march=i586 -Os -g0 -fno-exceptions -DCDEX_DLL -c CDRip.cpp

CDRomSettings.o: CDRomSettings.cpp
	gcc -march=i586 -Os -g0 -fno-exceptions -c CDRomSettings.cpp

NTScsi.o: NTScsi.cpp
	gcc -march=i586 -Os -g0 -fno-exceptions -c NTScsi.cpp

resources.o: CDRip.rc
	windres -O coff CDRip.rc -o resources.o
