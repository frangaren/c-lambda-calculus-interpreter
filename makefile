
# Global settings

CC=gcc
CFLAGS=-c -g -Wall -I $(HDR_DIR)
LDFLAGS=-g
DEPFLAGS=-I $(HDR_DIR)


# Main binary settings

SRC_DIR=src
HDR_DIR=hdr
BIN_DIR=bin
TST_DIR=test
OBJ_DIR=obj
DEP_DIR=dep

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(notdir $(SRCS)))))
LOBJS=$(addprefix $(OBJ_DIR)/,$(addsuffix .l.o,$(basename $(notdir $(SRCS)))))
DEPS=$(addprefix $(DEP_DIR)/,$(addsuffix .d,$(basename $(notdir $(SRCS)))))
NAME=lambda
TARGETS=$(BIN_DIR)/lib$(NAME).a $(BIN_DIR)/$(NAME)


# Test settings

T_SRC_DIR=test/src
T_BIN_DIR=test/bin
T_OBJ_DIR=test/obj
T_DEP_DIR=test/dep

T_SRCS=$(wildcard $(T_SRC_DIR)/*.c)
T_OBJS=$(addprefix $(T_OBJ_DIR)/,$(addsuffix .o,\
	$(basename $(notdir $(T_SRCS)))))
T_DEPS=$(addprefix $(T_DEP_DIR)/,$(addsuffix .d,\
	$(basename $(notdir $(T_SRCS)))))
T_TARGETS=$(addprefix $(T_BIN_DIR)/,$(addsuffix .test,\
	$(basename $(notdir $(T_SRCS)))))


# Phony rules

.PHONY: default
default: all

.PHONY: all
all: release run-tests

.PHONY: run-tests
run-tests: build-tests
	@printf 'Running tests: \n'
	@for test_exec in $(T_TARGETS); do\
		printf "\tTest \"$$test_exec\"...\n";\
		./$$test_exec &&\
			printf '\t\tSuccess\n' ||\
			printf '\t\tFailed\n';\
	done
	@echo 'Done'

.PHONY: build-tests
build-tests: $(T_TARGETS)

.PHONY: release
release: $(TARGETS)

.PHONY: clean
clean:
	@printf 'Cleaning...\t'
	@rm -rf $(OBJ_DIR)
	@rm -rf $(DEP_DIR)
	@rm -rf $(BIN_DIR)
	@rm -rf $(T_OBJ_DIR)
	@rm -rf $(T_DEP_DIR)
	@rm -rf $(T_BIN_DIR)
	@echo 'Done'

.PHONY: init-structure
init-structure:
	@printf 'Creating working structure...\t'
	@mkdir -p $(SRC_DIR)
	@mkdir -p $(HDR_DIR)
	@mkdir -p $(TST_DIR)
	@mkdir -p $(T_SRC_DIR)
	@echo 'Done'

# Binary building rules

$(DEP_DIR)/%.d: $(SRC_DIR)/%.c
	@mkdir -p $(DEP_DIR)
	@$(CC) $(DEPFLAGS) -MM $< -MT \
		$(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(notdir $<)))) > $@

-include $(DEPS)

$(BIN_DIR)/lib$(NAME).a: $(LOBJS)
	@printf 'Building $@ from $^...\t'
	@mkdir -p $(BIN_DIR)
	@ar rcs $@ $^
	@echo 'Done'

$(BIN_DIR)/$(NAME): $(OBJS)
	@printf 'Building $@ from $^...\t'
	@mkdir -p $(BIN_DIR)
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo 'Done'

$(OBJ_DIR)/%.l.o: $(SRC_DIR)/%.c
	@printf 'Building $@ from $<...\t'
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -DLIB -o $@ $<
	@echo 'Done'

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@printf 'Building $@ from $<...\t'
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -DEXE -o $@ $<
	@echo 'Done'

# Test building rules

$(T_DEP_DIR)/%.d: $(T_SRC_DIR)/%.c
	@mkdir -p $(T_DEP_DIR)
	@$(CC) $(DEPFLAGS) -MM $< -MT \
		$(addprefix $(T_OBJ_DIR)/,$(addsuffix .o,$(basename $(notdir $<)))) > $@

-include $(T_DEPS)

$(T_OBJ_DIR)/%.o: $(T_SRC_DIR)/%.c
	@printf 'Building $@ from $<...\t'
	@mkdir -p $(T_OBJ_DIR)
	@$(CC) $(CFLAGS) -DTEST -o $@ $<
	@echo 'Done'

$(T_BIN_DIR)/%.test: $(T_OBJ_DIR)/%.o $(T_OBJ_DIR)/lib$(NAME).a
	@printf 'Building $@ from $<...\t'
	@mkdir -p $(T_BIN_DIR)
	@$(CC) $(LDFLAGS) -o $@ $< -L$(T_OBJ_DIR) -l$(NAME)
	@echo 'Done'

$(T_OBJ_DIR)/lib$(NAME).a: $(BIN_DIR)/lib$(NAME).a
	@mkdir -p $(T_OBJ_DIR)
	@printf 'Copying $@ from $^...\t'
	@cp $< $@
	@echo 'Done'
