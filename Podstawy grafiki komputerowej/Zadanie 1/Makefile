default: tutorial02.out

%.o: %.cpp
	g++ -c -I/Desktop/include/ -I. $<

%.out: %.o
	g++ $<  -o  $@   -lGLEW -lGL -lglfw   -L/Desktop/lib common/shader.cpp

clean:
	rm *.o *.out *~
