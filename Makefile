LIB_NAME:=stupid
SRC_PATH:=./src
BIN_PATH:=./lib

TARGET = static_library shared_library clean_o
all:$(TARGET)

CC      := gcc
CFLAGS    += -g -Wall
CFLAGS  += -std=c99
#CFLAGS  += -D CPPUTEST
#CFLAGS  += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
CFLAGS   += -I./include

#LDFLAGS := -L$(CPPUTEST_HOME)/lib -lCppUTest

static_library:
	$(CC) -c $(SRC_PATH)/*.c $(CFLAGS)
	ar  -cr $(BIN_PATH)/lib${LIB_NAME}.a *.o

shared_library:
	$(CC) -shared -fPIC -o $(BIN_PATH)/lib${LIB_NAME}.so $(SRC_PATH)/*.c $(CFLAGS)

clean_o:
	rm -rf ./*.o
	
.PHONY: clean
clean:
	@echo "clean..."
	rm -rf $(SRC_PATH)/*.o $(BIN_PATH)/*.a $(BIN_PATH)/*.so

