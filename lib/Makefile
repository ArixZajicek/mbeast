# C Compiler
CC=gcc

# Important directories, relative to this Makefile
SRCDIR=src
INCDIR=src
OBJDIR=temp/obj
DEPDIR=temp/dep

# Main Target object/name
TARGET=libcostume.a

# Raspberry Pi name and paths
RPI_RGB_LIB_NAME=rgbmatrix
RPI_RGB_LIB_DIR=ext/rpi_rgb_led_matrix
RPI_RGB_LIB_FULLPATH=$(RPI_RGB_LIB_DIR)/lib/lib$(RPI_RGB_LIB_NAME).a

# Compile flags
CLIBS=-lSDL2 -L$(RPI_RGB_LIB_DIR)/lib -l$(RPI_RGB_LIB_NAME)
CINCLUDES=-I$(RPI_RGB_LIB_DIR)/include -I$(INCDIR)
CFLAGS=$(CLIBS) -fPIC -O3 $(CINCLUDES)

# Sources and intermediate files
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(subst $(SRCDIR),$(OBJDIR),$(SRCS:.c=.o))
DEPS=$(subst $(SRCDIR),$(DEPDIR),$(SRCS:.c=.d))

# Main library output
$(TARGET): $(RPI_RGB_LIB_FULLPATH) $(OBJS)
	@echo Building final target $(TARGET)...
# @$(CC) -o $@ $^ $(CFLAGS) -shared
	ar rcs $@ $^
	@echo Done.

# Raspberry Pi Lib
$(RPI_RGB_LIB_FULLPATH): 
	@mkdir -p ext
	@([ ! -d "$(RPI_RGB_LIB_DIR)/lib" ] && git clone -q https://github.com/hzeller/rpi-rgb-led-matrix.git $(RPI_RGB_LIB_DIR)) || git -C $(RPI_RGB_LIB_DIR) pull
	$(MAKE) -C $(RPI_RGB_LIB_DIR)/lib

# Intermediate dependencies
$(DEPS): $(RPI_RGB_LIB_FULLPATH) $(subst $(DEPDIR),$(SRCDIR),$(@:.d=.c))
	@echo Generating dependency file '$@'...
	@mkdir -p $(@D)
	cpp $(CFLAGS) $(subst $(DEPDIR),$(SRCDIR),$(@:.d=.c)) -MM -MT $(subst $(DEPDIR),$(OBJDIR),$(@:.d=.o)) > $@
	@echo '	@echo Building $$@...' >> $@
	@echo '	@mkdir -p $$(@D)' >> $@
	@echo '	@$$(CC) -c -o $$@ $$(subst $$(OBJDIR),$$(SRCDIR),$$(@:.o=.c)) $$(CFLAGS)' >> $@

-include $(DEPS)

.PHONY: clean
clean:
	@echo Removing $(OBJDIR), $(DEPDIR), and $(TARGET)...
	rm -fr ./temp/ $(TARGET) 
