ALL_SOURCE = $(shell find ../ -type f -name *.c)
OBJS=*.o
all:$(ROOT_DIR)/$(LIB_DIR)/lib${BIN}.a $(ROOT_DIR)/$(LIB_DIR)/lib${BIN}.so

#$(ROOT_DIR)/$(BIN_DIR)/$(BIN):$(ODIR)/$(OBJS)
#	$(CC) -o $@ $^

$(ROOT_DIR)/$(LIB_DIR)/lib${BIN}.a:
	ar -cr $@ $(ROOT_DIR)/$(OBJS_DIR)/$(OBJS)

$(ROOT_DIR)/$(LIB_DIR)/lib${BIN}.so:
	$(CC) -shared -fPIC -o $@ $(ALL_SOURCE) $(CFLAGS)

%.d:%.c
	@set -e; rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(OBJS:.o=.d)