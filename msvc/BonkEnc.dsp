# Microsoft Developer Studio Project File - Name="BonkEnc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BonkEnc - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "BonkEnc.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "BonkEnc.mak" CFG="BonkEnc - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "BonkEnc - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "BonkEnc - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BonkEnc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "BonkEnc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /Gm /GX /ZI /Od /I "../../smooth/include" /I "../include" /D "WIN32" /D "__WIN32__" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /D ID3LIB_LINKOPTION=LINKOPTION_USE_DYNAMIC /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 iolib.lib smooth.lib kernel32.lib wsock32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../../smooth/msvc/build" /libpath:"../objects"

!ENDIF 

# Begin Target

# Name "BonkEnc - Win32 Release"
# Name "BonkEnc - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "input"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE="..\src\input\filter-in-aiff.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\input\filter-in-au.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\input\filter-in-bonk.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\input\filter-in-cdrip.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\input\filter-in-lame.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\input\filter-in-voc.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\input\filter-in-vorbis.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\input\filter-in-wave.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\input\filter-in-winamp.cpp"
# End Source File
# Begin Source File

SOURCE=..\src\input\inputfilter.cpp
# End Source File
# End Group
# Begin Group "output"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE="..\src\output\filter-out-blade.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\output\filter-out-bonk.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\output\filter-out-faac.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\output\filter-out-lame.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\output\filter-out-tvq.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\output\filter-out-vorbis.cpp"
# End Source File
# Begin Source File

SOURCE="..\src\output\filter-out-wave.cpp"
# End Source File
# Begin Source File

SOURCE=..\src\output\outputfilter.cpp
# End Source File
# End Group
# Begin Group "dialogs"

# PROP Default_Filter ""
# Begin Group "genconfig"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\dialogs\genconfig\genconfig.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\genconfig\genconfig_cddb.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\genconfig\genconfig_cdrip.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\genconfig\genconfig_encoders.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\genconfig\genconfig_language.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\genconfig\genconfig_plugins.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\genconfig\genconfig_tags.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\dialogs\bladeconfig.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\bonkconfig.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\cddb_extsettings.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\cddb_multimatch.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\cddb_query.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\cddb_submit.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\faacconfig.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\lameconfig.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\language.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\tvqconfig.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dialogs\vorbisconfig.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\bonkenc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\cddb.cpp
# End Source File
# Begin Source File

SOURCE=..\src\cmdmain.cpp

!IF  "$(CFG)" == "BonkEnc - Win32 Release"

!ELSEIF  "$(CFG)" == "BonkEnc - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\config.cpp
# End Source File
# Begin Source File

SOURCE=..\src\console.cpp

!IF  "$(CFG)" == "BonkEnc - Win32 Release"

!ELSEIF  "$(CFG)" == "BonkEnc - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\debug.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dllinterfaces.cpp
# End Source File
# Begin Source File

SOURCE=..\src\encoder.cpp
# End Source File
# Begin Source File

SOURCE=..\src\joblist.cpp
# End Source File
# Begin Source File

SOURCE=..\src\main.cpp
# End Source File
# Begin Source File

SOURCE=..\src\playback.cpp
# End Source File
# Begin Source File

SOURCE=..\src\track.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "input-headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE="..\include\input\filter-in-aiff.h"
# End Source File
# Begin Source File

SOURCE="..\include\input\filter-in-au.h"
# End Source File
# Begin Source File

SOURCE="..\include\input\filter-in-bonk.h"
# End Source File
# Begin Source File

SOURCE="..\include\input\filter-in-cdrip.h"
# End Source File
# Begin Source File

SOURCE="..\include\input\filter-in-lame.h"
# End Source File
# Begin Source File

SOURCE="..\include\input\filter-in-voc.h"
# End Source File
# Begin Source File

SOURCE="..\include\input\filter-in-vorbis.h"
# End Source File
# Begin Source File

SOURCE="..\include\input\filter-in-wave.h"
# End Source File
# Begin Source File

SOURCE="..\include\input\filter-in-winamp.h"
# End Source File
# Begin Source File

SOURCE=..\include\input\inputfilter.h
# End Source File
# End Group
# Begin Group "output-headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE="..\include\output\filter-out-blade.h"
# End Source File
# Begin Source File

SOURCE="..\include\output\filter-out-bonk.h"
# End Source File
# Begin Source File

SOURCE="..\include\output\filter-out-faac.h"
# End Source File
# Begin Source File

SOURCE="..\include\output\filter-out-lame.h"
# End Source File
# Begin Source File

SOURCE="..\include\output\filter-out-tvq.h"
# End Source File
# Begin Source File

SOURCE="..\include\output\filter-out-vorbis.h"
# End Source File
# Begin Source File

SOURCE="..\include\output\filter-out-wave.h"
# End Source File
# Begin Source File

SOURCE=..\include\output\outputfilter.h
# End Source File
# End Group
# Begin Group "dialogs-headers"

# PROP Default_Filter ""
# Begin Group "genconfig-headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\dialogs\genconfig\genconfig.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\genconfig\genconfig_cddb.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\genconfig\genconfig_cdrip.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\genconfig\genconfig_encoders.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\genconfig\genconfig_language.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\genconfig\genconfig_plugins.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\genconfig\genconfig_tags.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\dialogs\bladeconfig.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\bonkconfig.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\cddb_extsettings.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\cddb_multimatch.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\cddb_query.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\cddb_submit.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\faacconfig.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\lameconfig.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\language.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\tvqconfig.h
# End Source File
# Begin Source File

SOURCE=..\include\dialogs\vorbisconfig.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\bonkenc.h
# End Source File
# Begin Source File

SOURCE=..\include\cddb.h
# End Source File
# Begin Source File

SOURCE=..\include\cmdmain.h
# End Source File
# Begin Source File

SOURCE=..\include\config.h
# End Source File
# Begin Source File

SOURCE=..\include\console.h
# End Source File
# Begin Source File

SOURCE=..\include\debug.h
# End Source File
# Begin Source File

SOURCE=..\include\dllinterfaces.h
# End Source File
# Begin Source File

SOURCE=..\include\main.h
# End Source File
# Begin Source File

SOURCE=..\include\resources.h
# End Source File
# Begin Source File

SOURCE=..\include\track.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\resources\resources\binary\bonkenc.ico
# End Source File
# Begin Source File

SOURCE=..\resources\resources.rc

!IF  "$(CFG)" == "BonkEnc - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BonkEnc - Win32 Debug"

# ADD BASE RSC /l 0x407 /i "\devel\projects\private\bonkenc\resources" /i "\devel\projects\mpsd\bonkenc\resources" /i "\devel\projects\mpsd\src\bonkenc\resources"
# ADD RSC /l 0x407 /i "\devel\projects\private\bonkenc\resources" /i "\devel\projects\mpsd\bonkenc\resources" /i "\devel\projects\mpsd\src\bonkenc\resources" /i "..\resources" /i ".."

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
