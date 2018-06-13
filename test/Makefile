#debug文件夹里的makefile文件需要最后执行，所以这里需要执行的子目录要排除debug文件夹，这里使用awk排除了debug文件夹，读取剩下的文件夹
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "debug" && $$9 != "include") print $$9}')

CUR_DIR=$(shell pwd)
#以下同根目录下的makefile的相同代码的解释
CUR_SOURCE=${wildcard *.cpp}
CUR_OBJS=${patsubst %.cpp, %.o, $(CUR_SOURCE)}

#注意这里的顺序，需要先执行SUBDIRS最后才能是DEBUG
all:$(SUBDIRS) $(CUR_OBJS) DEBUG

$(SUBDIRS):ECHO
	make -C $@

DEBUG:ECHO
#直接去debug目录下执行makefile文件
	make -C debug

$(CUR_OBJS):%.o:%.cpp
	$(CPP) -o $@ -c $< $(CPPFLAGS) $(CXXFLAGS)
	@cp $@ $(ROOT_DIR)/test/$(CUR_OBJS_DIR)/$@

%.d:%.cpp
	@set -e; rm -f $@; $(CPP) -MM $< $(CPPINCLUDEFLAGS) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(CUR_OBJS:.o=.d)

ECHO:
	@echo $(SUBDIRS)

