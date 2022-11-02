run: compile
	./psort input.bin

runR: compileR
	./psort input.bin

compile:
	gcc -Wall -Werror -pthread -O psort.c -o psort  

compileR:
	gcc -Wall -pthread -O psort.c -o psort  

clean:
	rm -rf ./*.o

