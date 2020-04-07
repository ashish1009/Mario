GCC = g++
CPP = c++17

MARIO_SRC = src
MARIO_INC = include

LOGGER_SRC = Logger/src
LOGGER_INC = Logger/include

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
SRC += $(MARIO_SRC)/Entity.cpp
SRC += $(MARIO_SRC)/Player.cpp
SRC += $(MARIO_SRC)/Mario.cpp
SRC += $(MARIO_SRC)/TileMap.cpp
SRC += $(MARIO_SRC)/Obstacle.cpp
SRC += $(MARIO_SRC)/Block.cpp
SRC += $(MARIO_SRC)/InitObstacle.cpp
SRC += $(MARIO_SRC)/Bonus.cpp
SRC += $(MARIO_SRC)/Fire.cpp

SRC += $(LOGGER_SRC)/Logger.cpp

INC += -I$(MARIO_INC)
INC += -I$(LOGGER_INC)

OBJ = mario.out

default:
	clear
	$(GCC) -g -std=$(CPP) $(SRC) $(SFML_LIB) $(INC) $(SFML_LIB) -o$(OBJ)
	clear
	./Mario.out

clean:
	rm -rf mario.out
	rm -rf mario.out.dSYM