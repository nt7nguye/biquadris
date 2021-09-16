CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -Werror=vla
EXEC = biquadris
OBJECTS = main.o commandinterpreter.o game.o block.o board.o cell.o command.o\
          player.o moveblock.o subject.o levelfactory.o incrementlevel.o\
          blockfactory.o fileblockfactory.o randomblockfactory.o prefixmap.o\
          dropblock.o rotateblock.o commandtokenizer.o textdisplay.o\
          heavyeffect.o heavyaction.o norandom.o restorerandom.o\
          transformblock.o garbagetower.o statuseffect.o forceaction.o\
          blindaction.o blindeffect.o window.o graphicsdisplay.o\
          heavyleveleffect.o gamesetup.o pokeaction.o


DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

debug: CXXFLAGS += -DDEBUG -g
debug: ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
