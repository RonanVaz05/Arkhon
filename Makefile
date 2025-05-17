CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS =

# Source files
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Object files
OBJDIR = obj
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Executable name
TARGET = arkhons_awakening

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET) savegame.dat

# Example run command (optional)
run:
	./$(TARGET)
