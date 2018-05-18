#debug�ļ������makefile�ļ���Ҫ���ִ�У�����������Ҫִ�е���Ŀ¼Ҫ�ų�debug�ļ��У�����ʹ��awk�ų���debug�ļ��У���ȡʣ�µ��ļ���
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "debug") print $$9}')
#������һ�е�ע�ʹ��룬��Ϊ�����Ѿ�֪��debug���makefile�����ִ�еģ��������ֱ��ȥdebugĿ¼��ִ��ָ����makefile�ļ����У�����������ע��
#DEBUG=$(shell ls -l | grep ^d | awk '{if($$9 == "debug") print $$9}')
#��ס��ǰ���̵ĸ�Ŀ¼·��
ROOT_DIR=$(shell pwd)
#����bin�ļ������֣����Ը���Ϊ�Լ���Ҫ��
BIN=stupid
#Ŀ���ļ����ڵ�Ŀ¼
OBJS_DIR=debug/obj
#bin�ļ����ڵ�Ŀ¼
BIN_DIR=debug/bin
#lib�ļ����ڵ�Ŀ¼
LIB_DIR=debug/lib
#��ȡ��ǰĿ¼�µ�c�ļ��������ڱ���CUR_SOURCE��
CUR_SOURCE=${wildcard *.c}
#����Ӧ��c�ļ���תΪo�ļ�����������CUR_OBJS������
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}
#���ñ�����
CC=gcc
CFLAGS  += -g -Wall
CFLAGS  += -std=c99
#CFLAGS  += -D CPPUTEST
#CFLAGS  += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
CFLAGS   += -I$(ROOT_DIR)/include
#�����±�����������shell�У������൱�ڵ�������Ŀ¼�µ�makefile��
export CC CFLAGS BIN OBJS_DIR BIN_DIR ROOT_DIR LIB_DIR ALL_SOURCE
#ע�������˳����Ҫ��ִ��SUBDIRS��������DEBUG

all:$(SUBDIRS) $(CUR_OBJS) DEBUG
#�ݹ�ִ����Ŀ¼�µ�makefile�ļ������ǵݹ�ִ�еĹؼ�
$(SUBDIRS):ECHO
	make -C $@
DEBUG:ECHO
#ֱ��ȥdebugĿ¼��ִ��makefile�ļ�
	make -C debug
ECHO:
	@echo $(SUBDIRS)
#��c�ļ�����Ϊo�ļ���������ָ������Ŀ���ļ���Ŀ¼�м�OBJS_DIR
$(CUR_OBJS):%.o:%.c
	$(CC) -c $^ $(CFLAGS) -o $(ROOT_DIR)/$(OBJS_DIR)/$@

%.d:%.c
	@set -e; rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(OBJS:.o=.d)

clean:
	@rm -rf $(OBJS_DIR)/*
	@rm -rf $(BIN_DIR)/*
	@rm -rf $(LIB_DIR)/*