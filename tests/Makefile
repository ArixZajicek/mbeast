# Important dirs
LIBDIR=../lib
RPILIBDIR=$(LIBDIR)/ext/rpi_rgb_led_matrix/lib

CLIBDIRS=-L$(RPILIBDIR) -L$(LIBDIR) 
CLIBS=
CINCLUDES=-I../include

.PHONY:
run-basic: basic_sdl_c/main.o
	(LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(RPILIBDIR):$(LIBDIR) ./basic_sdl_c/main.o)

# Test 
basic_sdl_c/main.o:
	gcc $(CINCLUDES) $(CLIBDIRS) $(CLIBS) basic_sdl_c/main.c -o basic_sdl_c/main.o -lcostume -lSDL2 -lrgbmatrix -lrt -lm -lpthread -lstdc++

.PHONY:
clean:
	rm -f */*.o