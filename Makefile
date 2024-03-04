CPPFLAGS=-Wall
OBJS=main.o run-length.o files.o
EXEC=main

all: $(EXEC)

$(EXEC): $(OBJS)
	g++ $(CPPFLAGS) -o $@ $(OBJS)

clean:
	rm *.o
