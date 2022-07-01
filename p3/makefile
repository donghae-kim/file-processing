a : main.o ftl.o devicedriver.o
	gcc -o a main.o ftl.o devicedriver.o

main.o : main.c
		gcc -c -o main.o main.c

ftl.o : ftl.c
		gcc -c -o ftl.o ftl.c

devicedriver.o : devicedriver.c
		gcc -c -o devicedriver.o devicedriver.c

clean :
		rm *.o ssu_sindex