CFLAGS = -DDEBUG -g -c -Wall -Wextra -Wwrite-strings
CFLAGS = -g -c -Wall -Wextra -Wwrite-strings
LFLAGS = -g

default: hashTab

clean:
	rm -f *.o
	rm -f hashTab

hashTab.o: hashTab.c global_defs.h
	gcc ${CFLAGS} hashTab.c

hashTab_funcs.o: hashTab_funcs.c hashTab_funcs.h
	gcc ${CFLAGS} hashTab_funcs.c

syscalls.o: syscalls.c syscalls.h
	gcc ${CFLAGS} syscalls.c

hashTab: hashTab.o hashTab_funcs.o syscalls.o
	gcc ${LFLAGS} -o hashTab hashTab.o hashTab_funcs.o syscalls.o

