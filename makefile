LIB_SRC=$(shell find lib -name '*.c')
CHALLS_SRC=$(shell find src -name '*.c')
LIB_OBJS=0xdeadbabe

all: lib challs

lib: $(LIB_SRC) dirs
	$(foreach file,$(LIB_SRC),$(CC) -c -shared \
	-o bin/lib/lib$(notdir $(subst .c,, $(file))).so $(file);)


lib_objs: lib dirs
	$(eval override LIB_OBJS=$(shell find bin/lib -name '*.so'))
	find bin/lib -name '*.so'
	echo $(LIB_OBJS)

challs: $(CHALLS_SRC) dirs lib lib_objs
	$(foreach file,$(CHALLS_SRC),\
	$(CC) -o \
	bin/$(subst src/,,$(basename $(file))) $(file) \
	-Ilib/\
	-Lbin/lib\
	$(foreach lib_name,$(LIB_OBJS),\
	-l$(subst lib,,$(subst .so,,$(notdir $(lib_name)))));)

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

clean:
	rm -rf bin
