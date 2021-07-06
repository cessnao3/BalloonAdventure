# Define the default compiler
CXX = g++

# Define the initial flags and libriaries
CXXFLAGS = $(file <BalloonAdventure.cxxflags)
CXXLIBS = $(shell pkg-config --libs allegro-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_ttf-5)

# Define the include directories and add to the flags
INCLUDE_DIRS = $(file <BalloonAdventure.includes)
CXXFLAGS += $(patsubst %,-I%,$(INCLUDE_DIRS))

# Define the output executable
EXEC = balloon.out

# Define the resulting output objects
OBJS = $(patsubst %.cpp,%.o,$(SRC))

# Read in all files and filter the source and header files
ALLFILES = $(file <BalloonAdventure.files)
SRC = $(filter %.cpp,$(ALLFILES))
HDR = %(filter %.h,%(ALLFILES))

# Provide the default target
all: $(EXEC)

# Define updates for release
release: CXXFLAGS += -O2
release: all

# Define updates for debugging
debug: CXXFLAGS += -g -fsanitize=address
debug: all

profile: CXXFLAGS += -pg
profile: all

# Provide the main executable file
$(EXEC): $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(CXXLIBS)

# Provide the targets for individual output files
%.o: %.cpp $(HDR)
	$(CXX) -o $@ $(CXXFLAGS) -c $< $(CXXLIBS)

# Provide the main clean function
clean:
	rm -f $(OBJS) $(EXEC)

# Mark non-files as virtual targets
.PHONY: all clean release debug
