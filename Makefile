# OBJS specifies which files to compile as part of the project
OBJS = main.c

# CC specifies which compiler we're using
CC = cc

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -g produces debugging information in the operating system's native format (so we can use gdb)
# -Wall enables all compiler's warning messages.
COMPILER_FLAGS = -Wall -g

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2

# OBJ_NAME specifies the name of our executable
OBJ_NAME = snake

# This is the target that compiles our executable
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
