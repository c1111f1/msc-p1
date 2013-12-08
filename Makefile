#Make file for MSc project - Real Time Video Communication
#Fei Cheng
#28Th 10 2013
#XJTLU

#LIBS_C = -lm -lpthread -ldl -I/home/fred/jrtplib-3.9.1/src
#LIBS = `pkg-config --libs sdl` -lx264 -lm -lpthread -ldl 

PLATFORM = ARM

ifeq ($(PLATFORM),ARM)
	CC = arm-linux-gnueabihf-g++
	EXN = ht_ard
else
	CC = g++
	EXN = ht_pc
endif

DIR=$(shell pwd)
INCS = -I.\
       -I$(DIR) \
       -I$(DIR)/hardware \
       -I$(DIR)/hardware/arduino \
       -I$(DIR)/hardware/arduino/cores \
       -I$(DIR)/hardware/arduino/cores/arduino \
       -I$(DIR)/hardware/arduino/variants \
       -I$(DIR)/hardware/arduino/variants/sunxi \
       -I$(DIR)/libraries \
       -I$(DIR)/libraries/Serial \
       -I$(DIR)/libraries/SPI \
	-I$(DIR)/libraries/Wire \
	-I$(DIR)/libraries/LiquidCrystal 

CFLAGS = -pipe -O2 
LDFLAGS = -pipe -O2 
LIBS_C = -I./code/rtp -lx264 -lm -lpthread -ldl 
#LIBS = -lSDL -lx264 -lm -lpthread -ljrtp -ljthread  -ldl
#`pkg-config --libs sdl`

#PATH
SRC_PATH = code/
BIN_PATH = bin/
OBJ_PATH = obj/

ifeq ($(PLATFORM),ARM)
	LIBS = -lx264 -lm -lpthread -ljrtp -ljthread  -ldl -larduino
	DEP_LIBS = -L./libarm
else
	LIBS = -lx264 -lm -lpthread -ljrtp -ljthread  -ldl
	DEP_LIBS = -L./libpc
endif


SWCOBJECT = $(OBJ_PATH)swc.o $(OBJ_PATH)opt.o $(OBJ_PATH)video.o $(OBJ_PATH)screen.o $(OBJ_PATH)x264_encoder.o $(OBJ_PATH)rtp.o 

all: $(BIN_PATH)$(EXN)

$(BIN_PATH)$(EXN): $(SWCOBJECT)
	$(CC) $(LDFLAGS) $(SWCOBJECT) -o $@ $(LIBS) $(DEP_LIBS)
ifeq ($(PLATFORM),ARM)
	scp bin/$(EXN) ubuntu@192.168.1.106:~/ht_ardT
endif
	
$(OBJ_PATH)swc.o: $(SRC_PATH)swc.c $(SRC_PATH)x264.h

ifeq ($(PLATFORM),ARM)
	@echo '#define _ARM' > code/platform.h
else
	@echo '#define _PC' > code/platform.h
endif

	$(CC) $(CFLAGS) -c $< $(LIBS_C) $(INCS) -o $@

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
