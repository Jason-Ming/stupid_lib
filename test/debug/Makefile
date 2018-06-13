OBJS=*.o
#ALL_C_O = $(shell find $(ROOT_DIR)/$(OBJS_DIR) -type f -name '*.o' | grep  -v "main.o")
ALL_OBJS = $(ROOT_DIR)/test/$(CUR_OBJS_DIR)/$(OBJS)
#ALL_OBJS += $(ALL_C_O)

all: clean $(ROOT_DIR)/test/$(CUR_BIN_DIR)/$(CUR_BIN_TEST)
clean:
	@rm -rf $(ROOT_DIR)/test/$(CUR_BIN_DIR)/$(CUR_BIN_TEST)

$(ROOT_DIR)/test/$(CUR_BIN_DIR)/$(CUR_BIN_TEST):
	$(CPP) -o $@ $(ALL_OBJS) $(CXXLDFLAGS)

#	$(CC) -o $@ /home/jason/github/The_C_Programming_Language/chapter_1/src/exercise/exercise_1_19.o $(LDFLAGS)
#$(ROOT_DIR)/$(CUR_LIB_DIR)/lib${BIN}.a:
#	ar -cr $@ $(ROOT_DIR)/$(OBJS_DIR)/$(OBJS)

#$(ROOT_DIR)/$(CUR_LIB_DIR)/lib${BIN}.so:$(ALL_SOURCE)
#	$(CC) -shared -fPIC -o $@ $^ $(CFLAGS)

