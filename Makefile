CFLAGS = -I ./include
##LIB    = ./libggfonts.so
NOAUDIOFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -std=c++20
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -lopenal -lsndfile -std=c++20
USERS = rwyatt.cpp rwyatt.h snez.cpp snez.h aarcosavalos.cpp aarcosavalos.h rvelasquez.cpp rvelasquez.h rgordon.cpp rgordon.h
GAMEDEPS = Graphics.cpp Graphics.h GameObjects/Game.cpp GameObjects/GameEntity.cpp

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp $(GAMEDEPS) $(USERS)
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp $(GAMEDEPS) $(USERS) libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids -DAUDIO

noaudio: asteroids.cpp log.cpp timers.cpp $(GAMEDEPS) $(USERS)
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp $(GAMEDEPS) $(USERS) libggfonts.a -Wall -Wextra $(NOAUDIOFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

