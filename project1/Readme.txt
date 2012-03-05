
In order to compile:
On a unix machine, run 'make'
This will build all three parts of the assignment.

To invoke:
Part 1:
./part1 <number of threads>

Part 2:
./part2 <number of threads>

Part3:
./part3 [-n <number of items to enqueue and dequeue>] [-m <queue size>] [-t <type>]
Where <type> is one of the following: lock, lockfree
If any of the above options are not provided, the defaults are as follows:
number of items: 200000
queue size:      100
type:            BOTH