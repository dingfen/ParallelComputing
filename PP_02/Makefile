CC=mpicc
OPENMP=
SOURCES:=$(shell find $(.) -name '*.c')
LIB=-lm
OBJS=$(SOURCES:%.c=%)


all : $(OBJS)
	@echo $(SOURCES)
	@echo "编译完成"
	if [ ! -d "build" ]; then mkdir build; fi
	mv $(OBJS) build

%: %.c
	$(CC) $(OPENMP) $< $(LIB) -o $@

.PHONY: clean
clean: 
	rm -rf build