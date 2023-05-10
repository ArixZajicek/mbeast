# Important dirs
LIBDIR=../lib
CLIBS=-I../include -L$(LIBDIR) -lcostume -lm
CFLAGS=$(CLIBS) -O2

.PHONY:
run-basic: basic_sdl_c/main.o
	(LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LIBDIR) ./basic_sdl_c/main.o)

# Test 
basic_sdl_c/main.o:
	@gcc basic_sdl_c/main.c -o basic_sdl_c/main.o $(CLIBS)

.PHONY:
clean:
	rm -f */*.o