C = g++
FLAGS = -g -Wall
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

HDIR = $(PWD)/src/headers
ODIR = $(PWD)/src/obj
JSON = $(PWD)/src/json/json.h

OBJ = $(ODIR)/main.o \
      $(ODIR)/game.o \
      $(ODIR)/handlers.o \
      $(ODIR)/config.o \
      $(ODIR)/jsoncpp.o \
      $(ODIR)/pacman.o \
      $(ODIR)/map.o \
      $(ODIR)/tile.o

.PHONY: clean

pacman: $(OBJ)
	$(C) -o $@ $^ $(FLAGS) $(LIBS)


_DEPS1 = game.hpp
DEPS1 = $(patsubst %, $(HDIR)/%, $(_DEPS1))
$(ODIR)/main.o: src/main.cpp $(DEPS1)
	$(C) -c -o $@ $< $(FLAGS)

_DEPS2 = config.hpp map.hpp tile.hpp
DEPS2 = $(patsubst %, $(HDIR)/%, $(_DEPS2))
$(ODIR)/config.o: src/config.cpp $(DEPS2)
	$(C) -c -o $@ $< $(FLAGS)

_DEPS3 = handlers.hpp
DEPS3 = $(patsubst %, $(HDIR)/%, $(_DEPS3))
$(ODIR)/handlers.o: src/handlers.cpp $(DEPS3)
	$(C) -c -o $@ $< $(FLAGS)

_DEPS4 = json.h
DEPS4 = $(patsubst %, src/json/%, $(_DEPS4))
$(ODIR)/jsoncpp.o: src/jsoncpp.cpp $(DEPS4)
	$(C) -c -o $@ $< $(FLAGS)

_DEPS5 = tile.hpp config.hpp
DEPS5 = $(patsubst %, $(HDIR)/%, $(_DEPS5))
$(ODIR)/tile.o: src/tile.cpp $(DEPS5)
	$(C) -c -o $@ $< $(FLAGS)

_DEPS6 = map.hpp tile.hpp config.hpp
DEPS6 = $(patsubst %, $(HDIR)/%, $(_DEPS6))
$(ODIR)/map.o: src/map.cpp $(DEPS6)
	$(C) -c -o $@ $< $(FLAGS)

_DEPS7 = pacman.hpp map.hpp tile.hpp config.hpp
DEPS7 = $(patsubst %, $(HDIR)/%, $(_DEPS7))
$(ODIR)/pacman.o: src/pacman.cpp $(DEPS7)
	$(C) -c -o $@ $< $(FLAGS)

_DEPS8 = game.hpp config.hpp handlers.hpp pacman.hpp map.hpp tile.hpp
DEPS8 = $(patsubst %, $(HDIR)/%, $(_DEPS8))
$(ODIR)/game.o: src/game.cpp $(DEPS8)
	$(C) -c -o $@ $< $(FLAGS)

clean:
	rm -f $(OBJ)/*.o
