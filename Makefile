#Make file for MSc project - Real Time Video Communication
#Fei Cheng
#28Th 10 2013
#XJTLU

#LIBS_C = -lm -lpthread -ldl -I/home/fred/jrtplib-3.9.1/src
#LIBS = `pkg-config --libs sdl` -lx264 -lm -lpthread -ldl 

PLATFORM = PC

#Flags
CC = g++
#CC = arm-linux-gnueabihf-g++
CFLAGS = -pipe -O2 
LDFLAGS = -pipe -O2 
LIBS_C = -I/usr/include/SDL -I./code/rtp -lx264 -lm -lpthread -ldl 
LIBS = -lSDL -lx264 -lm -lpthread -ljrtp -ljthread  -ldl
#`pkg-config --libs sdl`

#PATH
SRC_PATH = code/
BIN_PATH = bin/
OBJ_PATH = obj/
DEP_LIBS = -L./libpc

SWCOBJECT = $(OBJ_PATH)swc.o $(OBJ_PATH)opt.o $(OBJ_PATH)video.o $(OBJ_PATH)screen.o $(OBJ_PATH)x264_encoder.o $(OBJ_PATH)rtp.o 

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
	$(CC) $(CFLAGS) -c $< $(LIBS_C) -o $@

$(OBJ_PATH)x264_encoder.o: $(SRC_PATH)x264_encoder.c $(SRC_PATH)x264.h $(SRC_PATH)x264_config.h $(SRC_PATH)x264_encoder.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)rtp.o: $(SRC_PATH)rtp.cpp
	$(CC) $(CFLAGS)  -c $< $(LIBS_C) -o $@
	
clean:
	rm -f $(OBJ_PATH)*.o $(BIN_PATH)swc

.PHONY: all clean
