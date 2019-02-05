# Producer-Consumer
This program processes strings using multiple thread which must all work together.
## Program Structure
The program is divided into the threads and the control for the threads. Every thread can be run, but the behavior of the run function changes based on the type of thread it is. To achieve polymorphic behavior, each thread structure has a every field needed by any one of the threads, but each thread will only use a subset of the fields in its action function.

The main file will loop until the do\_action for each particular returns END, at that point the thread will stop running.
