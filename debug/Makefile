ALL_SOURCE = $(shell find ../ -type f -name *.c)
OBJS=*.o

#将对应的c文件名转为o文件后放在下面的CUR_OBJS变量中
CUR_OBJS=${patsubst %.c, %.o, $(ALL_SOURCE)}

all:clean $(ROOT_DIR)/$(LIB_DIR)/lib${BIN}.a $(ROOT_DIR)/$(LIB_DIR)/lib${BIN}.so

#$(ROOT_DIR)/$(BIN_DIR)/$(BIN):$(ODIR)/$(OBJS)
#	$(CC) -o $@ $<

$(ROOT_DIR)/$(LIB_DIR)/lib${BIN}.a:
	ar -cr $@ $(ROOT_DIR)/$(OBJS_DIR)/$(OBJS)

$(ROOT_DIR)/$(LIB_DIR)/lib${BIN}.so:$(ALL_SOURCE)
	$(CC) -shared -fPIC -o $@ $^ $(CFLAGS)

clean:
	@rm -rf $(ROOT_DIR)/$(LIB_DIR)/lib${BIN}.a
	@rm -rf $(ROOT_DIR)/$(LIB_DIR)/lib${BIN}.so