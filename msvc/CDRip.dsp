# Microsoft Developer Studio Project File - Name="CDRip" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CDRip - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "CDRip.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "CDRip.mak" CFG="CDRip - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "CDRip - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CDRip - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CDRip - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CDRIP_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CDRIP_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "CDRip - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CDRip___Win32_Debug"
# PROP BASE Intermediate_Dir "CDRip___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CDRip___Win32_Debug"
# PROP Intermediate_Dir "CDRip___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CDRIP_EXPORTS" /YX /FD /GZ  /c
# ADD CPP /nologo /MTd /W1 /Gm /GX /ZI /Od /I "../../smooth/include" /D "WIN32" /D "__WIN32__" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CDRIP_EXPORTS" /YX /FD /GZ  /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 smooth.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"Debug/CDRip.dll" /pdbtype:sept /libpath:"../../smooth/msvc/Build"

!ENDIF 

# Begin Target

# Name "CDRip - Win32 Release"
# Name "CDRip - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "paranoia"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\cdrip\paranoia\gap.c
# End Source File
# Begin Source File

SOURCE=..\cdrip\paranoia\isort.c
# End Source File
# Begin Source File

SOURCE=..\cdrip\paranoia\overlap.c
# End Source File
# Begin Source File

SOURCE=..\cdrip\paranoia\p_block.c
# End Source File
# Begin Source File

SOURCE=..\cdrip\paranoia\paranoia.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\cdrip\Aspi.cpp
# End Source File
# Begin Source File

SOURCE=..\cdrip\AspiCD.cpp

!IF  "$(CFG)" == "CDRip - Win32 Release"

!ELSEIF  "$(CFG)" == "CDRip - Win32 Debug"

# ADD CPP /D "CDEX_DLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\cdrip\CDExtract.cpp
# End Source File
# Begin Source File

SOURCE=..\cdrip\CDRip.cpp

!IF  "$(CFG)" == "CDRip - Win32 Release"

!ELSEIF  "$(CFG)" == "CDRip - Win32 Debug"

# ADD CPP /D "CDEX_DLL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\cdrip\CDRomSettings.cpp
# End Source File
# Begin Source File

SOURCE=..\cdrip\Ini.cpp
# End Source File
# Begin Source File

SOURCE=..\cdrip\NTScsi.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "paranoia_headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\cdrip\paranoia\cdda_paranoia.h
# End Source File
# Begin Source File

SOURCE=..\cdrip\paranoia\gap.h
# End Source File
# Begin Source File

SOURCE=..\cdrip\paranoia\isort.h
# End Source File
# Begin Source File

SOURCE=..\cdrip\paranoia\overlap.h
# End Source File
# Begin Source File

SOURCE=..\cdrip\paranoia\p_block.h
# End Source File
# End Group
# Begin Group "interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\cdrip\interface\cdda_interface.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\cdrip\Aspi.h
# End Source File
# Begin Source File

SOURCE=..\cdrip\AspiCD.h
# End Source File
# Begin Source File

SOURCE=..\cdrip\CDExtract.h
# End Source File
# Begin Source File

SOURCE=..\cdrip\CDRip.h
# End Source File
# Begin Source File

SOURCE=..\cdrip\CDRomSettings.h
# End Source File
# Begin Source File

SOURCE=..\cdrip\Ini.h
# End Source File
# Begin Source File

SOURCE=..\cdrip\NTScsi.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\cdrip\CDRip.rc
# End Source File
# End Group
# End Target
# End Project
