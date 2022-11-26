CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -lopenal -lsndfile #-lXrandr
USERS = rwyatt.cpp rwyatt.h snez.cpp snez.h aarcosavalos.cpp aarcosavalos.h rvelasquez.cpp rvelasquez.h rgordon.cpp rgordon.h
GAMEDEPS = GameObjects.cpp GameObjects.h Graphics.cpp Graphics.h

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp $(GAMEDEPS) $(USERS)
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp $(GAMEDEPS) $(USERS) libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

