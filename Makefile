CC = gcc
CFLAGS = -W -Wall -Wextra -std=c23 -pedantic -Wno-unused-parameter -Wno-unused-variable
TARGETS = parent child

all: $(TARGETS)

parent: parent.c
	$(CC) $(CFLAGS) -o parent parent.c

child: child.c
	$(CC) $(CFLAGS) -o child child.c

clean:
	rm -f $(TARGETS)
