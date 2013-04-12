testaf:  testaf.c libaf.a
	gcc -o testaf testaf.c -L. -laf 

testaf1:  test1af.c libaf.a
	gcc -o test1af test1af.c -L. -laf 

testaf2:  test2af.c libaf.a
	gcc -o test2af test2af.c -L. -laf 

all: test0 test1 test2 test3 testaf testaf1 testaf2

test1:  test1.c libaf.a
	gcc -o test1 test1.c -L. -laf 

test0:  test0.c libaf.a
	gcc -o test0 test0.c -L. -laf 

test2:  test2.c libaf.a
	gcc -o test2 test2.c -L. -laf 

test3:  test3.c libaf.a
	gcc -o test3 test3.c -L. -laf 

libaf.a: p1.o
	ar rf libaf.a p1.o

p1.o: p1.c
	gcc -c p1.c -o p1.o

clean:
	rm libaf.a test1 test2 test3 test0 testaf test1af test2af *.o
