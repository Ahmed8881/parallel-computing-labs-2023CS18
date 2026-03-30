# Lab 2: Point-to-Point Communication in MPI

## Overview
Lab 2 focuses on point-to-point (P2P) communication patterns in MPI, where individual processes explicitly send and receive messages. Students implement various communication patterns including basic send/receive, custom data gathering, parallel prefix sum, and wildcard message reception. This lab emphasizes the distinction between blocking and non-blocking communication, message tags, and working with derived data types.

## Files
- `send_recv.c` – Demonstrates basic blocking send (`MPI_Send`) and receive (`MPI_Recv`) between process pairs
- `parallel_sum.c` – Implements parallel sum computation using point-to-point communication
- `custom_gather.c` – Gathers data from all processes to a single root process using direct P2P communication
- `wildcard.c` – Demonstrates receiving messages with wildcard source (`MPI_ANY_SOURCE`) and tag (`MPI_ANY_TAG`)

## Compilation and Execution

```bash
# Compile all programs
mpicc -o send_recv send_recv.c
mpicc -o parallel_sum parallel_sum.c
mpicc -o custom_gather custom_gather.c
mpicc -o wildcard wildcard.c
```

### Running the Programs

```bash
# Send/Receive demonstration (process 2 sends to process 4)
mpirun -np 5 ./send_recv

# Parallel sum with multiple processes
mpirun -np 4 ./parallel_sum

# Custom gather from all processes
mpirun -np 4 ./custom_gather

# Wildcard message reception
mpirun -np 4 ./wildcard
```

## Example Output

### Send/Receive
```
Process 0: waiting
Process 1: waiting
Process 2 sent data to 4
Process 3: waiting
Process 4 received data from 2
Data received: 0.000000 1.000000 2.000000 3.000000 ...
```

### Parallel Sum
```
Process 0: Computing sum of local chunk [0 - 24999999]
Process 0 local sum = 312499875000000
Process 1: Computing sum of local chunk [25000000 - 49999999]
Process 1 local sum = 937499875000000
Process 2: Computing sum of local chunk [50000000 - 74999999]
Process 2 local sum = 1562499875000000
Process 3: Computing sum of local chunk [75000000 - 99999999]
Process 3 local sum = 2187499875000000
Total sum = 5000000375000000
Time = X.XX seconds
```

### Custom Gather
```
Process 0: Computed value 42
Process 1: Computed value 100
Process 2: Computed value 256
Process 3: Computed value 512
Process 0 gathered all values: [42, 100, 256, 512]
```

### Wildcard Reception
```
Process 0: Sending message to Process 1, 2, 3
Process 1: Received message from source 0 (tag 100)
Process 2: Received message from source 0 (tag 101)
Process 3: Received message from source 0 (tag 102)
```

## Challenges Faced

### Deadlock in Send/Receive
**Challenge**: Improper ordering of sends and receives can cause processes to block indefinitely, waiting for messages.
**Resolution**: Understood that large messages may use buffered mode (requiring a matching receive), while small messages use eager mode. Used careful sequencing: sender sends first, then receiver receives, or used non-blocking operations.

### Message Ordering and Tags
**Challenge**: Multiple messages in flight between processes can arrive out of order or be ambiguous.
**Resolution**: Assigned unique message tags to distinguish between different types of communications, allowing precise matching of sends with receives.

### Memory Allocation for Received Data
**Challenge**: Receiving variable-sized data from multiple processes requires pre-allocating sufficient buffer space.
**Resolution**: Used `MPI_Probe()` to determine message size before receiving, or preallocated buffers based on problem constraints.

### Custom Gather vs Collective Operations
**Challenge**: Implementing gather functionality with P2P communication is inefficient compared to MPI collective operations.
**Resolution**: Recognized this implementation as educational; noted that `MPI_Gather()` provides better performance for this pattern.

### Wildcard Source and Tag Management
**Challenge**: Using `MPI_ANY_SOURCE` and `MPI_ANY_TAG` makes it difficult to track which process sent which message type.
**Resolution**: Used `MPI_Status` to retrieve sender information after receive, then acted accordingly on the received data.

### Non-blocking Communication
**Challenge**: Distinguishing between blocking (`MPI_Send`/`MPI_Recv`) and non-blocking (`MPI_Isend`/`MPI_Irecv`) communication patterns.
**Resolution**: Used blocking operations initially for correctness, then explored non-blocking variants for understanding how to overlap communication with computation.
