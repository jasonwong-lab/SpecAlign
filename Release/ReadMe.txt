No installation is required. 

To run the Windows version, first install Visual C++ Redistributable Packages for Visual Studio 2013 (https://www.microsoft.com/en-in/download/details.aspx?id=40784) x86 version.

SpecAlign.exe should then simply execute.

If there is an error message relating to "Side by side configuration is incorrect", open Microsoft.VC90.*.manifest in a text editor and make sure that the version matches the one that is installed in Windows. The version can be checked in the Application Error log in Event Viewer.