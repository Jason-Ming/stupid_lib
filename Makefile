#debug�ļ������makefile�ļ���Ҫ���ִ�У�����������Ҫִ�е���Ŀ¼Ҫ�ų�debug�ļ��У�����ʹ��awk�ų���debug�ļ��У���ȡʣ�µ��ļ���
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "include" && $$9 != "debug" && $$9 != "test" && $$9 != "test_files") print $$9}')
#������һ�е�ע�ʹ��룬��Ϊ�����Ѿ�֪��debug���makefile�����ִ�еģ��������ֱ��ȥdebugĿ¼��ִ��ָ����makefile�ļ����У�����������ע��
#DEBUG=$(shell ls -l | grep ^d | awk '{if($$9 == "debug") print $$9}')

#ʹ�õ��ⲿ���·��
LIB_PATH=$(HOME)/github/stupid_lib
#�ⲿ���ļ�������
LIB_BIN=stupid
#�ⲿ���ļ�������
LIB_BIN_TEST=stupid_test

#��ס��ǰ���̵ĸ�Ŀ¼·��
ROOT_DIR=$(shell pwd)
#��ǰ·��
CUR_DIR=$(shell pwd)
#Ŀ���ļ����ڵ�Ŀ¼
CUR_OBJS_DIR=debug/obj
#Ŀ���ļ����ڵ�Ŀ¼
CUR_OBJS_TEST_DIR=debug/obj_test
#bin�ļ����ڵ�Ŀ¼
CUR_BIN_DIR=debug/bin
#lib�ļ����ڵ�Ŀ¼����Ŀ¼Ϊ��Ŀ���ɵĶ�̬��Ŀ��Ŀ¼
CUR_LIB_DIR=debug/lib

#����bin�ļ������֣����Ը���Ϊ�Լ���Ҫ��
CUR_BIN=main
#���ղ���bin�ļ������֣����Ը���Ϊ�Լ���Ҫ��
CUR_BIN_TEST=tst
#�ⲿ���ļ�������
CUR_LIB_BIN=stupid
#�ⲿ���ļ�������
CUR_LIB_BIN_TEST=stupid_test

#��ȡ��ǰĿ¼�µ�c�ļ��������ڱ���CUR_SOURCE��
CUR_SOURCE=${wildcard *.c}
#����Ӧ��c�ļ���תΪo�ļ�����������CUR_OBJS������
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

CPPUTEST_HOME = /usr

#���ñ�����
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

#���ñ�����
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

#�����±�����������shell�У������൱�ڵ�������Ŀ¼�µ�makefile��
export CPPUTEST_HOME CC CFLAGS CFLAGS_TEST CLDFLAGS CLDFLAGS_TEST CPP CPPFLAGS CXXFLAGS CXXLDFLAGS INCLUDEFLAGS CPPINCLUDEFLAGS CUR_BIN CUR_BIN_TEST CUR_LIB_BIN CUR_LIB_BIN_TEST ROOT_DIR CUR_OBJS_DIR CUR_OBJS_TEST_DIR CUR_BIN_DIR CUR_LIB_DIR LIB_PATH LIB_BIN LIB_BIN_TEST CUR_BIN

#ע�������˳����Ҫ��ִ��SUBDIRS��������DEBUG
all:$(SUBDIRS) $(CUR_OBJS) DEBUG TEST

#�ݹ�ִ����Ŀ¼�µ�makefile�ļ������ǵݹ�ִ�еĹؼ�
$(SUBDIRS):ECHO
	make -C $@

DEBUG:ECHO
#ֱ��ȥdebugĿ¼��ִ��makefile�ļ�
	make -C debug

TEST:ECHO
#ֱ��ȥdebugĿ¼��ִ��makefile�ļ�
	make -C test

ECHO:
	@echo $(SUBDIRS)

#��c�ļ�����Ϊo�ļ���������ָ������Ŀ���ļ���Ŀ¼�м�OBJS_DIR
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