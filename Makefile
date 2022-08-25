CXX := g++
CXX_FLAG := -g -std=c++17

INCLUDE := ../include
LIB := ../lib
BIN := ../lib/bin

SRC := src
IMGUI_SRC := imgui_impl
IMGUI_CONFIG := $(IMGUI_SRC)\\imgui.ini
ALL_CPP := $(SRC)/*.cpp $(IMGUI_SRC)/*.cpp

DLLS := $(BIN)/*.dll
LIBRARIES := -lglad $(DLLS)
ALL_DLL := ($(subst /,\\,$(DLLS)))

BUILD_DIR := build
BUILD_SRC := (shaders objects)
EXECUTABLE := $(BUILD_DIR)\\robot
EXEC := .\\$(EXECUTABLE)


.PHONY: build
.PHONY: clean


all: build $(EXECUTABLE)
	@ echo [INFO] Executing project ...
	@ echo [START]
	@ $(EXEC)
	@ echo [END]
	@ if exist imgui.ini del imgui.ini

build: clean
	@ echo [INFO] Building project ...
	@ mkdir $(BUILD_DIR)
	@ for %%s in $(ALL_DLL); do \
		@ copy %%s $(BUILD_DIR) > NUL
	@ for %%s in $(BUILD_SRC); do \
		@ xcopy %%s $(BUILD_DIR)\\%%s /E/H/C/I/Y /EXCLUDE:except.txt > NUL
	@ copy $(IMGUI_CONFIG) . > NUL
	@ copy $(IMGUI_CONFIG) $(BUILD_DIR) > NUL

$(EXECUTABLE): $(ALL_CPP)
	@ echo [INFO] Compiling source codes ...
	@ $(CXX) $(CXX_FLAG) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@ if exist $(BUILD_DIR) rmdir /q/s $(BUILD_DIR)
	@ echo [INFO] Redundant files removed
