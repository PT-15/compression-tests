CPPFLAGS=-Wall -g
OBJS=main.o run-length.o files.o dictionary.o node.o timer.o compare-methods.o
EXEC=main


all: $(EXEC)

$(EXEC): $(OBJS)
	g++ $(CPPFLAGS) -o $@ $(OBJS)

clean:
	rm *.o
