CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
USERS = rwyatt.cpp rwyatt.h snez.cpp snez.h aarcosavalos.cpp aarcosavalos.h rvelasquez.cpp rvelasquez.h 

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp $(USERS)
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp $(USERS) libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

