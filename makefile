# Executable name:
EXE = starfighters

# Compiler name, source file extension and compilation data (flags and libs):
CC = gcc
EXT = .c
CFLAGS = -Wall -g -I $(IDIR)
LIBS = -lm -lpthread

# Project paths:
IDIR = include
ODIR = src/obj
SDIR = src

# Project dependency files (header files):
_DEPS = name_generator.h

# Project object files:
_OBJ = main.o name_generator.o

# Project source files:
_SRC = main.c name_generator.c

# Joining file names with their respective paths:
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
SRC = $(patsubst %,$(SDIR)/%,$(_SRC))

# Object files compilation rule:
$(ODIR)/%.o: $(SDIR)/%$(EXT) $(DEPS)
	@if [ ! -d $(ODIR) ]; then mkdir $(ODIR); fi
	$(CC) -c -o $@ $< $(CFLAGS)

# Project executable compilation rule:
$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# List of aditional makefile commands:
.PHONY: clean
.PHONY: structure
.PHONY: verification

# Command to clean object files and project executable:
clean:
	@rm -f $(ODIR)/*.o *~ core
	@if [ -f $(EXE) ]; then rm $(EXE) -i; fi

# Command to create the basic project structure:
structure:

	# Create project folders:
	mkdir -p include
	mkdir -p src
	mkdir -p src/obj

	# Move project files to their respective folders:
	if [ -f *.h ]; then mv *.h $(IDIR); fi
	if [ -f *$(EXT) ]; then mv *$(EXT) $(SDIR); fi
	if [ -f *.o ]; then mv *.o $(ODIR); fi

# Command to verify the source code using cppcheck and valgrind:
verification:
	cppcheck $(SRC) ./$(EXE) --enable=all
	valgrind --leak-check=full ./$(EXE)
