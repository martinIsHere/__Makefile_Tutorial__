OUT.exe: main.o
	g++ main.o -o OUT # Runs first

main.o: main.cpp
	g++ -c main.cpp -o main.o # Runs second
