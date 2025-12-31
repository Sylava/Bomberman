CC = g++
CFLAGS = -Wall -g -I include
LDFLAGS = -L lib -lmingw32 -lSDL2main -lSDL2

SRC_DIRS = src src/Helper src/Math
SRC = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
OBJ = $(SRC:src/%.cpp=obj/%.o)

Bomberman : $(OBJ)
	$(CC) $(OBJ) -o Bomberman $(LDFLAGS)

obj/%.o : src/%.cpp
	if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	del /S /Q obj 2> NUL

fclean : clean
	del /Q Bomberman.exe 2> NUL

re : fclean Bomberman
