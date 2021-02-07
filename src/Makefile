IDIR =../include
CC=cc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR=../lib

LIBS=-lm -lmicrohttpd

_DEPS = serve.h fetch.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = httpsvr.o  main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

svr: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	clear
