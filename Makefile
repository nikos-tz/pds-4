CC = gcc
CILKCC = /usr/local/OpenCilk-9.0.1-Linux/bin/clang
MPICC = /usr/bin/mpicc


BUILD_DIR := ./build
SRC_DIRS := ./src

GREEN = \033[1;32m
RED = \033[1;31m
NC = \033[0m

LIBRARIES_SRC := $(shell find $(SRC_DIRS)/library -name '*.c')
LIBRARIES_SRC := $(LIBRARIES_SRC:%=$(BUILD_DIR)/%.o)

SEQUENTIAL_SRC := $(shell find $(SRC_DIRS)/sequential -name '*.c')
SEQUENTIAL_SRC := $(SEQUENTIAL_SRC:%=$(BUILD_DIR)/%.o)

PARALLEL_SRC := $(shell find $(SRC_DIRS)/parallel -name '*.c')
PARALLEL_SRC := $(PARALLEL_SRC:%=$(BUILD_DIR)/%.o)

THRESHOLD_PARALLEL_SRC := $(shell find $(SRC_DIRS)/threshold_parallel -name '*.c') $(SRC_DIRS)/parallel/utils.c $(SRC_DIRS)/sequential/vptree.c
THRESHOLD_PARALLEL_SRC := $(THRESHOLD_PARALLEL_SRC:%=$(BUILD_DIR)/%.o)

KNN_SRC := $(shell find $(SRC_DIRS)/knn -name '*.c') $(SRC_DIRS)/parallel/utils.c $(SRC_DIRS)/parallel/vptree.c
KNN_SRC := $(KNN_SRC:%=$(BUILD_DIR)/%.o)

PARALLEL_KNN_SRC := $(shell find $(SRC_DIRS)/parallel_knn -name '*.c') $(SRC_DIRS)/sequential/vptree.c
PARALLEL_KNN_SRC := $(PARALLEL_KNN_SRC:%=$(BUILD_DIR)/%.o)


INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := $(INC_FLAGS) -O3 -w

all: pre_build build_sequential build_parallel build_threshold_parallel build_knn build_parallel_knn post_build


pre_build:
	@echo "$(GREEN)Building all...$(NC)"

post_build:
	@echo "$(GREEN)Build all successful!!$(NC)"


build_sequential: $(SEQUENTIAL_SRC) $(LIBRARIES_SRC)
	@echo "    $(GREEN)Building sequential binary...$(NC)"
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/sequential.out $(SEQUENTIAL_SRC) $(LIBRARIES_SRC)
	@echo "    $(GREEN)Build finished successfully!$(NC)"
	@echo



build_parallel: $(PARALLEL_SRC) $(LIBRARIES_SRC)
	@echo "    $(GREEN)Building parallel binary...$(NC)"
	@$(CILKCC) $(CFLAGS) -o $(BUILD_DIR)/parallel.out $(PARALLEL_SRC) $(LIBRARIES_SRC) -fcilkplus
	@echo "    $(GREEN)Build finished successfully!$(NC)"
	@echo


build_threshold_parallel: $(THRESHOLD_PARALLEL_SRC) $(LIBRARIES_SRC)
	@echo "    $(GREEN)Building threshold_parallel binary...$(NC)"
	@$(CILKCC) $(CFLAGS) -o $(BUILD_DIR)/threshold_parallel.out $(THRESHOLD_PARALLEL_SRC) $(LIBRARIES_SRC) -fcilkplus
	@echo "    $(GREEN)Build finished successfully!$(NC)"
	@echo

build_knn: $(KNN_SRC) $(LIBRARIES_SRC)
	@echo "    $(GREEN)Building knn binary...$(NC)"
	@$(CILKCC) $(CFLAGS) -o $(BUILD_DIR)/knn.out $(KNN_SRC) $(LIBRARIES_SRC) -fcilkplus
	@echo "    $(GREEN)Build finished successfully!$(NC)"
	@echo

build_parallel_knn: $(PARALLEL_KNN_SRC) $(LIBRARIES_SRC)
	@echo "    $(GREEN)Building parallel knn binary...$(NC)"
	@$(MPICC) $(CFLAGS) -o $(BUILD_DIR)/parallel_knn.out $(PARALLEL_KNN_SRC) $(LIBRARIES_SRC)
	@echo "    $(GREEN)Build finished successfully!$(NC)"
	@echo


# parallel
$(BUILD_DIR)/$(SRC_DIRS)/parallel/%.c.o: $(SRC_DIRS)/parallel/%.c
	@mkdir -p $(dir $@)
	@$(CILKCC) $(CFLAGS) -c $< -o $@ -fcilkplus


$(BUILD_DIR)/$(SRC_DIRS)/parallel_main.c.o: $(SRC_DIRS)/parallel_main.c
	@mkdir -p $(dir $@)
	@$(CILKCC) $(CFLAGS) -c $< -o $@ -fcilkplus

# threshold parallel
$(BUILD_DIR)/$(SRC_DIRS)/threshold_parallel/%.c.o: $(SRC_DIRS)/threshold_parallel/%.c $(SRC_DIRS)/parallel/utils.c $(SRC_DIRS)/sequential/vptree.c
	@mkdir -p $(dir $@)
	@$(CILKCC) $(CFLAGS) -c $< -o $@ -fcilkplus


$(BUILD_DIR)/$(SRC_DIRS)/threshold_parallel_main.c.o: $(SRC_DIRS)/threshold_parallel_main.c
	@mkdir -p $(dir $@)
	@$(CILKCC) $(CFLAGS) -c $< -o $@ -fcilkplus

# knn
$(BUILD_DIR)/$(SRC_DIRS)/knn/%.c.o: $(SRC_DIRS)/knn/%.c $(SRC_DIRS)/parallel/utils.c $(SRC_DIRS)/parallel/vptree.c
	@mkdir -p $(dir $@)
	@$(CILKCC) $(CFLAGS) -c $< -o $@ -fcilkplus


$(BUILD_DIR)/$(SRC_DIRS)/knn_main.c.o: $(SRC_DIRS)/knn_main.c
	@mkdir -p $(dir $@)
	@$(CILKCC) $(CFLAGS) -c $< -o $@ -fcilkplus


$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@


run_sequential: build_sequential
	@echo run forrest run



.PHONY: clean
clean:
	@echo "$(RED)Clearing build directory...$(NC)"
	@rm -rf $(BUILD_DIR)
	@echo "$(GREEN)Done!$(NC)"