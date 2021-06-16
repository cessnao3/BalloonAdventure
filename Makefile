CXX=g++
CXXFLAGS=$(file < BalloonAdventure.cxxflags)
CXXLIBS = $(shell pkg-config --libs allegro-5 allegro_primitives-5 allegro_ttf-5 allegro_font-5)

ALLFILES=$(file < BalloonAdventure.files)

SRCDIRS=game_objects

SRC=$(foreach folder,$(SRCDIRS),$(wildcard src/$(folder)/*.cpp))
HDR=$(foreach folder,$(SRCDIRS),$(wildcard src/$(folder)/*.h))

MAIN=src/main.cpp
EXEC=balloon.out

OBJS=$(patsubst %.cpp,%.o,$(SRC))

list:
	echo "$(ALLFILES)"

all: $(EXEC)

release: CXXFLAGS+=-O2
release: all

debug: CXXFLAGS+=-g
debug: all

$(EXEC): $(OBJS) $(MAIN)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(MAIN) $(CXXLIBS)

%.o: %.cpp $(HDR)
	$(CXX) -o $@ $(CXXFLAGS) -c $< $(CXXLIBS)

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
