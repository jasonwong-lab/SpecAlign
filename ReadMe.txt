# SpecAlign version 2.4

SpecAlign was initially designed for pre-processing of mass spectrometry and chromatographic data with a focus
on alignment of signals across samples that may be shifted due to minor changes in spectrometric or 
chromatographic conditions over time. 

The software and algorithms underlying specalign are described in:
https://pubmed.ncbi.nlm.nih.gov/15691857/
https://pubmed.ncbi.nlm.nih.gov/16131078/

SpecAlign has continued to find applications in a range of modern datasets. To ensure its continual availability
the source code for the orginal Windows version (compiled using VS 7.0) is are available here.

The executable can also be download under Releases.

The code for the command line linux version is available under Linux.

The Matlab code implementing RAFFT is available under Matlab.

Please note that this software has not been maintained for some years.



================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : SpecAlign Project Overview
===============================================================================

The application wizard has created this SpecAlign application for 
you.  This application not only demonstrates the basics of using the Microsoft 
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your SpecAlign application.

SpecAlign.vcproj
    This is the main project file for VC++ projects generated using an application wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    application wizard.

SpecAlign.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CSpecAlignApp application class.

SpecAlign.cpp
    This is the main application source file that contains the application
    class CSpecAlignApp.

SpecAlign.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\SpecAlign.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file SpecAlign.rc.

res\SpecAlign.rc2
    This file contains resources that are not edited by Microsoft 
    Visual C++. You should place all resources not editable by
    the resource editor in this file.
SpecAlign.reg
    This is an example .reg file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .reg
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.
/////////////////////////////////////////////////////////////////////////////

For the main frame window:
    The project includes a standard MFC interface.
MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CMDIFrameWnd and controls all MDI frame features.
res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the CMainFrame
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in SpecAlign.rc to add
    toolbar buttons.
/////////////////////////////////////////////////////////////////////////////

For the child frame window:

ChildFrm.h, ChildFrm.cpp
    These files define and implement the CChildFrame class, which
    supports the child windows in an MDI application.

/////////////////////////////////////////////////////////////////////////////

The application wizard creates one document type and one view:

SpecAlignDoc.h, SpecAlignDoc.cpp - the document
    These files contain your CSpecAlignDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CSpecAlignDoc::Serialize).
    The Document will have the following strings:
        File extension:      spa
        File type ID:        SpecAlign.Document
        Main frame caption:  SpecAlign
        Doc type name:       SpecAlign
        Filter name:         SpecAlign Files (*.spa)
        File new short name: SpecAlign
        File type long name: SpecAlign.Document
SpecAlignView.h, SpecAlignView.cpp - the view of the document
    These files contain your CSpecAlignView class.
    CSpecAlignView objects are used to view CSpecAlignDoc objects.
res\SpecAlignDoc.ico
    This is an icon file, which is used as the icon for MDI child windows
    for the CSpecAlignDoc class.  This icon is included by the main
    resource file SpecAlign.rc.
/////////////////////////////////////////////////////////////////////////////

Help Support:

hlp\SpecAlign.hhp
    This file is a help project file. It contains the data needed to
    compile the help files into a .chm file.

hlp\SpecAlign.hhc
    This file lists the contents of the help project.

hlp\SpecAlign.hhk
    This file contains an index of the help topics.

hlp\afxcore.htm
    This file contains the standard help topics for standard MFC
    commands and screen objects. Add your own help topics to this file.

hlp\afxprint.htm
    This file contains the help topics for the printing commands.

makehtmlhelp.bat
    This file is used by the build system to compile the help files.

hlp\Images\*.gif
    These are bitmap files required by the standard help file topics for
    Microsoft Foundation Class Library standard commands.

/////////////////////////////////////////////////////////////////////////////

Other Features:

ActiveX Controls
    The application includes support to use ActiveX controls.

Printing and Print Preview support
    The application wizard has generated code to handle the print, print setup, and print preview
    commands by calling member functions in the CView class from the MFC library.
/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named SpecAlign.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

SpecAlign.manifest
	Application manifest files are used by Windows XP to describe an applications 
	dependency on specific versions of Side-by-Side assemblies. The loader uses this 
	information to load the appropriate assembly from the assembly cache or private 
	from the application. The Application manifest  maybe included for redistribution 
	as an external .manifest file that is installed in the same folder as the application 
	executable or it may be included in the executable in the form of a resource. 
/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is in a 
language other than the operating system's current language, you will need 
to copy the corresponding localized resources MFC70XXX.DLL from the Microsoft
Visual C++ CD-ROM under the Win\System directory to your computer's system or 
system32 directory, and rename it to be MFCLOC.DLL.  ("XXX" stands for the 
language abbreviation.  For example, MFC70DEU.DLL contains resources 
translated to German.)  If you don't do this, some of the UI elements of 
your application will remain in the language of the operating system.

/////////////////////////////////////////////////////////////////////////////
