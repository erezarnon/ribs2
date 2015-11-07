TARGET=ds_code_gen
SRC=ds_code_gen.c
CFLAGS+= -I ../include
GEN_OUTPUT=../include/index_gen.h

buildall: all $(GEN_OUTPUT)

include ../make/ribs.mk

$(GEN_OUTPUT): $(TARGET_FILE)
	@echo "$(TARGET_FILE) ../include"
	$(shell $(TARGET_FILE) ../include index_gen.h index_container.h)
