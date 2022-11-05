run: compile
	./psort input output

runR: compileR
	./psort input output

compile:
	gcc -Wall -Werror -pthread -O psort.c -o psort  

compileR:
	gcc -Wall -pthread -O psort.c -o psort  

hexDump: input.bin output.bin
	hexdump input.bin > inputHex.txt
	hexdump output.bin > outputHex.txt

hexDumpVerbose: input.bin output.bin
	hexdump --canonical input.bin > inputHex.txt
	hexdump --canonical output.bin > outputHex.txt

hexDumpInteger: input.bin output.bin
	hexdump -e'25/4 "%d ""\n"' output.bin > outputHex.txt
	hexdump -e'25/4 "%d ""\n"' input.bin > inputHex.txt

genRandBin:
	dd if=/dev/random of=input bs=1000 count=1 iflag=fullblock

test: clean compile
	~cs537-1/tests/p3a/test-psort.sh

clean:
	rm -rf ./*.o
	rm -rf tests-out
	rm -f input
	rm -f output
	rm -f inputHex.txt
	rm -f outputHex.txt

