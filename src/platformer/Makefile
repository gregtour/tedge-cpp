# Bloke Platformer Makefile

BINARY_TARGET = ../../bin/bloke/bloke
INCLUDES = -I /usr/include/SDL/

GAME_SRC =	main.cpp \
        entity/cameraController.cpp entity/gun.cpp entity/level.cpp \
        entity/platform.cpp entity/player.cpp game/game.cpp game/gameInput.cpp

ENGINE_SRC =	../engine/engine.cpp ../engine/update.cpp \
		../engine/common/log.cpp \
		../engine/common/resource.cpp ../engine/common/timer.cpp \
		../engine/entity/camera.cpp ../engine/entity/entity.cpp \
		../engine/entity/graphicalObj.cpp ../engine/entity/inputtingObj.cpp \
		../engine/entity/loggableObj.cpp ../engine/entity/physicalObj.cpp \
		../engine/graphics/font.cpp \
		../engine/input/input.cpp ../engine/input/inputData.cpp \
		../engine/physics/collision.cpp ../engine/physics/common.cpp \
		../engine/physics/matrix.cpp ../engine/physics/svector.cpp \
		../engine/physics/object.cpp ../engine/physics/physics.cpp
#		../engine/sound/music.cpp ../engine/sound/sound.cpp \
#		../engine/sound/wavesound.cpp

GRAPHICS_SRC =	../engine/graphics/graphics.cpp \
		../engine/graphics/lighting.cpp ../engine/graphics/particle.cpp \
		../engine/graphics/texture.cpp ../engine/graphics/model.cpp \
		../engine/graphics/buffer/IGLIndxBuf.cpp ../engine/graphics/buffer/IGLVertBuf.cpp \
		../engine/graphics/model/msModel.cpp ../engine/graphics/model/mathlib.cpp \

####
SOURCES = $(GAME_SRC) $(ENGINE_SRC) $(GRAPHICS_SRC)
OBJECTS = $(SOURCES:.cpp=.o)
GRAPHICS_OBJECTS = $(GRAPHICS_SRC:.cpp=.o)
####

MAKE_OPTIONS =	-D _OPEN_GL -O2
#-Wall -Wno-unused-value -ansi -D _OPEN_GL

LIBS = 		-lGL -lGLU -lSDL -lSDL_image
# -lSDL_mixer

OPTIONS = 	$(LIBS) `sdl-config --cflags --libs`

#all:
#	g++ $(MAKE_OPTIONS) $(INCLUDES) $(GAME_SRC) $(ENGINE_SRC) -o $(BINARY_TARGET) $(OPTIONS)

all: $(SOURCES) $(BINARY_TARGET)

$(BINARY_TARGET): $(OBJECTS)
	g++ $(MAKE_OPTIONS) $(INCLUDES) -o $@ $^ $(OPTIONS)

.cpp.o:
	g++ $(MAKE_OPTIONS) $(INCLUDES) -c -o $@ $<


debug:
	g++ $(MAKE_OPTIONS) $(INCLUDES) $(GAME_SRC) $(ENGINE_SRC) -o $(BINARY_TARGET) $(OPTIONS) -g

clean:
	@echo Cleaning up...
	rm -rf $(BINARY_TARGET)
	rm $(OBJECTS)
	@echo Done.

clean_gfx:
	@echo Cleaning...
	rm $(GRAPHICS_OBJECTS)
	@echo Done.
