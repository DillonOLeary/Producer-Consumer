CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = muncher
SCAN_BUILD_DIR = scan-build-out

muncher: main.o sync.o queue.o p2_thread.o
	$(CC) -pthread -o $(EXE) main.o sync.o queue.o p2_thread.o

main.o: main.c
	$(CC) $(WARNING_FLAGS) -c main.c

sync.o: sync.c  sync.h
	$(CC) $(WARNING_FLAGS) -c sync.c  sync.h

queue.o: queue.c  queue.h
	$(CC) $(WARNING_FLAGS) -c queue.c  queue.h

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
	cp *.[ch] makefile partner.txt README ~cs537-1/handin/do-leary/proj2

submit-e:
	cp partner.txt ~cs537-1/handin/ezra/proj2

