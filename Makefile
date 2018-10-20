CC = gcc
WARNING_FLAGS = -Wall -Wextra -g

EXE = prodcomm
SCAN_BUILD_DIR = scan-build-out

prodcomm: main.o sync.o queue.o p2_thread.o
	$(CC) -g -o $(EXE) main.o sync.o queue.o p2_thread.o -lpthread

main.o: main.c
	$(CC) $(WARNING_FLAGS) -c -g main.c

sync.o: sync.c  sync.h
	$(CC) $(WARNING_FLAGS) -c -g sync.c

queue.o: queue.c  queue.h
	$(CC) $(WARNING_FLAGS) -c -g queue.c

p2_thread.o: p2_thread.c  p2_thread.h
	$(CC) $(WARNING_FLAGS) -c -g p2_thread.c

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

