include CONFIG.cfg

CC = gcc
LD = gcc
TARGETS = $(BUILD_DIR)/$(NAME)
OBJ = $(BUILD_DIR)/main.o  
LOG = $(patsubst $(TEST_DIR)/%.in, $(BUILD_DIR)/%.log, $(wildcard $(TEST_DIR)/*.in))
ERR = $(BUILD_DIR)/err.err

.PHONY: all check clean

all: $(TARGETS)

$(TARGETS) : $(OBJ) | $(BUILD_DIR)
	$(LD) $^ -o $@

$(OBJ): $(BUILD_DIR)/%.o: $(SOURCE_DIR)/*.c | $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR):	
	@mkdir -p $@ 

check: $(LOG)
	@if [ -f $(ERR) ]; then \
		$(RM) $(ERR); \
		exit 1; \
	fi
	
$(LOG): $(BUILD_DIR)/%.log: $(TEST_DIR)/%.in $(TEST_DIR)/%.out $(TARGETS)
	@$(TARGETS) $< >$@
	@if cmp -s $(TEST_DIR)/$*.out $@; then \
		echo Test $* - was successful; \
	else \
		echo Test $* - was failed; \
		touch $(ERR); \
	fi

clean:
	$(RM) $(OBJ) $(LOG) $(TARGETS) $(ERR)