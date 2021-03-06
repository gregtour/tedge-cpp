# Chronicles Computer Makefile

BINARY_TARGET = ../../bin/chronicles/chronicles.bin
INCLUDES = -I /usr/include/SDL/

#GAME_SRC =	main.cpp

GAME_SRC = 	main.cpp \
		entity/bat.cpp entity/boss.cpp entity/cameraController.cpp entity/character.cpp \
		entity/gate.cpp entity/ghost.cpp \
		entity/gremling.cpp entity/hud.cpp entity/item.cpp entity/level.cpp \
		entity/menuInput.cpp entity/monster.cpp entity/pickup.cpp entity/player.cpp \
		entity/portal.cpp entity/spike.cpp entity/pause.cpp \
		entity/particles/death.cpp entity/particles/fire.cpp entity/particles/fountain.cpp \
		entity/particles/gem.cpp entity/particles/playerDeath.cpp entity/particles/smoke.cpp \
		entity/particles/spin.cpp \
		game/game.cpp game/gameInput.cpp \
		game/options.cpp game/splash.cpp

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
		../engine/sound/wavesound.cpp

####
SOURCES = $(GAME_SRC) $(ENGINE_SRC)
OBJECTS = $(SOURCES:.cpp=.o)
####

MAKE_OPTIONS =	-O2 -D _OPEN_GL -D NO_SOUND
# -Wall -Wno-unused-value -ansi 
# -D _DEBUG

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

clean2: 
	@echo Cleaning...
	rm $(OBJECTS)
clean:
	@echo Cleaning up...
	rm -rf $(BINARY_TARGET)
	rm $(OBJECTS)
	@echo Done.
