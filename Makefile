GCC = g++
CPP = c++17

MARIO_SRC = src
MARIO_INC = include

LOGGER_SRC = Logger/src
LOGGER_INC = Logger/include

TIMER_SRC = Timer/src
TIMER_INC = Timer/include

BIN = bin

SFML_GRAPHICS = -lsfml-graphics
SFML_SYSTEM = -lsfml-system
SFML_WINDOW = -lsfml-window
SFML_AUDIO = -lsfml-audio

SFML_LIB = $(SFML_GRAPHICS)
SFML_LIB += $(SFML_SYSTEM)
SFML_LIB += $(SFML_WINDOW)
SFML_LIB += $(SFML_AUDIO)

SRC += main.cpp
SRC += $(MARIO_SRC)/Mario.cpp
SRC += $(MARIO_SRC)/SfmlRenders.cpp
SRC += $(MARIO_SRC)/WelcomeScreen.cpp
SRC += $(MARIO_SRC)/TileMap.cpp
SRC += $(MARIO_SRC)/Obstacle.cpp
SRC += $(MARIO_SRC)/InitObstacle.cpp
SRC += $(MARIO_SRC)/Block.cpp
SRC += $(MARIO_SRC)/Entity.cpp
SRC += $(MARIO_SRC)/Player.cpp
SRC += $(MARIO_SRC)/SoundBuffer.cpp
SRC += $(MARIO_SRC)/Item.cpp
SRC += $(MARIO_SRC)/Bullet.cpp
#SRC += $(MARIO_SRC)/Enemy.cpp

SRC += $(LOGGER_SRC)/Logger.cpp
SRC += $(TIMER_SRC)/Timer.cpp

INC += -I$(MARIO_INC)
INC += -I$(LOGGER_INC)
INC += -I$(TIMER_INC)

OBJ = $(BIN)/mario.out

default:
	clear
	$(GCC) -g -std=$(CPP) $(SRC) $(SFML_LIB) $(INC) $(SFML_LIB) -o$(OBJ)
	clear
	./$(BIN)/Mario.out

clean:
	rm -rf $(BIN)/mario.out
	rm -rf $(BIN)/mario.out.dSYM
