SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))
PROGRAM=http
CC=g++
CPPFLAGS=-std=c++11
CFLAGS=-lpthread

$(PROGRAM) : $(OBJECTS)
		echo $(SOURCES)
		echo $(OBJECTS)
	$(CC) $(CPPFLAGS) -o $(PROGRAM) $(OBJECTS) 
$(OBJECTS):$(SOURCES)

.PHONY : clean
clean : 
	rm -rf $(OBJECTS) $(PROGRAM)
