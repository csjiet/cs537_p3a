run: compile
	./psort input.bin

runR: compileR
	./psort input.bin

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
	dd if=/dev/random of=input.bin bs=100000000 count=1 iflag=fullblock



clean:
	rm -rf ./*.o
	rm -f output.bin
	rm -f inputHex.txt
	rm -f outputHex.txt

