# Microsoft Developer Studio Project File - Name="algorithms" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=algorithms - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "algorithms.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "algorithms.mak" CFG="algorithms - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "algorithms - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "algorithms - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "algorithms - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "algorithms - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /Zp1 /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "algorithms - Win32 Release"
# Name "algorithms - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Ag.c
# End Source File
# Begin Source File

SOURCE=.\AlbBcg.c
# End Source File
# Begin Source File

SOURCE=.\AlbBcp.c
# End Source File
# Begin Source File

SOURCE=.\Algorithms.c
# End Source File
# Begin Source File

SOURCE=.\Alp.c
# End Source File
# Begin Source File

SOURCE=.\Alt.c
# End Source File
# Begin Source File

SOURCE=.\Amy.c
# End Source File
# Begin Source File

SOURCE=.\Ast.c
# End Source File
# Begin Source File

SOURCE=.\Ba1.c
# End Source File
# Begin Source File

SOURCE=.\Ba2.c
# End Source File
# Begin Source File

SOURCE=.\BeadCheck1.c
# End Source File
# Begin Source File

SOURCE=.\BeadCheck2.c
# End Source File
# Begin Source File

SOURCE=.\Bun.c
# End Source File
# Begin Source File

SOURCE=.\Ca.c
# End Source File
# Begin Source File

SOURCE=.\Chol.c
# End Source File
# Begin Source File

SOURCE=.\Ck.c
# End Source File
# Begin Source File

SOURCE=.\Cl1.c
# End Source File
# Begin Source File

SOURCE=.\Cl2.c
# End Source File
# Begin Source File

SOURCE=.\CommonAlgFuncs.c
# End Source File
# Begin Source File

SOURCE=.\Cre.c
# End Source File
# Begin Source File

SOURCE=.\DacTrims.c
# End Source File
# Begin Source File

SOURCE=.\Dbil.c
# End Source File
# Begin Source File

SOURCE=.\DistributionCheck.c
# End Source File
# Begin Source File

SOURCE=.\Ggt.c
# End Source File
# Begin Source File

SOURCE=.\Glob.c
# End Source File
# Begin Source File

SOURCE=.\Glu.c
# End Source File
# Begin Source File

SOURCE=.\Hdl1.c
# End Source File
# Begin Source File

SOURCE=.\Hdl2.c
# End Source File
# Begin Source File

SOURCE=.\Ibil.c
# End Source File
# Begin Source File

SOURCE=.\Iqc.c
# End Source File
# Begin Source File

SOURCE=.\K.c
# End Source File
# Begin Source File

SOURCE=.\Ldh.c
# End Source File
# Begin Source File

SOURCE=.\Ldl.c
# End Source File
# Begin Source File

SOURCE=.\Mg.c
# End Source File
# Begin Source File

SOURCE=.\Na1.c
# End Source File
# Begin Source File

SOURCE=.\Na2.c
# End Source File
# Begin Source File

SOURCE=.\Offsets.c
# End Source File
# Begin Source File

SOURCE=.\Phos.c
# End Source File
# Begin Source File

SOURCE=.\RotorResults.c
# End Source File
# Begin Source File

SOURCE=.\Rqc.c
# End Source File
# Begin Source File

SOURCE=.\SampleBlank.c
# End Source File
# Begin Source File

SOURCE=.\SharedBarcode.c
# End Source File
# Begin Source File

SOURCE=.\SharedCal.c
# End Source File
# Begin Source File

SOURCE=.\SharedFactory.c
# End Source File
# Begin Source File

SOURCE=.\SharedRaw.c
# End Source File
# Begin Source File

SOURCE=.\SharedRoc.c
# End Source File
# Begin Source File

SOURCE=.\Statistics.c
# End Source File
# Begin Source File

SOURCE=.\SystemCheck.c
# End Source File
# Begin Source File

SOURCE=.\T4.c
# End Source File
# Begin Source File

SOURCE=.\Tbil.c
# End Source File
# Begin Source File

SOURCE=.\Tch.c
# End Source File
# Begin Source File

SOURCE=.\Tco2.c
# End Source File
# Begin Source File

SOURCE=.\Temperature.c
# End Source File
# Begin Source File

SOURCE=.\Tp.c
# End Source File
# Begin Source File

SOURCE=.\Trig.c
# End Source File
# Begin Source File

SOURCE=.\Ua.c
# End Source File
# Begin Source File

SOURCE=.\Vldl.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Ag.h
# End Source File
# Begin Source File

SOURCE=.\AlbBcg.h
# End Source File
# Begin Source File

SOURCE=.\AlbBcp.h
# End Source File
# Begin Source File

SOURCE=.\AlgGlobals.h
# End Source File
# Begin Source File

SOURCE=.\Algorithms.h
# End Source File
# Begin Source File

SOURCE=.\AlgResult.h
# End Source File
# Begin Source File

SOURCE=.\Alp.h
# End Source File
# Begin Source File

SOURCE=.\Alt.h
# End Source File
# Begin Source File

SOURCE=.\Amy.h
# End Source File
# Begin Source File

SOURCE=.\Ast.h
# End Source File
# Begin Source File

SOURCE=.\Ba1.h
# End Source File
# Begin Source File

SOURCE=.\Ba2.h
# End Source File
# Begin Source File

SOURCE=.\BarcodeData.h
# End Source File
# Begin Source File

SOURCE=.\BeadCheck1.h
# End Source File
# Begin Source File

SOURCE=.\BeadCheck2.h
# End Source File
# Begin Source File

SOURCE=.\Bun.h
# End Source File
# Begin Source File

SOURCE=.\Ca.h
# End Source File
# Begin Source File

SOURCE=.\CalData.h
# End Source File
# Begin Source File

SOURCE=.\Chol.h
# End Source File
# Begin Source File

SOURCE=.\Ck.h
# End Source File
# Begin Source File

SOURCE=.\Cl1.h
# End Source File
# Begin Source File

SOURCE=.\Cl2.h
# End Source File
# Begin Source File

SOURCE=.\CommonAlgFuncs.h
# End Source File
# Begin Source File

SOURCE=.\Cre.h
# End Source File
# Begin Source File

SOURCE=.\DacTrims.h
# End Source File
# Begin Source File

SOURCE=.\Dbil.h
# End Source File
# Begin Source File

SOURCE=.\DistributionCheck.h
# End Source File
# Begin Source File

SOURCE=.\EndpointData.h
# End Source File
# Begin Source File

SOURCE=.\FactoryData.h
# End Source File
# Begin Source File

SOURCE=.\Ggt.h
# End Source File
# Begin Source File

SOURCE=.\Glob.h
# End Source File
# Begin Source File

SOURCE=.\Glu.h
# End Source File
# Begin Source File

SOURCE=.\Hdl1.h
# End Source File
# Begin Source File

SOURCE=.\Hdl2.h
# End Source File
# Begin Source File

SOURCE=.\Ibil.h
# End Source File
# Begin Source File

SOURCE=.\Iqc.h
# End Source File
# Begin Source File

SOURCE=.\IqcData.h
# End Source File
# Begin Source File

SOURCE=.\K.h
# End Source File
# Begin Source File

SOURCE=.\Ldh.h
# End Source File
# Begin Source File

SOURCE=.\Ldl.h
# End Source File
# Begin Source File

SOURCE=.\Mg.h
# End Source File
# Begin Source File

SOURCE=.\Na1.h
# End Source File
# Begin Source File

SOURCE=.\Na2.h
# End Source File
# Begin Source File

SOURCE=.\Offsets.h
# End Source File
# Begin Source File

SOURCE=.\Phos.h
# End Source File
# Begin Source File

SOURCE=.\RadData.h
# End Source File
# Begin Source File

SOURCE=.\RateData.h
# End Source File
# Begin Source File

SOURCE=.\RawData.h
# End Source File
# Begin Source File

SOURCE=.\ResultsData.h
# End Source File
# Begin Source File

SOURCE=.\RocData.h
# End Source File
# Begin Source File

SOURCE=.\RotorResults.h
# End Source File
# Begin Source File

SOURCE=.\Rqc.h
# End Source File
# Begin Source File

SOURCE=.\RqcData.h
# End Source File
# Begin Source File

SOURCE=.\SampleBlank.h
# End Source File
# Begin Source File

SOURCE=.\SampleBlankData.h
# End Source File
# Begin Source File

SOURCE=.\SharedBarcode.h
# End Source File
# Begin Source File

SOURCE=.\SharedCal.h
# End Source File
# Begin Source File

SOURCE=.\SharedFactory.h
# End Source File
# Begin Source File

SOURCE=.\SharedRaw.h
# End Source File
# Begin Source File

SOURCE=.\SharedRoc.h
# End Source File
# Begin Source File

SOURCE=.\Statistics.h
# End Source File
# Begin Source File

SOURCE=.\SystemCheck.h
# End Source File
# Begin Source File

SOURCE=.\SystemCheckData.h
# End Source File
# Begin Source File

SOURCE=.\T4.h
# End Source File
# Begin Source File

SOURCE=.\Tbil.h
# End Source File
# Begin Source File

SOURCE=.\Tch.h
# End Source File
# Begin Source File

SOURCE=.\Tco2.h
# End Source File
# Begin Source File

SOURCE=.\Temperature.h
# End Source File
# Begin Source File

SOURCE=.\Tp.h
# End Source File
# Begin Source File

SOURCE=.\Trig.h
# End Source File
# Begin Source File

SOURCE=.\Ua.h
# End Source File
# Begin Source File

SOURCE=.\Vldl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
