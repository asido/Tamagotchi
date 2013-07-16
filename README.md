========================================================================
Tamagotchi
========================================================================

Cross platform OpenGL ES 2.0 game engine for PC and iOS written in C++.


Build
------------------------------------------------------------------------
#### Visual Studio
Requirements: CMake 2.8.8+ and Visual Studio 2012.

Run Sources\build_vs2012.bat to generate .sln and other IDE files.

The generated files will appear in Sources\msvc folder.

NOTE: You have to manually edit project properties and set working
directory or otherwise .dll files will not get loaded.

NOTE2: 3rd party .lib files are build with VS2012 and so no other
Visual Studio version can be used.


#### XCode
Requirements: CMake 2.8.8+ and XCode 4+.

TODO: CMakeFile.txt and build_xcode.sh is not yet created for XCode.

3rd party library .a files are compiled with Clang 3.2

**NOTE: the project is still *very* incomplete.**