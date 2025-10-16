CC = gcc
CFLAGS = -g -Wall -Wno-unused-function

FLEX = flex
BISON = bison
BISON_FLAGS = -d -v

SOURCES = main.c ast.c visualize.c codegen.c parser.tab.c lex.yy.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = mini_compiler

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS)

parser.tab.c parser.tab.h: parser.y ast.h
	$(BISON) $(BISON_FLAGS) parser.y

lex.yy.c: scanner.l parser.tab.h
	$(FLEX) -o lex.yy.c scanner.l

main.o: main.c parser.tab.h
	$(CC) $(CFLAGS) -c main.c -o main.o

visualize.o: visualize.c parser.tab.h
	$(CC) $(CFLAGS) -c visualize.c -o visualize.o

codegen.o: codegen.c parser.tab.h
	$(CC) $(CFLAGS) -c codegen.c -o codegen.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(EXECUTABLE) input.c

clean:
	rm -f $(EXECUTABLE) $(OBJECTS) lex.yy.c parser.tab.c parser.tab.h parser.output *.dot *.png