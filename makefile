CC				= gcc
CFLAGS		= -Wall
LDFLAGS		=
OBJFILES	= main.o cpu.o io.o
TARGET		= chip8
DEBUG     =

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(DEBUG) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

debug: DEBUG = -DDEBUG
debug: clean $(TARGET)

%o: %c
	$(CC) $(CFLAGS) $(DEBUG) -c $< $(LDFlAGS)
# all: library.cpp main.cpp
# In this case:
# $@ evaluates to all
# $< evaluates to library.cpp
# $^ evaluates to library.cpp main.cpp

clean:
	rm -f $(TARGET) $(OBJFILES) *.log

run: $(TARGET)
	./$(TARGET)
