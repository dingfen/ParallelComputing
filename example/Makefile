CC=gcc
OPENMP=-fopenmp
SOURCES:=$(shell find $(.) -name '*.c')
OBJS=$(SOURCES:%.c=%)


all : $(OBJS)
	@echo $(SOURCES)

%: %.c
	$(CC) $(OPENMP) $< -o $@

.PHONY: clean
clean: 
	rm  $(OBJS)