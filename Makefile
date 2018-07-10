
## Created by Anjuta

CC = gcc
CFLAGS = -g
SOURCES = dtoa.c log10.c finite.c pow10.c strtod.c SoftFloat-2c/softfloat/bits32/softfloat.c
OBJECTS = $(SOURCES:%.c=%.o)
INCFLAGS = 
LDFLAGS = -D__LITTLE_ENDIAN
LIBS =

all: softfloat

softfloat: $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o softfloat $(SOURCES)

.SUFFIXES:
.SUFFIXES:	.c .cc .C .cpp .o

count:
	wc *.c *.cc *.C *.cpp *.h *.hpp

clean:
	rm -f $(OBJECTS) softfloat

.PHONY: all
.PHONY: count
.PHONY: clean
