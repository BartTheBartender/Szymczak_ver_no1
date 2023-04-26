CXX = g++
#~ CXXFLAGS = -Wall -g
CXXFLAGS = -O3 -funsafe-math-optimizations -fno-stack-protector -ffast-math -funroll-loops -fomit-frame-pointer -march=native
output: main.o Groups.o Relations.o
	$(CXX) main.o Groups.o Relations.o -o output
	
main.o: main.cpp Groups.h Relations.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Relations.o: Relations.cpp Relations.h Groups.h
	$(CXX) $(CXXFLAGS) -c Relations.cpp
		
Groups.o: Groups.cpp Groups.h
	$(CXX) $(CXXFLAGS) -c Groups.cpp
	
clean:
	rm -f *.o output

distclean:
	rm -f *.o
