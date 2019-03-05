OPTIONS = -D_JUDGE_ -D_GLIBCXX_DEBUG -ansi -O2 -Wall -Wextra -Wno-uninitialized -Wno-sign-compare -std=c++0x

program.exe: program.o
	g++ -o program.exe program.o 
	rm *.o

program.o:
	g++ -c program.cc $(OPTIONS)

clean:
	rm *.o
	rm *.exe
	rm *.gch
 
