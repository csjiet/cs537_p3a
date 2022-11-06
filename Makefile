run: compile
	./psort input output

runR: compileR
	./psort input output

compile:
	gcc -Wall -Werror -pthread -O psort.c -o psort  

compileR:
	gcc -Wall -pthread -O psort.c -o psort  

hexDump: input output
	hexdump input > inputHex.txt
	hexdump output > outputHex.txt

hexDumpVerbose: input output
	hexdump --canonical input > inputHex.txt
	hexdump --canonical output > outputHex.txt

hexDumpInteger: input output
	hexdump -e'25/4 "%d ""\n"' output > outputHex.txt
	hexdump -e'25/4 "%d ""\n"' input > inputHex.txt

genRandBin:
	dd if=/dev/random of=input bs=1000 count=1 iflag=fullblock

test: clean compile
	time ~cs537-1/tests/p3a/test-psort.sh

clean:
	rm -rf ./*.o
	rm -rf tests-out
	rm -f input
	rm -f output
	rm -f inputHex.txt
	rm -f outputHex.txt

