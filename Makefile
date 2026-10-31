CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
SOURCES = main.cpp TasScratchVector.cpp TasSTL.cpp Benchmark.cpp

tas: $(SOURCES) TasScratchVector.h TasSTL.h Benchmark.h
	$(CXX) $(CXXFLAGS) $(SOURCES) -o tas

clean:
	 rm -f tas tas.exe
