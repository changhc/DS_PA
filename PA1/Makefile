CXX = g++
CXXFLAGS = -O2 -g

P2 = p2
P3 = p3
P4 = p4
GEN = gen
SRC2 = p2.cpp
SRC3 = p3.cpp
SRC4 = p4.cpp
SRC_GEN = gen.cpp
OBJ2 = $(SRC2:.cpp=.o)
OBJ3 = $(SRC3:.cpp=.o)
OBJ4 = $(SRC4:.cpp=.o)
OBJ_GEN = $(SRC_GEN:.cpp=.o)
.PHONY: all clean run

all: $(P2) $(P3) $(P4) $(GEN)

$(P2): $(OBJ2)
	$(CXX) $(LDFLAGS) -o $@ $^
$(P3): $(OBJ3)
	$(CXX) $(LDFLAGS) -o $@ $^
$(P4): $(OBJ4)
	$(CXX) $(LDFLAGS) -o $@ $^
$(GEN): $(OBJ_GEN)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<
run:
	@./gen
	@./p2

clean:
	$(RM) $(OBJ2) $(P2) 
	$(RM) $(OBJ3) $(P3)
	$(RM) $(OBJ4) $(P4)
	$(RM) $(OBJ_GEN) $(GEN)

