CC := cc
CCFLAGS := -std=c11 -O0 -g -Wall -Wextra -pedantic -I/usr/local/include
LDFLAGS := -L/usr/local/lib -lSDL2 -FOpenGL
SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
TARGET := game3

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $<

# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $< -o $@ $(CFLAGS)

clean:
	rm -rf $(TARGET) $(OBJECTS)
