CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = test_queue
SCAN_BUILD_DIR = scan-build-out

test_queue: test_queue.o queue.o 
	$(CC) -o $(EXE) test_queue.o queue.o

test_queue.o: test_queue.c
	$(CC) $(WARNING_FLAGS) -c test_queue.c

queue.o: queue.h queue.c
	$(CC) $(WARNING_FLAGS) -c queue.c

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
