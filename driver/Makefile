# C Compiler
CXX=gcc

# Important directories, relative to this Makefile
SRCDIR=src
INCDIR=include
BUILDDIR=build
OBJDIR=$(BUILDDIR)/obj
DEPDIR=$(BUILDDIR)/dep

# Main Target object/name
TARGET=$(BUILDDIR)/mbdriver

# Raspberry Pi name and paths
RPI_RGB_LIB_NAME=rgbmatrix
RPI_RGB_LIB_DIR=rpi-rgb-led-matrix
RPI_RGB_LIB_FULLPATH=$(RPI_RGB_LIB_DIR)/lib/lib$(RPI_RGB_LIB_NAME).a

# Compile flags
CLIBS= \
	-L$(RPI_RGB_LIB_DIR)/lib \
	-l$(RPI_RGB_LIB_NAME) \
	-lpthread -lrt -lm -lpthread

CINCLUDES= \
	-I$(INCDIR) \
	-I$(RPI_RGB_LIB_DIR)/include

CFLAGS= \
	$(CINCLUDES) \
	-O3 -fno-exceptions -std=c17 -lstdc++

# Sources and intermediate files
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(subst $(SRCDIR),$(OBJDIR),$(SRCS:.c=.o))
DEPS=$(subst $(SRCDIR),$(DEPDIR),$(SRCS:.c=.d))

# Main output
$(TARGET): $(RPI_RGB_LIB_FULLPATH) $(OBJS)
	$(CXX) -o $@ $^ $(CLIBS) $(CFLAGS)
	$(info Build succeeded!)

# Raspberry Pi lib (statically linked)
$(RPI_RGB_LIB_FULLPATH): 
	$(info Building the RPi RGB Matrix lib for the first time...)
	$(MAKE) -C $(RPI_RGB_LIB_DIR)/lib

# Intermediate dependencies
$(DEPS): $(RPI_RGB_LIB_FULLPATH) $(subst $(DEPDIR),$(SRCDIR),$(@:.d=.c))
	$(info Generating dependency file '$@'...)
	mkdir -p $(@D)
	cpp $(CFLAGS) $(subst $(DEPDIR),$(SRCDIR),$(@:.d=.c)) -MM -MT $(subst $(DEPDIR),$(OBJDIR),$(@:.d=.o)) > $@
	@echo '	@mkdir -p $$(@D)' >> $@
	@echo '	$$(CXX) -c -o $$@ $$(subst $$(OBJDIR),$$(SRCDIR),$$(@:.o=.c)) $$(CFLAGS)' >> $@

-include $(DEPS)

clean:
	rm -rf build