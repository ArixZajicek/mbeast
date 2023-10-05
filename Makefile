# C Compiler
CC=g++

# Important directories, relative to this Makefile
SRCDIR=src
INCDIR=include
BUILDDIR=build
OBJDIR=$(BUILDDIR)/obj
DEPDIR=$(BUILDDIR)/dep

# Main Target object/name
TARGET=$(BUILDDIR)/mbeast


# Raspberry Pi name and paths
RPI_RGB_LIB_NAME=rgbmatrix
RPI_RGB_LIB_DIR=lib/rpi_rgb_led_matrix
RPI_RGB_LIB_FULLPATH=$(RPI_RGB_LIB_DIR)/lib/lib$(RPI_RGB_LIB_NAME).a

# Skia
SKIA_LIB_NAME=skia
SKIA_LIB_DIR=lib/skia
SKIA_SUB_DIR=out/Shared
SKIA_LIB_FULLPATH=$(SKIA_LIB_DIR)/$(SKIA_SUB_DIR)/libskia.so

# Compile flags
CLIBS=-lSDL2 -L$(RPI_RGB_LIB_DIR)/lib -l$(RPI_RGB_LIB_NAME) -L$(SKIA_LIB_DIR)/$(SKIA_SUB_DIR) -l$(SKIA_LIB_NAME) -lGL -lpthread -lrt -lm -lpthread
CINCLUDES=-I$(RPI_RGB_LIB_DIR)/include -I$(SKIA_LIB_DIR) -I$(INCDIR)
CFLAGS=$(CINCLUDES) -g -fno-exceptions -std=c++17 -Wl,-rpath=$(SKIA_LIB_DIR)/$(SKIA_SUB_DIR)

# Sources and intermediate files
SRCS=$(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)
OBJS=$(subst $(SRCDIR),$(OBJDIR),$(SRCS:.cpp=.o))
DEPS=$(subst $(SRCDIR),$(DEPDIR),$(SRCS:.cpp=.d))

# Main output
$(TARGET): $(RPI_RGB_LIB_FULLPATH) $(SKIA_LIB_FULLPATH) $(OBJS)
	@echo Building final target $(TARGET)...
	@$(CC) -o $@ $^ $(CLIBS) $(CFLAGS)
	@echo Done.

# Raspberry Pi lib (statically linked)
$(RPI_RGB_LIB_FULLPATH): 
	$(MAKE) -C $(RPI_RGB_LIB_DIR)/lib

# Skia lib (dynamically linked)
# Requires clang and Chromium build_depot to be set up and in the path:
# 	Clone build_depot from https://chromium.googlesource.com/chromium/tools/depot_tools.git
# 	Add to path with export PATH="/path/to/build_depot:${PATH}" in .bashrc
#
# REMOVED:
#  skia_enable_fontmgr_empty=true
$(SKIA_LIB_FULLPATH):
	cd $(SKIA_LIB_DIR) && \
	python3 tools/git-sync-deps && \
	bin/fetch-ninja && \
	bin/gn gen $(SKIA_SUB_DIR) --args='is_official_build=true is_component_build=true cc="clang" cxx="clang++" skia_pdf_subset_harfbuzz=false' && \
	ninja -C $(SKIA_SUB_DIR)

# Intermediate dependencies
$(DEPS): $(RPI_RGB_LIB_FULLPATH) $(SKIA_LIB_FULLPATH) $(subst $(DEPDIR),$(SRCDIR),$(@:.d=.cpp))
	@echo Generating dependency file '$@'...
	@mkdir -p $(@D)
	cpp $(CFLAGS) $(subst $(DEPDIR),$(SRCDIR),$(@:.d=.cpp)) -MM -MT $(subst $(DEPDIR),$(OBJDIR),$(@:.d=.o)) > $@
	@echo '	@echo Building $$@...' >> $@
	@echo '	@mkdir -p $$(@D)' >> $@
	@echo '	@$$(CC) -c -o $$@ $$(subst $$(OBJDIR),$$(SRCDIR),$$(@:.o=.cpp)) $$(CFLAGS)' >> $@

-include $(DEPS)

.PHONY: clean
clean:
	rm -fr $(BUILDDIR)

.PHONY: cleanrpi
cleanrpi:
	$(MAKE) -C $(RPI_RGB_LIB_DIR)/lib clean
