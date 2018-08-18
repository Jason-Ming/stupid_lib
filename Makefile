#debug文件夹里的makefile文件需要最后执行，所以这里需要执行的子目录要排除debug文件夹，这里使用awk排除了debug文件夹，读取剩下的文件夹
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "include" && $$9 != "debug" && $$9 != "test" && $$9 != "test_files") print $$9}')
#无需下一行的注释代码，因为我们已经知道debug里的makefile是最后执行的，所以最后直接去debug目录下执行指定的makefile文件就行，具体下面有注释
#DEBUG=$(shell ls -l | grep ^d | awk '{if($$9 == "debug") print $$9}')

#使用的外部库的路径
LIB_PATH=$(HOME)/github/stupid_lib
#外部库文件的名字
LIB_BIN=stupid
#外部库文件的名字
LIB_BIN_TEST=stupid_test

#记住当前工程的根目录路径
ROOT_DIR=$(shell pwd)
#当前路径
CUR_DIR=$(shell pwd)
#目标文件所在的目录
CUR_OBJS_DIR=debug/obj
#目标文件所在的目录
CUR_OBJS_TEST_DIR=debug/obj_test
#bin文件所在的目录
CUR_BIN_DIR=debug/bin
#lib文件所在的目录，此目录为项目生成的动态库目标目录
CUR_LIB_DIR=debug/lib

#最终bin文件的名字，可以更改为自己需要的
CUR_BIN=main
#最终测试bin文件的名字，可以更改为自己需要的
CUR_BIN_TEST=tst
#外部库文件的名字
CUR_LIB_BIN=stupid
#外部库文件的名字
CUR_LIB_BIN_TEST=stupid_test

#获取当前目录下的c文件集，放在变量CUR_SOURCE中
CUR_SOURCE=${wildcard *.c}
#将对应的c文件名转为o文件后放在下面的CUR_OBJS变量中
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

CPPUTEST_HOME = /usr

#设置编译器
CPP     = g++
CPPFLAGS  += -g -Wall
#CPPFLAGS  += -D CPPUTEST
CPPFLAGS  += -I$(CPPUTEST_HOME)/include
CPPFLAGS  += -I$(ROOT_DIR)/include
CPPFLAGS  += -I$(ROOT_DIR)/src/include
CPPFLAGS  += -I$(ROOT_DIR)/src/s_cmd
#CPPFLAGS  += -I$(LIB_PATH)/include
#CPPFLAGS works for both .c and .cpp files!

CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h

CPPINCLUDEFLAGS = -I$(ROOT_DIR)/include 
CPPINCLUDEFLAGS += -I$(ROOT_DIR)/src/include
CPPINCLUDEFLAGS  += -I$(ROOT_DIR)/src/s_cmd
#CPPINCLUDEFLAGS += -I$(LIB_PATH)/include

CXXLDFLAGS := -L$(CUR_DIR)/$(CUR_LIB_DIR) -l$(CUR_LIB_BIN_TEST)
#CXXLDFLAGS += -L$(LIB_PATH)/debug/lib -l$(LIB_BIN_TEST)
CXXLDFLAGS += -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt -lm

#设置编译器
CC=gcc
CFLAGS  += -g -Wall
CFLAGS  += -std=c99
CFLAGS   += -I$(ROOT_DIR)/include
CFLAGS   += -I$(ROOT_DIR)/src/include
#CFLAGS   += -I$(LIB_PATH)/include

CFLAGS_TEST  = -D CPPUTEST
CFLAGS_TEST  += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h

INCLUDEFLAGS = -I$(ROOT_DIR)/include
INCLUDEFLAGS += -I$(ROOT_DIR)/src/include
#INCLUDEFLAGS += -I$(LIB_PATH)/include

CLDFLAGS += -lm
CLDFLAGS_TEST = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt -lm

#将以下变量导出到子shell中，本次相当于导出到子目录下的makefile中
export CPPUTEST_HOME CC CFLAGS CFLAGS_TEST CLDFLAGS CLDFLAGS_TEST CPP CPPFLAGS CXXFLAGS CXXLDFLAGS INCLUDEFLAGS CPPINCLUDEFLAGS CUR_BIN CUR_BIN_TEST CUR_LIB_BIN CUR_LIB_BIN_TEST ROOT_DIR CUR_OBJS_DIR CUR_OBJS_TEST_DIR CUR_BIN_DIR CUR_LIB_DIR LIB_PATH LIB_BIN LIB_BIN_TEST CUR_BIN

#注意这里的顺序，需要先执行SUBDIRS最后才能是DEBUG
all:$(SUBDIRS) $(CUR_OBJS) DEBUG TEST

#递归执行子目录下的makefile文件，这是递归执行的关键
$(SUBDIRS):ECHO
	make -C $@

DEBUG:ECHO
#直接去debug目录下执行makefile文件
	make -C debug

TEST:ECHO
#直接去debug目录下执行makefile文件
	make -C test

ECHO:
	@echo $(SUBDIRS)

#将c文件编译为o文件，并放在指定放置目标文件的目录中即OBJS_DIR
$(CUR_OBJS):%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)
	@cp $@ $(ROOT_DIR)/$(CUR_OBJS_DIR)/$@
	$(CC) -o $@ -c $< $(CFLAGS) $(CFLAGS_TEST)
	@cp $@ $(ROOT_DIR)/$(CUR_OBJS_TEST_DIR)/$@

%.d:%.c
	@set -e; rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(CUR_OBJS:.o=.d)

ALL_D = $(shell find -type f -name '*.d')
ALL_D_TEMP = $(shell find -type f -name '*.d.*')
ALL_O = $(shell find -type f -name '*.o')
ALL_TXT = $(shell find -type f -name '*.txt')
ALL_I = $(shell find -type f -name '*.i')

clean:
	@rm -rf $(CUR_OBJS_DIR)/*
	@rm -rf $(CUR_OBJS_TEST_DIR)/*
	@rm -rf $(CUR_BIN_DIR)/*
	@rm -rf $(CUR_LIB_DIR)/*
	@rm -rf test/$(CUR_OBJS_DIR)/*
	@rm -rf test/$(CUR_BIN_DIR)/*
	@rm -rf test/$(CUR_LIB_DIR)/*
	@rm -rf $(ALL_D)
	@rm -rf $(ALL_D_TEMP)
	@rm -rf $(ALL_TXT)
	@rm -rf $(ALL_O)
	@rm -rf $(ALL_I)