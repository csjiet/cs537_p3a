run: compile
	./psort

compile:
	gcc -Wall -Werror -pthread -O psort.c -o psort  

clean:
	rm -rf ./*.o
	rm ./psort 

