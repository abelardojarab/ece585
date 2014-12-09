CXX = g++
CXXFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o L3Cache.o L3Set.o L3Line.o
	$(CXX) $(CXXFLAGS) -o main main.o L3Cache.o L3Set.o L3Line.o

# The main.o target can be written more simply

main.o: main.cpp L3Cache.h L3Cache.o L3Set.o L3Line.o
	$(CXX) $(CXXFLAGS) -c main.cpp

L3Cache.o: L3Cache.cpp L3Cache.h
	$(CXX) $(CXXFLAGS) -c -o L3Cache.o L3Cache.cpp

L3Set.o: L3Set.h L3Set.cpp
	$(CXX) $(CXXFLAGS) -c -o L3Set.o L3Set.cpp

L3Line.o: L3Line.h L3Line.cpp
	$(CXX) $(CXXFLAGS) -c -o L3Line.o L3Line.cpp

clean:
	rm *.o
