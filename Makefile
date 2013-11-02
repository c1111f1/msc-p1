#Make file for MSc project - Real Time Video Communication
#Fei Cheng
#28Th 10 2013
#XJTLU

#Flags
CC = gcc
CFLAGS = -Wall -Werror -Wcast-align -g
LDFLAGS =
LIBS_C = `pkg-config --cflags sdl` -lx264 -lm -lpthread -ldl 
LIBS = `pkg-config --libs sdl` -lx264 -lm -lpthread -ldl 

#PATH
SRC_PATH = code/
BIN_PATH = bin/
OBJ_PATH = obj/
DEP_LIBS = -L.

SWCOBJECT = $(OBJ_PATH)swc.o $(OBJ_PATH)opt.o $(OBJ_PATH)video.o $(OBJ_PATH)screen.o $(OBJ_PATH)x264_encoder.o

all: $(BIN_PATH)swc

$(BIN_PATH)swc: $(SWCOBJECT)
	$(CC) $(LDFLAGS) $(SWCOBJECT) -o $@ $(LIBS) $(DEP_LIBS)

$(OBJ_PATH)swc.o: $(SRC_PATH)swc.c $(SRC_PATH)x264.h
	$(CC) $(CFLAGS) -c $< $(LIBS_C) -o $@

$(OBJ_PATH)opt.o: $(SRC_PATH)opt.c $(SRC_PATH)opt.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)video.o: $(SRC_PATH)video.c $(SRC_PATH)video.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)screen.o: $(SRC_PATH)screen.c $(SRC_PATH)screen.h
	$(CC) $(CFLAGS)  -c $< $(LIBS_C) -o $@

$(OBJ_PATH)x264_encoder.o: $(SRC_PATH)x264_encoder.c $(SRC_PATH)x264.h $(SRC_PATH)x264_config.h $(SRC_PATH)x264_encoder.h
	$(CC) $(CFLAGS)  -c $< -o $@
	
clean:
	rm -f $(OBJ_PATH)*.o $(BIN_PATH)swc

.PHONY: all clean
