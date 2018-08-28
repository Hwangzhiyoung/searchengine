INC_DIR :=  ./include/cppjieba ./include/network ./include/utils ./include
SRC_DIR = ./src ./src/utils ./src/network
#EXE_DIR := ./bin
CC := g++
CPPFLAGS := -std=c++11 -g -Wno-deprecated
LIBS := -ljson -lpthread
INC_FILE := $(addprefix -I, $(INC_DIR))
CPP_FILE :=  $(wildcard ./src/utils/*.cpp) $(wildcard ./src/network/*.cpp) $(wildcard ./src/*.cpp)
#OBJS_HPP :=$(wildcard  $(INC_DIR)/*.hpp) $(wildcard $(INC_DIR)/*.h)
TARGET := miniSearchEngine.exe
$(TARGET):$(CPP_FILE)
	$(CC) $(CPPFLAGS) -o $@ $(CPP_FILE) $(INC_FILE) $(LIBS)

clean:
	rm -rf $(TARGET)
