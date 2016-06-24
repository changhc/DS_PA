CXX = g++
CXXFLAGS = -O2 -g

TARGET = pa2
SRC = PA2.cpp
OBJ = $(SRC:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<
run:
	@./pa2

clean:
	$(RM) $(OBJ) $(TARGET) 

