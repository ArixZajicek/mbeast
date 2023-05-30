# C Compiler
CC=gcc

# Important directories, relative to this Makefile
SRCDIR=src
INCDIR=src
BUILDDIR=build
OBJDIR=$(BUILDDIR)/obj
DEPDIR=$(BUILDDIR)/dep

# Main Target object/name
TARGET=$(BUILDDIR)/beast

# Raspberry Pi name and paths
RPI_RGB_LIB_NAME=rgbmatrix
RPI_RGB_LIB_DIR=lib/rpi-rgb-led-matrix
RPI_RGB_LIB_FULLPATH=$(RPI_RGB_LIB_DIR)/lib/lib$(RPI_RGB_LIB_NAME).a

# Compile flags
CLIBS=-lSDL2 -L$(RPI_RGB_LIB_DIR)/lib -l$(RPI_RGB_LIB_NAME)
CINCLUDES=-I$(RPI_RGB_LIB_DIR)/include -I$(INCDIR)
CFLAGS=$(CLIBS) -fPIC -O3 $(CINCLUDES)

# Sources and intermediate files
SRCS=$(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)
OBJS=$(subst $(SRCDIR),$(OBJDIR),$(SRCS:.cpp=.o))
DEPS=$(subst $(SRCDIR),$(DEPDIR),$(SRCS:.cpp=.d))

# Main output
$(TARGET): $(RPI_RGB_LIB_FULLPATH) $(OBJS)
	@echo Building final target $(TARGET)...
  @$(CC) -o $@ $^ $(CFLAGS)
	@echo Done.

# Raspberry Pi Lib
$(RPI_RGB_LIB_FULLPATH): 
	$(MAKE) -C $(RPI_RGB_LIB_DIR)/lib

# Intermediate dependencies
$(DEPS): $(RPI_RGB_LIB_FULLPATH) $(subst $(DEPDIR),$(SRCDIR),$(@:.d=.cpp))
	@echo Generating dependency file '$@'...
	@mkdir -p $(@D)
	cpp $(CFLAGS) $(subst $(DEPDIR),$(SRCDIR),$(@:.d=.cpp)) -MM -MT $(subst $(DEPDIR),$(OBJDIR),$(@:.d=.o)) > $@
	@echo '	@echo Building $$@...' >> $@
	@echo '	@mkdir -p $$(@D)' >> $@
	@echo '	@$$(CC) -c -o $$@ $$(subst $$(OBJDIR),$$(SRCDIR),$$(@:.o=.cpp)) $$(CFLAGS)' >> $@

-include $(DEPS)

.PHONY: clean
clean:
	@echo Removing $(OBJDIR), $(DEPDIR), and $(TARGET)...
	rm -fr ./temp/ $(TARGET) 