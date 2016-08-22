#
# Based on makefile:
#       @(#)Makefile    5.1 (Berkeley) 5/31/85
#
#       cop4disas.c
#
HDRS = 
PHDRS = $(HDRS)
SRCS = cop4disas.c
PSRCS = $(SRCS)
OBJS = cop4disas.o
DESTDIR = /usr/local

DFLAGS = -DVP -DBSD -DUNIX
CFLAGS =  -O $(DFLAGS)

LINTFLAGS = $(DFLAGS)

cop4disas:      $(OBJS)
        $(CC) $(CFLAGS) $(OBJS) -o cop4disas

$(OBJS): $(HDRS)

install:
        install -s cop4disas $(DESTDIR)/bin/cop4disas
        install -c cop4disas.man $(DESTDIR)/man/man1/cop4disas.1

clean:
        rm -f cop4disas $(OBJS)

lint:   
        lint $(LINTFLAGS) $(SRCS)

psrcs:
        echo $(PHDRS) $(PSRCS)

print:
        print $(PHDRS) $(PSRCS)
