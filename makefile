LIB_SRC=$(shell find lib -name '*.c')
CHALLS_SRC=$(shell find src -name '*.c')
LIB_OBJS=0xdeadbabe

all: lib challs

challs: $(CHALLS_OBJS) dirs
	$(foreach file,$(CHALLS_SRC),$(CC) -o bin/$(subst src/,,$(basename $(file))) $(file) $(LIB_OBJS);)

lib: $(LIB_SRC) dirs
	$(foreach file,$(LIB_SRC),$(CC) -c -o bin/$(basename $(file)).o $(file);)
	$(eval override LIB_OBJS=$(shell find bin/lib -name '*.o'))

dirs:
	mkdir -p bin
	mkdir -p bin/lib
	mkdir -p bin/s1
	mkdir -p bin/s2
	mkdir -p bin/s3
	mkdir -p bin/s4
	mkdir -p bin/s5
	mkdir -p bin/s6
	mkdir -p bin/s7
	mkdir -p bin/s8
