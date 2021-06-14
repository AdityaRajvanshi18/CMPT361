	#! /usr/bin/make

# Including of non standard library files:
#   INCLUDEDIR is where the header files can be found
#   LIBDIR is where the library object files can be found
INCLUDEDIR=/usr/include/
LIBDIR=/usr/lib
 
SOURCE= scene.cpp image_util.cpp sphere.cpp vector.cpp trace.cpp raycast.cpp include/InitShader.cpp

# The compiler  
CC= g++

# The flags
CFLAGS= -std=c++11 -O3 -g -Wall -pedantic -DGL_GLEXT_PROTOTYPES

# The name of the final executable 
EXECUTABLE= rayT

# Linux (default)
LDFLAGS = -lGL -lglut -lGLEW -lXext -lX11 -lm

#other library files
INCLUDEFLAG= -I. -I$(INCLUDEDIR) -Iinclude/
LIBFLAG= -L$(LIBDIR)

OBJECT= $(SOURCE:.cpp=.o)

all: $(OBJECT) depend
	$(CC) $(CFLAGS) $(INCLUDEFLAG) $(LIBFLAG)  $(OBJECT) -o $(EXECUTABLE) $(LDFLAGS)

depend:
	$(CC) -M $(SOURCE) > depend

$(OBJECT):
	$(CC) $(CFLAGS) $(INCLUDEFLAG) -c -o $@ $(@:.o=.cpp)

clean_object:
	rm -f $(OBJECT)

clean:
	rm -f $(OBJECT) depend $(EXECUTABLE)

include depend
