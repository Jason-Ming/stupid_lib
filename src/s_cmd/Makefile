#子目录的Makefile直接读取其子目录就行
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "include")print $$9}')
#以下同根目录下的makefile的相同代码的解释
CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

all:$(SUBDIRS) $(CUR_OBJS)

$(SUBDIRS):ECHO
	cp Makefile ./$@
	make -C $@

$(CUR_OBJS):%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)
	cp $@ $(ROOT_DIR)/$(CUR_OBJS_DIR)/$@

%.d:%.c
	@set -e; rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(CUR_OBJS:.o=.d)

ECHO:
	@echo $(SUBDIRS)