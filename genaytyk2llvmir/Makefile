CC=/usr/bin/gcc-4.8
CXX=/usr/bin/g++-4.8
# First Makefile variables used as compiler flags
LLVM_HOME=/home/fare9/Documentos/Books/LLVM-Essentials

CLANGXX=$(LLVM_HOME)/install/bin/clang++

# llvm-config will give us the necessary flags
# when we execute it with an specific flags (see below)
LLVM_CONFIG?=$(LLVM_HOME)/install/bin/llvm-config


ifndef VERBOSE
QUIET:=@
endif


SRC_DIR?=$(PWD)/src/
SRC_GENAYTYK_DIR=$(SRC_DIR)genaytyk/
SRC_DISASSEMBLER_DIR=$(SRC_DIR)disassembler/
OUT?=$(PWD)/out/
OBJ?=$(PWD)/obj-files/
COMMON_FLAGS=-g -Wall -Wextra -O0
# LDFLAGS
LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags)
# CXXFLAGS
ifndef DEBUG
CXXFLAGS+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags) -fno-rtti -fexceptions
else
CXXFLAGS_LINE+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags) -fno-rtti -fexceptions
CXXFLAGS=$(shell echo $(CXXFLAGS_LINE) | sed 's/O3/O0/')
endif
CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags) -I $(SRC_GENAYTYK_DIR) -I $(SRC_DISASSEMBLER_DIR)
# FLAGS FOR USED LIBS
LLVMLIBS=$(shell $(LLVM_CONFIG) --libs bitwriter core support)
SYSTEMLIBS=$(shell $(LLVM_CONFIG) --system-libs)

.PHONY: clean remove


BIN_NAME=genaytyk2llvmir


default: dirs $(OUT)$(BIN_NAME)

dirs:
	mkdir -p $(OBJ)
	mkdir -p $(OUT)

$(OUT)$(BIN_NAME): $(OBJ)genaytyk.o $(OBJ)genaytyk_init.o $(OBJ)genaytyk-dis.o $(OBJ)main.o
	@echo linking $@
	$(QUIET)$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ $(LLVMLIBS) $(SYSTEMLIBS)

$(OBJ)genaytyk.o: $(SRC_GENAYTYK_DIR)genaytyk.cpp
	@echo Compiling $<
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<	

$(OBJ)genaytyk_init.o: $(SRC_GENAYTYK_DIR)genaytyk_init.cpp
	@echo Compiling $<
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

$(OBJ)genaytyk-dis.o: $(SRC_DISASSEMBLER_DIR)genaytyk-dis.cpp
	@echo Compiling $<
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<	

$(OBJ)main.o: $(SRC_DIR)main.cpp
	@echo Compiling $<
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<


clean:
	$(QUIET) rm -rf $(OBJ)
	$(QUIET) rm -rf $(OUT)