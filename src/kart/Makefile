# Kart Thing Makefile

BINARY_TARGET = ../../bin/fiesta/fiesta
INCLUDES = -I /usr/include/SDL/

GAME_SRC = *.cpp entity/*.cpp game/*.cpp entity/particles/*.cpp
#main.cpp \
#        entity/cameraController.cpp entity/gun.cpp entity/level.cpp \
#        entity/platform.cpp entity/player.cpp game/game.cpp game/gameInput.cpp

ENGINE_SRC =	../engine/engine.cpp ../engine/update.cpp \
		../engine/common/log.cpp \
		../engine/common/resource.cpp ../engine/common/timer.cpp \
		../engine/entity/camera.cpp ../engine/entity/entity.cpp \
		../engine/entity/graphicalObj.cpp ../engine/entity/inputtingObj.cpp \
		../engine/entity/loggableObj.cpp ../engine/entity/physicalObj.cpp \
		../engine/graphics/font.cpp ../engine/graphics/graphics.cpp \
		../engine/graphics/lighting.cpp ../engine/graphics/particle.cpp \
		../engine/graphics/texture.cpp ../engine/graphics/model.cpp \
		../engine/graphics/buffer/IGLIndxBuf.cpp ../engine/graphics/buffer/IGLVertBuf.cpp \
		../engine/graphics/model/msModel.cpp ../engine/graphics/model/mathlib.cpp \
		../engine/input/input.cpp ../engine/input/inputData.cpp \
		../engine/physics/collision.cpp ../engine/physics/common.cpp \
		../engine/physics/matrix.cpp ../engine/physics/svector.cpp \
		../engine/physics/object.cpp ../engine/physics/physics.cpp \
		../engine/sound/music.cpp ../engine/sound/sound.cpp \
		../engine/sound/wavesound.cpp ../engine/common/profile.cpp

MAKE_OPTIONS =	-Wall -Wno-unused-value -ansi -D _OPEN_GL

LIBS = 		-lGL -lGLU -lSDL -lSDL_mixer -lSDL_image

OPTIONS = 	$(LIBS) `sdl-config --cflags --libs`

all:
	g++ $(MAKE_OPTIONS) $(INCLUDES) $(GAME_SRC) $(ENGINE_SRC) -o $(BINARY_TARGET) $(OPTIONS)

debug:
	g++ $(MAKE_OPTIONS) $(INCLUDES) $(GAME_SRC) $(ENGINE_SRC) -o $(BINARY_TARGET) $(OPTIONS) -g

clean:
	@echo Cleaning up...
	rm -rf $(BINARY_TARGET)
	@echo Done.
