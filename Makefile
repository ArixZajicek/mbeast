# C Compiler
CC=gcc

# Important directories, relative to this Makefile
SRCDIR=libcostume/src
INCDIR=libcostume/src
OBJDIR=build/obj
DEPDIR=build/dep

# Main Target object/name
TARGET=build/libcostume.so

# Compile flags
CLIBS=
CFLAGS=$(CLIBS) -fPIC -I$(INCDIR) -lSDL2

SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(subst $(SRCDIR),$(OBJDIR),$(SRCS:.c=.o))
DEPS=$(subst $(SRCDIR),$(DEPDIR),$(SRCS:.c=.d))

# Main output executable
$(TARGET): $(OBJS)
	@echo Building final target $(TARGET)...
	@$(CC) -o $@ $^ $(CFLAGS) -shared
	@echo Done.

# Dependencies
$(DEPS): $(subst $(DEPDIR),$(SRCDIR),$(@:.d=.c))
	@echo Generating dependency file '$@'...
	@mkdir -p $(@D)
	@cpp $(CFLAGS) $(subst $(DEPDIR),$(SRCDIR),$(@:.d=.c)) -MM -MT $(subst $(DEPDIR),$(OBJDIR),$(@:.d=.o)) > $@
	@echo '	@echo Building $$@...' >> $@
	@echo '	@mkdir -p $$(@D)' >> $@
	@echo '	@$$(CC) -c -o $$@ $$(subst $$(OBJDIR),$$(SRCDIR),$$(@:.o=.c)) $$(CFLAGS)' >> $@

-include $(DEPS)

# Test 
test: $(TARGET)
	@gcc tests/basic_sdl_c/main.c -o build/test -Ilibcostume -Lbuild -lcostume -lm
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:./build ./build/test

.PHONY: clean
clean:
	@echo Removing $(OBJDIR), $(DEPDIR), and $(TARGET)...
	@rm -f ./build/
