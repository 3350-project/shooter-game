CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
USERS = rwyatt.cpp snez.cpp rgordon.cpp axel.cpp

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp $(USERS)
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp $(USERS) libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

