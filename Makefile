CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = prodcomm
SCAN_BUILD_DIR = scan-build-out

prodcomm: main.o sync.o queue.o p2_thread.o
	$(CC) -o $(EXE) main.o sync.o queue.o p2_thread.o -lpthread

main.o: main.c
	$(CC) $(WARNING_FLAGS) -c main.c

sync.o: sync.c  sync.h
	$(CC) $(WARNING_FLAGS) -c sync.c

queue.o: queue.c  queue.h
	$(CC) $(WARNING_FLAGS) -c queue.c

p2_thread.o: p2_thread.c  p2_thread.h
	$(CC) $(WARNING_FLAGS) -c p2_thread.c

clean:
	rm *.o $(EXE) 
	rm -rf $(SCAN_BUILD_DIR)

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html

submit-d: 
	cp partner.txt README ~cs537-1/handin/do-leary/proj2
	cp *.[ch] Makefile ~cs537-1/handin/do-leary/proj2/code

submit-e:
	cp partner.txt ~cs537-1/handin/ezra/proj2

test1:
	prodcomm < ~cs537-1/public/proj2/test1

test2:
	prodcomm < ~cs537-1/public/proj2/test2

test3:
	prodcomm < ~cs537-1/public/proj2/test3.txt

ten_t:
	prodcomm < ~cs537-1/public/proj2/ten_thousand_char_line.txt

one_h:
	prodcomm < ~cs537-1/public/proj2/one_hundred_thousand_char_line.txt

one_m:
	prodcomm < ~cs537-1/public/proj2/one_million_char_line.txt

no_n:
	prodcomm < ~cs537-1/public/proj2/no_newline.txt
