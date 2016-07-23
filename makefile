CC				= gcc
CFLAGS		= -Wall
LDFLAGS		= -lncurses
TARGET		= chip8
DEBUG     =
BUILDDIR  = build
SRCDIR    = src
TESTDIR   = test
SRCFILES  = $(wildcard $(SRCDIR)/*.c)
OBJFILES  = $(patsubst $(SRCDIR)%.c,$(BUILDDIR)%.o,$(SRCFILES))
# TESTFILES = $(wildcard $(TESTDIR)/*.cc)

all: $(TARGET)

$(TARGET): make_build_dir $(OBJFILES) #make_build_dir $(OBJFILES)
	$(CC) $(CFLAGS) $(DEBUG) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

debug: DEBUG = -DDEBUG
debug: clean $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(DEBUG) -c $< $(LDFlAGS) -o $@
# Make Automatic Variables:
# EG: `all: library.cpp main.cpp`
# In this case:
# $@ evaluates to all
# $< evaluates to library.cpp
# $^ evaluates to library.cpp main.cpp

make_build_dir:
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(TARGET) $(BUILDDIR) *.log test/test

run: $(TARGET)
	./$(TARGET) roms/PONG


.PHONY: test
test:
	make -C $(TESTDIR)
