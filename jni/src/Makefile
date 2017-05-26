CXXFLAGS = -Wall -DRESDIR=\"../../assets/\" -I.
UNAME    = $(shell uname -s)

SDL_SRC = kiss_example2.c kiss_widgets.c kiss_posix.c kiss_general.c \
	kiss_draw.c
ifeq ($(UNAME), Darwin)
	SDL_TTF_CXXFLAGS = -I/Library/Frameworks/SDL2_ttf.framework/Headers/
	SDL_IMAGE_CXXFLAGS = -I/Library/Frameworks/SDL2_image.framework/Headers/
  SDL_LIB = -lpthread -framework SDL2 -framework SDL2_ttf -framework SDL2_image
else
  SDL_LIB = -lpthread -lSDL2 -lSDL2_ttf -lSDL2_image
endif

all: clean kiss_example2

kiss_example2:
	$(CXX) $(CXXFLAGS) $(SDL_TTF_CXXFLAGS) $(SDL_IMAGE_CXXFLAGS) `sdl2-config --cflags` -o kiss_example2 $(SDL_SRC) $(SDL_LIB)

clean:
	rm -rf sdl *.dSYM
