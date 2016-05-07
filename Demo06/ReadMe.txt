========================================================================
    CONSOLE APPLICATION : Demo06 Project Overview
========================================================================

AppWizard has created this Demo05 application for you.

This file contains a summary of what you will find in each of the files that
make up your Demo05 application.


Demo05.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

Demo05.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

Demo05.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Demo05.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

Project uses FreeGLUT and GLEW libraries that were downloaded from:
http://freeglut.sourceforge.net/
http://glew.sourceforge.net/

And copied to solution folder. In order to use them, you need to configure the
project properties as follows:

// Base macro
$(SolutionDir)

// Project Properties, Configuration Properties, General, Project Defaults, Character Set (All Config), (All Platforms)
Not Set

// Project Properties, Configuration Properties, VC++ Directories, Include Directories (All Config), (All Platforms)
$(SolutionDir)\GLEW1.13.0\include
$(SolutionDir)\FreeGLUT3.0.0-2\include

// Project Properties, Configuration Properties, C/C++, Additional Include Directories (All Config), (All Platforms)
$(SolutionDir)\GLEW1.13.0\include
$(SolutionDir)\FreeGLUT3.0.0-2\include

// Project Properties, Configuration Properties, VC++ Directories, Library Directories (All Config), (Win32)
$(SolutionDir)\FreeGLUT3.0.0-2\lib
$(SolutionDir)\GLEW1.13.0\lib\Release\Win32

// Project Properties, Configuration Properties, VC++ Directories, Library Directories (All Config), (x64)
$(SolutionDir)\FreeGLUT3.0.0-2\lib\x64
$(SolutionDir)\GLEW1.13.0\lib\Release\x64

// Post-Build events (Win32)
copy /Y "$(SolutionDir)\FreeGLUT3.0.0-2\bin\*.dll" "$(OutDir)"
copy /Y "$(SolutionDir)\GLEW1.13.0\bin\Release\Win32\*.dll" "$(OutDir)"

// Post-Build events (x64)
copy /Y "$(SolutionDir)\FreeGLUT3.0.0-2\bin\x64\*.dll" "$(OutDir)"
copy /Y "$(SolutionDir)\GLEW1.13.0\bin\Release\x64\*.dll" "$(OutDir)"
