CC = gcc
LD = gcc
CFLAGS = -g -Wall -I/usr/local/include
LDFLAGS = -L/usr/local/lib
RM = /bin/rm -f

LIBS = -levent

OBJS = hoa.o denon.o pwrmate.o ultrax.o mplayer.o
PROG = hoa

all: $(PROG)

$(PROG): $(OBJS)
		$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROG)

%.o: %.c
		$(CC) $(CFLAGS) -c $<

clean:
		$(RM) $(PROG) $(OBJS)

#install: all
#        install -m 751 -b -o vpopmail -g vchkpw chkpw /home/vpopmail/bin/chkpw
