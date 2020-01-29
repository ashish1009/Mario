GCC = g++
CPP = c++17

MARIO_SRC = src/
MARIO_INC = include/

LOGGER_SRC = Logger/src/
LOGGER_INC = Logger/include/

LINKED_LIST_SRC = LinkedList/src/
LINKED_LIST_INC = LinkedList/include/

SFML_GRAPHICS = -lsfml-graphics
SFML_SYSTEM = -lsfml-system
SFML_WINDOW = -lsfml-window

SFML_LIB = $(SFML_GRAPHICS)
SFML_LIB += $(SFML_SYSTEM)
SFML_LIB += $(SFML_WINDOW)

SRC = main.cpp
SRC += $(MARIO_SRC)Entity.cpp
SRC += $(MARIO_SRC)Player.cpp
SRC += $(MARIO_SRC)Mario.cpp
SRC += $(MARIO_SRC)TileMap.cpp
SRC += $(MARIO_SRC)Obstacle.cpp
SRC += $(MARIO_SRC)Block.cpp

SRC += $(LINKED_LIST_SRC)LinkedList.cpp

SRC += $(LOGGER_SRC)Logger.cpp

INC = -I$(MARIO_INC)
INC += -I$(LOGGER_INC)

OBJ = mario.out

default:
	$(GCC) -g -std=$(CPP) $(SRC) $(SFML_LIB) $(INC) $(SFML_LIB) -o$(OBJ)

clean:
	rm -rf mario.out
	rm -rf mario.out.dSYM
