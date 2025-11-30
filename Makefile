# simple script for compiling .cpp files to .o to .exe

OUT.exe: $(patsubst %.cpp, %.o, $(wildcard *.cpp))
	echo "$@" # current target
	echo "$^" # current prereqs
	g++ $^ -o OUT

#targets: specific target: prereqs
$(patsubst %.cpp, %.o, $(wildcard *.cpp)): %.o : %.cpp
	echo "--------"
	echo "$@" # current target
	echo "$^" # current prereqs
	g++ -c $^ -o $@
