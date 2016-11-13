#OBJS specifies which files to compile as part of the project
OBJS = 01_hello_SDL.c
#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = 01_hello_SDL 
#This is the target that compiles our executable
all : $(OBJS) 
	g++ $(OBJS) -w  -ltmx -lxml2 -lz -lSDL2 -lSDL2_image -o $(OBJ_NAME)
