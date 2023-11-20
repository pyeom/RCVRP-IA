CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=solve.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=solve

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)