# Makefile

# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -I include -Wall -Wextra -O2 -mwindows -m64

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
TARGETDIR = target

# Target Executable
TARGET = $(TARGETDIR)/main.exe

# Automatically find all .c files in SRCDIR
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Convert source file paths to object file paths
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Default Rule
all: $(TARGET)

# Link Object Files to Create Executable
$(TARGET): $(OBJECTS) | $(TARGETDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) -ldwmapi -luser32 -lgdi32 -lkernel32

# Compile Source Files into Object Files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create Object Directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Create Target Directory
$(TARGETDIR):
	mkdir -p $(TARGETDIR)

# Clean Build Artifacts
clean:
	rm -rf $(OBJDIR) $(TARGETDIR)

