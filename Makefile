CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all:

asteroids: asteroids.cpp log.cpp timers.cpp
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp snez.cpp rwyatt.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

