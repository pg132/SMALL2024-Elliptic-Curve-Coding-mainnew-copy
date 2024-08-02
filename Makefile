# MACROS
CC = g++
CFLAGS = -std=c++11

# Dependency rules for non-file targets
all: graphgarbage

clobber: clean
	rm -f *~ \#*\# 

clean: 
	rm -f *.o graphgarbage

# Dependency rules for file targets
computegarbage: quarticclasses.o helperfunctions.o
	$(CC) $(CFLAGS) quarticclasses.o helperfunctions.o -o computegarbage

graphgarbage: quarticgrapher.o helperfunctions.o
	$(CC) $(CFLAGS) quarticgrapher.o helperfunctions.o -o graphgarbage

#quarticclasses.o: quarticclasses.cpp helperfunctions.h
#$(CC) -c quarticclasses.cpp

quarticgrapher.o: quarticgrapher.cpp helperfunctions.h
	$(CC) $(CFLAGS) -c quarticgrapher.cpp

helperfunctions.o: helperfunctions.cpp helperfunctions.h
	$(CC) $(CFLAGS) -c helperfunctions.cpp

helperfunctions.h:
	touch helperfunctions.h
