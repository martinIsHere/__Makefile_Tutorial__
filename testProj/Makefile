path_to_SDL3 := ../SDL3-3.2.26
include_dir := $(path_to_SDL3)/x86_64-w64-mingw32/include
lib_dir := $(path_to_SDL3)/x86_64-w64-mingw32/lib
#path_to_SDL3_win := ..\SDL3-3.2.26

OUT.exe: bin/obj/clear.o
	gcc bin/obj/clear.o -o bin/clear -L $(lib_dir) -l SDL3

bin/obj/clear.o: all bin/SDL3.dll
	gcc -c clear.c -o ./bin/obj/clear.o -I $(include_dir)

all: $(wildcard src/*.cpp)

# SDL3.dll:
# 	copy $(path_to_SDL3_win)\x86_64-w64-mingw32\bin\SDL3.dll .
# 	echo "copying SDL3.dll from $(path_to_SDL3_win)\x86_64-w64-mingw32\bin\ to current directory"
