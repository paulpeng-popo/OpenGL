CXX := g++
CXX_FLAG := -g -ansi -Wall -std=c++17

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

DEBUG := $(BUILD_DIR)\\texture_debug
DEBUG_EXEC := .\\$(DEBUG)

RELEASE := $(BUILD_DIR)\\texture_release


.PHONY: release
.PHONY: build
.PHONY: clean
.PHONY: help


all: build $(DEBUG)
	@ copy $(IMGUI_CONFIG) . > NUL
	@ echo [INFO] Executing project ...
	@ echo [START]
	@ $(DEBUG_EXEC)
	@ echo [END]
	@ if exist imgui.ini del imgui.ini

release: build $(RELEASE)
	@ echo [INFO] Release version done

build: clean
	@ echo [INFO] Building project ...
	@ mkdir $(BUILD_DIR)
	@ for %%s in $(ALL_DLL); do \
		@ copy %%s $(BUILD_DIR) > NUL
	@ for %%s in $(BUILD_SRC); do \
		@ xcopy %%s $(BUILD_DIR)\\%%s /E/H/C/I/Y /EXCLUDE:except.txt > NUL
	@ copy $(IMGUI_CONFIG) $(BUILD_DIR) > NUL

$(DEBUG): $(ALL_CPP)
	@ echo [INFO] Compiling source codes ...
	@ $(CXX) -DDEBUG_MODE=1 $(CXX_FLAG) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

$(RELEASE): $(ALL_CPP)
	@ echo [INFO] Compiling source codes ...
	@ $(CXX) -DDEBUG_MODE=0 $(CXX_FLAG) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@ if exist $(BUILD_DIR) rmdir /q/s $(BUILD_DIR)
	@ echo [INFO] Redundant files removed

help:
	@echo Usage: make [target] (default is all)
	@echo Available targets:
	@echo 	all:		Builds debugging target and executes it
	@echo 	release:	Builds release target
	@echo 	build:		Builds execute environment
	@echo 	clean:		Removes build directory
