
output: main.o Groups.o
	g++ main.o Groups.o -o output
	
main.o: main.cpp
	g++ -c main.cpp
	
clean:
	rm *.o output
