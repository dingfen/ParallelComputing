CC=gcc
OPENMP=-fopenmp
SOURCES:=$(shell find $($(shell pwd)) -name '*.c')
OBJS=$(SOURCES:%.c=%)


all : $(OBJS)
	@echo "编译中..."
	@echo $(SOURCES)
	@echo "编译完成！"
	

%: %.c
	$(CC) $(OPENMP) $< -o $@

.PHONY: clean
clean: 
	rm  $(OBJS)