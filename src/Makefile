OBJS = bmp.o image.o mbstring.o pnm.o hashtable.o hist.o option.o display.o display_console.o display_html.o process.o
MOBJS = $(OBJS) main.o
OBJTS = bmpt.o imaget.o mbstringt.o pnmt.o hashtablet.o
BINS = bmpt imaget mbstringt pnmt hashtablet histt main 
CC = gcc
LDFLAGS = -lm


main: $(MOBJS)
	$(CC) -g -O0 -o main $(MOBJS) $(LDFLAGS)
	$(CC) -g -O0 -o bmpt $(OBJS) bmpt.c $(LDFLAGS)
	$(CC) -g -O0 -o imaget $(OBJS) imaget.c $(LDFLAGS)
	$(CC) -g -O0 -o mbstringt $(OBJS) mbstringt.c $(LDFLAGS)
	$(CC) -g -O0 -o pnmt $(OBJS) pnmt.c $(LDFLAGS)
	$(CC) -g -O0 -o hashtablet $(OBJS) hashtablet.c $(LDFLAGS)


bmp.o: bmp.c
	$(CC) -c -g -O0 bmp.c $(LDFLAGS)

image.o: image.c
	$(CC) -c -g -O0 image.c $(LDFLAGS)

mbstring.o: mbstring.c
	$(CC) -c -g -O0 mbstring.c $(LDFLAGS)

pnm.o: pnm.c
	$(CC) -c -g -O0 pnm.c $(LDFLAGS)

hashtable.o: hashtable.c
	$(CC) -c -g -O0 hashtable.c $(LDFLAGS)

hist.o: hist.c
	$(CC) -c -g -O0 hist.c $(LDFLAGS)

option.o: option.c
	$(CC) -c -g -O0 option.c $(LDFLAGS)

display.o: display.c
	$(CC) -c -g -O0 display.c $(LDFLAGS)

display_console.o: display_console.c
	$(CC) -c -g -O0 display_console.c $(LDFLAGS)

display_html.o: display_html.c
	$(CC) -c -g -O0 display_html.c $(LDFLAGS)

process.o: process.c
	$(CC) -c -g -O0 process.c $(LDFLAGS)

main.o: main.c
	$(CC) -c -g -O0 main.c $(LDFLAGS)

bmpt.o: bmpt.c
	$(CC) -c -g -O0 bmpt.c $(LDFLAGS)

imaget.o: imaget.c
	$(CC) -c -g -O0 imaget.c $(LDFLAGS)

mbstringt.o: mbstringt.c
	$(CC) -c -g -O0 mbstringt.c $(LDFLAGS)

pnmt.o: pnmt.c
	$(CC) -c -g -O0 pnmt.c $(LDFLAGS)

hashtablet.o: hashtablet.c
	$(CC) -c -g -O0 hashtablet.c $(LDFLAGS)

clean:
	rm -f $(MOBJS)  $(OBJTS) $(BINS)
