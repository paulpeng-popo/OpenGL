CXX := g++
CXX_FLAG := -g -std=c++17

INCLUDE := ../include
LIB := ../lib

SRC := src
IMGUI_SRC := imgui_impl

ALL_CPP := $(SRC)/*.cpp $(IMGUI_SRC)/*.cpp

LIBRARIES := -lglad ../lib/bin/glfw3.dll ../lib/bin/libassimp-5.dll
OUT_DIR := build
EXECUTABLE := $(OUT_DIR)/robot

ALL_DLL := (..\\lib\\bin\\glfw3.dll ..\\lib\\bin\\libassimp-5.dll)

MKDIR_BUILD := mkdir $(OUT_DIR)
COPY_SRC := Xcopy $(SRC) $(OUT_DIR)\\$(SRC) /E/H/C/I > NUL
EXEC := .\\$(EXECUTABLE)

.PHONY: build
.PHONY: clean

all: clean build $(EXECUTABLE)
	@ for %%s in $(ALL_DLL); do \
		@ copy %%s $(OUT_DIR) > NUL;
	@ echo [INFO] Executing project ...
	@ echo [START]
	@ $(EXEC)
	@ echo [END]

build:
	@ $(MKDIR_BUILD)
	@ $(COPY_SRC)

$(EXECUTABLE): $(ALL_CPP)
	@ echo [INFO] Compiling source codes ...
	@ $(CXX) $(CXX_FLAG) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@ if exist $(OUT_DIR) rmdir /q/s $(OUT_DIR)
	@ echo [INFO] Redundant files removed
