CFLAG=-Wall
	PROG = findDomSet
	OBJS = findDomSet.cpp
	CC = g++
	LIBS =

all: $(OBJS)
	$(CC) -O3 -o $(PROG) $(OBJS) $(LIBS)

clean:
	@rm -r $(PROG)
