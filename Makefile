BINDIR	= bin/
OBJDIR	= obj/
SRCDIR	= src/

_BIN	= kernel
BIN	= $(addprefix $(BINDIR), $(_BIN))

C_SRC	= $(wildcard $(SRCDIR)*.c)
S_SRC	= $(wildcard $(SRCDIR)*.S)
SRC = $(C_SRC) $(S_SRC)

_C_OBJS	= $(addsuffix .o,$(basename $(notdir $(C_SRC))))
_S_OBJS	= $(addsuffix .o,$(basename $(notdir $(S_SRC))))
C_OBJS	= $(addprefix $(OBJDIR), $(_C_OBJS))
S_OBJS	= $(addprefix $(OBJDIR), $(_S_OBJS))
OBJS	= $(C_OBJS) $(S_OBJS)

CC = gcc
LD = ld

ASFLAGS = -m32
CFLAGS = -m32 -Wall -g -fno-stack-protector -nostdinc
LDFLAGS = -melf_i386 -Ttext=0x100000

$(BIN): $(BINDIR) $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(BIN)

$(BINDIR):
	mkdir -p $(BINDIR)

$(C_OBJS): $(OBJDIR) $(C_SRC)
	$(CC) $(CFLAGS) -c $(C_SRC) -o $(C_OBJS)

$(S_OBJS): $(OBJDIR) $(S_SRC)
	$(CC) $(ASFLAGS) -c $(S_SRC) -o $(S_OBJS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

start:
	qemu-system-i386 -kernel $(BIN)

clean:
	rm -rf $(BINDIR) $(OBJDIR)

.PHONY: all
