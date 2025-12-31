NAME				:= libasm.a
EXEC				:= a.out

SRCS_DIR			:= src
BONUS_DIR			:= $(SRCS_DIR)/bonus
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
BONUS_SRCS := $(wildcard $(BONUS_DIR)/*_bonus.$(SRC_EXT))

OBJS := $(patsubst $(SRCS_DIR)/%.$(SRC_EXT),$(OBJS_DIR)/%.o,$(SRCS))
BONUS_OBJS := $(patsubst $(BONUS_DIR)/%.$(SRC_EXT),$(OBJS_DIR)/bonus/%.o,$(BONUS_SRCS))
INCLUDES := -I$(SRCS_DIR)

# ボーナスビルドかどうかを制御する変数
WITH_BONUS := 0

CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17
TEST_EXEC := $(TEST_DIR)/test.out
TEST_MAIN := $(TEST_DIR)/main.cpp
TEST_SRCS := $(wildcard $(TEST_DIR)/test_*.cpp)

all: $(OBJS_DIR) $(NAME)

bonus:
	@$(MAKE) WITH_BONUS=1 $(NAME)

$(NAME): $(OBJS) $(if $(filter 1,$(WITH_BONUS)),$(BONUS_OBJS))
	$(AR) $@ $^

$(EXEC): $(OBJS_DIR)/main.o $(NAME)
	$(LD) -o $@ $^ -lc --dynamic-linker /lib64/ld-linux-x86-64.so.2

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.$(SRC_EXT)
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJS_DIR)/bonus/%.o: $(BONUS_DIR)/%.$(SRC_EXT)
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
	$(RM) $(NAME) $(EXEC)

re: fclean all

test: bonus $(TEST_EXEC)
	@$(TEST_EXEC)

$(TEST_EXEC): $(TEST_MAIN) $(TEST_SRCS) $(NAME)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_MAIN) $(TEST_SRCS) -L. -lasm

.PHONY: all bonus clean fclean re test debug
