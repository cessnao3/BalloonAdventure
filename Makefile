CXX=g++
CXXFLAGS=-Wall -Werror -pedantic -Isrc -Ilib
CXXLIBS = $(shell pkg-config --libs allegro-5 allegro_primitives-5 allegro_ttf-5 allegro_font-5)

SRCDIRS=src lib/gamelib

SRC=$(foreach folder,$(SRCDIRS),$(wildcard $(folder)/*.cpp))
HDR=$(foreach folder,$(SRCDIRS),$(wildcard $(folder)/*.h))

EXEC=balloon.out

OBJS=$(patsubst %.cpp,%.o,$(SRC))

all: $(EXEC)

release: CXXFLAGS+=-O2
release: all

debug: CXXFLAGS+=-g
debug: all

$(EXEC): $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(CXXLIBS)

%.o: %.cpp $(HDR)
	$(CXX) -o $@ $(CXXFLAGS) -c $< $(CXXLIBS)

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean

