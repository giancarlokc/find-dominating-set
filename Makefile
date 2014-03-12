	CFLAG=-O4
	FIND_PROG = bin/findDomSet
	FIND_OBJS = source/findDomSet.cpp
	CHECK_PROG = bin/checkDomSet
	CHECK_OBJS = source/checkDomSet.cpp
	CC = g++
	
	LIBS =

all: $(OBJS)
	@echo "Compiling $(CHECK_OBJS)"
	@echo "---Compiler: $(CC)"
	@echo "---Flags: $(CFLAG)"
	$(CC) $(CFLAG) -o $(CHECK_PROG) $(CHECK_OBJS) $(LIBS)
	
	@echo "Compiling $(FIND_OBJS)"
	@echo "---Compiler: $(CC)"
	@echo "---Flags: $(CFLAG)"
	$(CC) $(CFLAG) -o $(FIND_PROG) $(FIND_OBJS) $(LIBS)

clean:
	@rm -r $(PROG)
