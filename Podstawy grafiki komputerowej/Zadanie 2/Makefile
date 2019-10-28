CC = g++
GLFLAGS = -lGLEW -lGL -lglfw
CFLAGS = -Wall -Werror -Wextra -pedantic

#default: main.out

all: main.o point.o linesegment.o triangle.o extrafunctions.o
	$(CC) point.o linesegment.o triangle.o extrafunctions.o main.o -o main.out -lGLEW -lGL -lglfw   -L/Desktop/lib common/shader.cpp common/controls.cpp -L/home/anl/lib

point.o: point.cpp point.hpp
	$(CC) $(CFLAGS) point.cpp -c -o point.o

linesegment.o: linesegment.cpp linesegment.hpp point.cpp point.hpp
	$(CC) $(CFLAGS) linesegment.cpp -c -o linesegment.o

triangle.o: triangle.cpp triangle.hpp point.cpp point.hpp extrafunctions.cpp extrafunctions.hpp
	$(CC) $(CFLAGS) triangle.cpp -c -o triangle.o

extrafunctions.o: extrafunctions.cpp extrafunctions.hpp point.cpp point.hpp linesegment.cpp linesegment.hpp triangle.cpp triangle.hpp
	$(CC) $(CFLAGS) extrafunctions.cpp -c -o extrafunctions.o

main.o: main.cpp extrafunctions.cpp extrafunctions.hpp point.cpp point.hpp linesegment.cpp linesegment.hpp triangle.cpp triangle.hpp
	$(CC) $(GLFLAGS) main.cpp -c -o main.o -I/Desktop/include/ -I. -I/home/anl/include/

%.out: %.o
	$(CC) $^  -o  $@   -lGLEW -lGL -lglfw   -L/Desktop/lib common/shader.cpp common/controls.cpp -L/home/anl/lib

clean:
	echo "Removing unnecessary files"
	rm *.o *~
