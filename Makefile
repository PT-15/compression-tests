CPPFLAGS=-Wall -g
OBJS=main.o run-length.o files.o dictionary.o node.o
EXEC=main

# TODO: Get rid of debugging flag

all: $(EXEC)

$(EXEC): $(OBJS)
	g++ $(CPPFLAGS) -o $@ $(OBJS)

clean:
	rm *.o
