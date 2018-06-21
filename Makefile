CC			=	cc
CFLAGS		=	-I include/
COMPILE		=	$(CC) $(CFLAGS) -c $< -o $@
LINK		=	$(CC) $(CFLAGS) $^ lib/libsx.so -o $@

all: ColorMatch

ColorMatch: build/colormatch.o build/game.o build/board.o build/setting.o
	@echo linking files
	@$(LINK)

build/colormatch.o: src/colormatch.c include/*
	@mkdir -p build
	@echo compiling colormatch
	@$(COMPILE)

build/game.o: src/game.c include/game.h
	@mkdir -p build
	@echo compiling game
	@$(COMPILE)

build/board.o: src/board.c include/board.h
	@mkdir -p build
	@echo compiling board
	@$(COMPILE)

build/setting.o: src/setting.c include/setting.h
	@mkdir -p build
	@echo compiling setting
	@$(COMPILE)

.PHONY: clean
clean:
	@rm -rf build
