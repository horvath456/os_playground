BINDIR	= bin/
OBJDIR	= obj/
SRCDIR	= src/

_BIN	= kernel
BIN	= $(addprefix $(BINDIR), $(_BIN))

C_SRC	= $(wildcard $(SRCDIR)*.c)
S_SRC	= $(wildcard $(SRCDIR)*.S)
ASM_SRC = $(wildcard $(SRCDIR)*.asm)
SRC = $(C_SRC) $(S_SRC) $(ASM_SRC)

_C_OBJS	    = $(addsuffix .o,$(basename $(notdir $(C_SRC))))
_S_OBJS	    = $(addsuffix .o,$(basename $(notdir $(S_SRC))))
_ASM_OBJS	= $(addsuffix .o,$(basename $(notdir $(ASM_SRC))))
C_OBJS   	= $(addprefix $(OBJDIR), $(_C_OBJS))
S_OBJS	    = $(addprefix $(OBJDIR), $(_S_OBJS))
ASM_OBJS	= $(addprefix $(OBJDIR), $(_ASM_OBJS))
OBJS	= $(C_OBJS) $(S_OBJS) $(ASM_OBJS)

CC = gcc
LD = ld
NASM = nasm

ASFLAGS = -m32
NASMFLAGS = -felf
CFLAGS = -m32 -Wall -g -fno-stack-protector -nostdinc
LDFLAGS = -melf_i386 -Tkernel.ld

$(BIN): $(BINDIR) $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(BIN)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(OBJDIR)%.o: $(SRCDIR)%.S
	$(CC) $(ASFLAGS) -c -o $@ $^

$(OBJDIR)%.o: $(SRCDIR)%.asm
	$(NASM) $(NASMFLAGS) -o $@ $^

start:
	qemu-system-i386 -kernel $(BIN)

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*

.PHONY: all
