
hyperloop.o: hyperloop.S
	gcc -c -o hyperloop.o hyperloop.S

hyper.o: hyper.c
	gcc -c -g -o hyper.o hyper.c

hyper: hyper.o hyperloop.o
	gcc -o hyper hyper.o hyperloop.o

clean:
	rm hyper *.o

