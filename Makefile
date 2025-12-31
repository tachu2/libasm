NAME				:= libasm.a
EXEC				:= a.out

SRCS_DIR			:= src
OBJS_DIR			:= obj
ROOT_DIR			:= $(shell pwd)
TEST_DIR			:= test
BUILD_DIR			:= $(TEST_DIR)/build

AS := nasm
ASFLAGS := -f elf64
AR := ar rcs
LD := ld
RM				:= rm -rf

SRC_EXT := s
VPATH := $(SRCS_DIR)

SRCS := $(wildcard $(addsuffix /*.$(SRC_EXT), $(VPATH)))
OBJS := $(patsubst $(SRCS_DIR)/%.$(SRC_EXT),$(OBJS_DIR)/%.o,$(SRCS))
INCLUDES := -I$(SRCS_DIR)

CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17
TEST_EXEC := $(TEST_DIR)/test.out
TEST_MAIN := $(TEST_DIR)/main.cpp
TEST_SRCS := $(wildcard $(TEST_DIR)/test_*.cpp)

all: $(OBJS_DIR) $(NAME) $(EXEC)

$(NAME): $(OBJS)
	$(AR) $@ $^

$(EXEC): $(OBJS_DIR)/main.o $(NAME)
	$(LD) -o $@ $^

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.$(SRC_EXT)
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJS_DIR)/main.o: main.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJS_DIR):
	mkdir -p $@

clean:
	$(RM) $(OBJS_DIR) $(BUILD_DIR) $(TEST_EXEC)

fclean: clean
	$(RM) $(NAME)

re: fclean all


test: $(NAME) $(TEST_EXEC)
	@$(TEST_EXEC)

$(TEST_EXEC): $(TEST_MAIN) $(TEST_SRCS) $(NAME)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_MAIN) $(TEST_SRCS) -L. -lasm

.PHONY: all clean fclean re test debug
