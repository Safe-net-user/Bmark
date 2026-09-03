NAME		:= libbmark.a

# ------------------- CONSTANTS ------------------- #
CC			:= cc
AR			:= ar
ARFLAGS		:= rcs
RMF			:= rm -f
RMRF		:= rm -rf
MKDIRP		:= mkdir -p

# ------------------- SHELL CMDS ------------------ #
CC_VERSION		:= $(shell $(CC) --version)

# --------------- MAIN DIRECTORIES ---------------- #
OBJ			:= obj
CORE		:= core
ASM			:= asm
TEST		:= test
# ---------------- SUB_DIRECTORIES ---------------- #
HDR			:= include/bmark
SRC			:= src

# ------------------- COMPILER -------------------- #
ifeq ($(findstring clang,$(CC_VERSION)),clang)
    COMPILER := clang
else
ifeq ($(findstring GCC,$(CC_VERSION)),GCC)
    COMPILER := gcc
else
    $(error Unsupported compiler. Use clang or gcc)
endif
endif

# ---------------------- MODE -------------------- #
MODE				?= release

# --------------------- FLAGS -------------------- #
ifeq ($(MODE),release)
	CFLAGS			?= -Wall -Werror -Wextra
else
	ifeq ($(COMPILER),clang)
		CFLAGS		?= 	-Wall -Werror -Wextra -Wvla -Wno-pedantic -Wmisleading-indentation -Wshadow -Wnull-dereference -fshort-enums
	else ifeq ($(COMPILER),gcc)
		CFLAGS		?= 	-Wall -Werror -Wextra -Wvla -Wno-pedantic -Wmisleading-indentation -Wstrict-aliasing=3 -Wduplicated-cond -Wstringop-overflow -Wshadow -Wnull-dereference -Warray-bounds -Wrestrict
	endif
endif

ifeq ($(MODE),debug)
	CFLAGS += -g -O1
else ifeq ($(MODE),debug_memory)
	CFLAGS += -g -O1 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
else ifeq ($(MODE),release)
	CFLAGS += -O3 -march=native
endif

CPPFLAGS		:= -I$(HDR)
LDLIBS			:= -lm

# --------------------- FILES --------------------- #
SOURCES			:= $(CORE)/$(SRC)/microbench.c

# -------------------- OBJECTS -------------------- #
OBJECTS			:= $(patsubst $(CORE)/$(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

# ---------------- RULES -------------- #
all: $(NAME)

$(NAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ)/%.o: $(CORE)/$(SRC)/%.c
	$(MKDIRP) $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

# ------------------ CLEAN UP RULES --------------- #
clean:
	$(RMRF) $(OBJ)

fclean: clean
	$(RMF) $(NAME)

re: fclean all

.PHONY: all clean fclean re

