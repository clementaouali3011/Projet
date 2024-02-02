# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -g -O3
LDFLAGS =
LDLIBS =

EXE = traitement_t traitement_s

all: ${EXE}

$(foreach f, $(EXE), $(eval $(f):))

.PHONY: clean

clean:
	${RM} ${EXE}

# END
