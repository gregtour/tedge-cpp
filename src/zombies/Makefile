# Zombies Stole My Truck

BINARY_TARGET = ../../bin/zombies/zombies
INCLUDES = -I /usr/include/SDL/

GAME_SRC =	main.cpp \
		entity/cameraController.cpp entity/gun.cpp entity/track.cpp \
		entity/platform.cpp entity/vehicle.cpp game/game.cpp game/gameInput.cpp \
		entity/zombie.cpp entity/player.cpp \
		entity/particles/death.cpp entity/particles/explosion.cpp entity/particles/fire.cpp \
		entity/particles/fountain.cpp entity/particles/gem.cpp entity/particles/laserBlast.cpp \
		entity/particles/playerDeath.cpp entity/particles/smoke.cpp entity/particles/spin.cpp

ENGINE_SRC =	../engine/engine.cpp ../engine/update.cpp \
		../engine/common/log.cpp \
		../engine/common/profile.cpp \
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
		../engine/sound/wavesound.cpp
#		../engine/graphics/TexFont3.cpp

MAKE_OPTIONS =	-Wall -Wno-unused-value -O3 -ansi -D _OPEN_GL

LIBS = 		-lGL -lGLU -lSDL -lSDL_mixer -lSDL_image -lrt -lpthread

OPTIONS = 	$(LIBS) `sdl-config --cflags --libs`

all:
	g++ $(MAKE_OPTIONS) $(INCLUDES) $(GAME_SRC) $(ENGINE_SRC) -o $(BINARY_TARGET) $(OPTIONS)

threaded:
	g++ $(MAKE_OPTIONS) $(INCLUDES) $(GAME_SRC) $(ENGINE_SRC) -o $(BINARY_TARGET) $(OPTIONS) -D THREADED_PHYSICS -lpthread

debug:
	g++ $(MAKE_OPTIONS) $(INCLUDES) $(GAME_SRC) $(ENGINE_SRC) -o $(BINARY_TARGET) $(OPTIONS) -g

profile:
	g++ $(MAKE_OPTIONS) $(INCLUDES) $(GAME_SRC) $(ENGINE_SRC) -o $(BINARY_TARGET) $(OPTIONS) -g -pg

clean:
	@echo Cleaning up...
	rm -rf $(BINARY_TARGET)
	@echo Done.
