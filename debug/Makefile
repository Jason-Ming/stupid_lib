ALL_SOURCE = $(shell find ../ -type f -name *.c)

OBJS=*.o

CUR_OBJS = $(ROOT_DIR)/$(CUR_OBJS_DIR)/$(OBJS)
CUR_OBJS_TEST = $(ROOT_DIR)/$(CUR_OBJS_TEST_DIR)/$(OBJS)

all: clean $(ROOT_DIR)/$(CUR_LIB_DIR)/lib${CUR_LIB_BIN}.a $(ROOT_DIR)/$(CUR_LIB_DIR)/lib${CUR_LIB_BIN}.so $(ROOT_DIR)/$(CUR_LIB_DIR)/lib${CUR_LIB_BIN_TEST}.a

#$(ROOT_DIR)/$(CUR_BIN_DIR)/$(CUR_BIN):
#	$(CC) -o $@ $(CUR_OBJS) $(CLDFLAGS)

$(ROOT_DIR)/$(CUR_LIB_DIR)/lib${CUR_LIB_BIN}.a:
	ar -cr $@ $(CUR_OBJS)

$(ROOT_DIR)/$(CUR_LIB_DIR)/lib${CUR_LIB_BIN}.so:$(ALL_SOURCE)
	$(CC) -shared -fPIC -o $@ $^ $(CFLAGS) $(CLDFLAGS)

$(ROOT_DIR)/$(CUR_LIB_DIR)/lib${CUR_LIB_BIN_TEST}.a:
	ar -cr $@ $(CUR_OBJS_TEST)

clean:
	@rm -rf $(ROOT_DIR)/$(CUR_LIB_DIR)/lib${CUR_LIB_BIN}.a
	@rm -rf $(ROOT_DIR)/$(CUR_LIB_DIR)/lib${CUR_LIB_BIN}.so
	@rm -rf $(ROOT_DIR)/$(CUR_LIB_DIR)/lib${CUR_LIB_BIN_TEST}.a