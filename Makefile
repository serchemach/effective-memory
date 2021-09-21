BUILD_TARGET = main.exe
LIBS_PATH = lib
SRC_PATH = src
INCLUDE_PATH = include
#LIBS = $(patsubst %.a,-l%, $(notdir $(wildcard $(LIBS_PATH)/*.a)))
LIBS = -lmingw32 -lSDL2main -lSDL2
SOURCES = $(wildcard $(SRC_PATH)/*.c)
OBJ := $(patsubst $(SRC_PATH)/%.c, %.o, $(SOURCES))

.PHONY: all clean

all : $(BUILD_TARGET)
	echo All done

$(BUILD_TARGET) : $(OBJ)
	gcc -o $@ $^ -L$(LIBS_PATH) $(LIBS)

%.o : $(SRC_PATH)/%.c 
	gcc -c $< -o $@ -I$(INCLUDE_PATH)

clean : 
	del $(wildcard *.o) $(BUILD_TARGET) 
	echo Clean done 

-include $(OBJ:.o=.d)
