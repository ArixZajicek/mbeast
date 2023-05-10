
.PHONY:
test: libcostume
	$(MAKE) -C tests

.PHONY:
libcostume:
	$(MAKE) -C lib

.PHONY:
clean:
	$(MAKE) -C tests clean
	$(MAKE) -C lib clean
