shell: libexecute.a
	gcc -L. -o mainShell shell.c -lexecute
libexecute.a: execute.o
	ar -cvq libexecute.a execute.o
execute.o:
	gcc -c execute.c
clean:
	rm -f execute a.out lib*.a *.o 
