
EXE:= main
INC_DIR:= ./include ./include/cppjieba
SRC_DIR:= ./src
SRCS:= $(wildcard $(SRC_DIR)/*.cpp)
LIBS:= -lboost_regex -lpthread
CXXFLAGS:= -std=c++11 -g

$(EXE):$(SRCS)
	g++ -g -o $@ $^  $(LIBS) $(addprefix -I, $(INC_DIR)) $(CXXFLAGS)
clean:
	rm main
