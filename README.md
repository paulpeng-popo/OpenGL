## OpenGL environment setting with MinGW64 (tutorial)

### My devices

+ Windows 11 (64 bit)
+ Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz
+ Intel(R) UHD Graphics 630
+ Nvidia GeForce GTX 1050 Ti

### Prerequisites

#### Required

+ [MinGW64](https://github.com/niXman/mingw-builds-binaries/releases)
  + x86_64-posix-seh (if using thread)
  + x86_64-win32-seh (for general purpose)
+ [Cmake](https://cmake.org/download/)
+ [Make for Windows](http://gnuwin32.sourceforge.net/packages/make.htm)
+ [GLFW](https://www.glfw.org/download.html)
+ [GLAD](https://glad.dav1d.de/)
+ [GLM](https://github.com/g-truc/glm)

#### Optional

+ [Assimp](https://github.com/assimp/assimp)
+ [OpenMesh-8.1](https://www.graphics.rwth-aachen.de/software/openmesh/) (OpenMesh-9.0 has some bugs)
  + [QT5](https://www.qt.io/download-qt-installer)
  + [Doxygen](https://doxygen.nl/)

### Stpes

1. MinGW64
   - [ ] Unzip <span style="color:green">mingw-w64.zip</span> file and copy it into root (eg: <span style="color:red">C:\\</span>)
   - [ ] Add <span style="color:red">mingw-w64\\</span> and <span style="color:red">mingw-w64\\bin\\</span> to env path
   - [ ] Try `gcc --version` to test it for success

2. Install Cmake
   - [ ] Try `cmake --version` to test it for success
   - [ ] If command failed, try add install path into env path

3. Install Make for Windows
   - [ ] Try `make --version` to test it for success
   - [ ] Find your <span style="color:red">mingw64\\bin\\mingw32-make.exe</span>
   - [ ] Find your <span style="color:red">GnuWin32\\bin\\make.exe</span>
   - [ ] Replace make.exe with mingw32-make.exe by renaming mingw32-make.exe to make.exe
   - [ ] Try `make --version` again, you will see different informations
   - [ ] If command failed, try add install path into env path

4. Create an empty directory (I use <span style="color:red">test\\</span> for explanation)
```
test/
├── include
├── lib
│   ├── bin
│   └── libsrc
└── src
```
5. GLAD
   - [ ] Unzip <span style="color:green">glad.zip</span>
   - [ ] Copy <span style="color:red">glad\\include\\glad\\</span> into <span style="color:red">test\\include\\</span>
   - [ ] Copy <span style="color:red">glad\\include\\KHR\\</span> into <span style="color:red">test\\include\\</span>
   - [ ] Copy <span style="color:red">glad\\src\\glad.c</span> into <span style="color:red">test\\lib\\libsrc\\</span>
   - [ ] Compile <span style="color:green">glad.c</span>
        ```cmd
        > gcc glad.c -c -I .\\..\\..\\include
        > ar -rc libglad.a gload.o
        ```
   - [ ] Move libglad.a to <span style="color:red">test\\lib\\</span>

```
test/
├── include
│   ├── KHR
│   └── glad
├── lib
│   ├── bin
│   ├── libglad.a
│   └── libsrc
│       ├── glad.c
│       └── glad.o
└── src
```

6. GLFW (pre-compiled)
   - [ ] Unzip <span style="color:green">glfw.zip</span>
   - [ ] Copy <span style="color:red">glfw\\include\\GLFW\\</span> into <span style="color:red">test\\include\\</span>
   - [ ] Copy <span style="color:red">glfw\\lib-mingw-w64\\libglfw3.a</span> into <span style="color:red">test\\lib\\</span>
   - [ ] Copy <span style="color:red">glfw\\lib-mingw-w64\\libglfw3dll.a</span> into <span style="color:red">test\\lib\\</span>
   - [ ] Copy <span style="color:red">glfw\\lib-mingw-w64\\glfw3.dll</span> into <span style="color:red">test\\lib\\bin\\</span>

```
test/
├── include
│   ├── GLFW
│   ├── KHR
│   └── glad
├── lib
│   ├── bin
│   │   └── glfw3.dll
│   ├── libglad.a
│   ├── libglfw3.a
│   ├── libglfw3dll.a
│   └── libsrc
│       ├── glad.c
│       └── glad.o
└── src
```

7. GLM
   - [ ] `git clone https://github.com/g-truc/glm.git`
   - [ ] Copy <span style="color:red">glm\\glm\\</span> into <span style="color:red">test\\include\\</span>

```
test/
├── include
│   ├── GLFW
│   ├── KHR
│   ├── glad
│   └── glm
│        ├── detail
│        ├── ext
│        ├── gtc
│        ├── gtx
│        ├── simd
│        ├── glm.hpp
│        └── ...
├── lib
│   ├── bin
│   │   └── glfw3.dll
│   ├── libglad.a
│   ├── libglfw3.a
│   ├── libglfw3dll.a
│   └── libsrc
│       ├── glad.c
│       └── glad.o
└── src
```

8. ASSIMP (need to compile yourself)
   - [ ] `git clone https://github.com/assimp/assimp.git`
   - [ ] Open Cmake-GUI
     - [ ] Source: <span style="color:red">assimp\\</span>
     - [ ] Build: <span style="color:red">assimp\\build\\</span>
     - [ ] Configure
     - [ ] Generator: MinGW Makefiles
     - [ ] Specify native compilers
     - [ ] C:/mingw64/bin/gcc.exe
     - [ ] C:/mingw64/bin/g++.exe
     - [ ] C:/mingw64/bin/gfortran.exe
     - [ ] Finish
   - [ ] Enable ASSIMP_BUILD_ASSIMP_TOOLS
   - [ ] Enable BUILD_SHARED_LIBS
   - [ ] Generate
   - [ ] Open cmd (admin)
   - [ ] cmd: `cd assimp\build`
   - [ ] cmd: `make -j8`
   - [ ] cmd: `make install`
   - [ ] Copy <span style="color:red">C:\\Program Files (x86)\\Assimp\\include\\assimp\\</span> into <span style="color:red">test\\include\\</span>
   - [ ] Copy <span style="color:red">C:\\Program Files (x86)\\Assimp\\lib\\libassimp.dll.a</span> into <span style="color:red">test\\lib\\</span>
   - [ ] Copy <span style="color:red">C:\\Program Files (x86)\\Assimp\\lib\\libzlibstatic.a</span> into <span style="color:red">test\\lib\\</span>
   - [ ] Copy <span style="color:red">C:\\Program Files (x86)\\Assimp\\bin\\libassimp-5.dll</span> into <span style="color:red">test\\lib\\bin\\</span>

```
test/
├── include
│   ├── GLFW
│   ├── KHR
│   ├── assimp
│   ├── glad
│   └── glm
├── lib
│   ├── bin
│   │   ├── glfw3.dll
│   │   └── libassimp-5.dll
│   ├── libassimp.dll.a
│   ├── libzlibstatic.a
│   ├── libglad.a
│   ├── libglfw3.a
│   ├── libglfw3dll.a
│   └── libsrc
│       ├── glad.c
│       └── glad.o
└── src
```

### Congratulations

+ Now, hane fun for OpenGL programming!!
+ I have my project codes for testing
+ Makefile for using
