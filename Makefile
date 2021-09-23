BUILD_TARGET = main.exe
LIBS_PATH = lib
SRC_PATH = src
INCLUDE_PATH = include -Iinclude/SDL2
#LIBS = $(patsubst %.a,-l%, $(notdir $(wildcard $(LIBS_PATH)/*.a)))
LIBS = -lmingw32 -lSDL2main -lSDL2_ttf -lSDL2_image -lSDL2 -lglew32 -lopengl32 -lglu32

CC = gcc
LD = $(CC)

SOURCES = $(wildcard $(SRC_PATH)/*.c)
OBJ := $(patsubst $(SRC_PATH)/%.c, %.o, $(SOURCES))

.PHONY: all clean

all : $(BUILD_TARGET)
	echo All done

$(BUILD_TARGET) : $(OBJ)
	$(CC) -o $@ $^ -L$(LIBS_PATH) $(LIBS)

%.o : $(SRC_PATH)/%.c 
	$(LD) -c $< -o $@ -I$(INCLUDE_PATH)

clean : 
	del $(wildcard *.o) $(BUILD_TARGET) 
	echo Clean done 

-include $(OBJ:.o=.d)
