CXX = g++-5
CXXFLAGS = -std=c++14 -Wall -MMD -Werror=vla
EXEC = sorcery
OBJECTS = main.o card.o minion.o basic_minion.o player.o handler.o card_factory.o spell.o ability.o activated_ability.o triggered_ability.o ritual.o board.o ascii_graphics.o enchantment.o window.o subject.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
.PHONY: clean
