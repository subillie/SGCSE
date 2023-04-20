# Contents

## Midterm

### 1. [ECF - Exceptions and Processes](./01_ECF_Process.md)
- Exceptions
	- Events that require nonstandard control flow
	- Generated externally (interrupts) or internally (traps and faults)
- Processes
	- At any given time, system has multiple active processes
	- Only one can execute at a time on a single core, though
	- Each process appears to have total control of processor + private memory space

### 2. [ECF - Signals and Nonlocal Jumps](./02_ECF_Signal.md)
- Signals provide process-level exception handling
	- Can gererate from user programs
	- Can define effect by declaring signal handler
	- Be very careful when  writing signal handlers
- Nonlocal jumps provide exceptional control flow within process
	- Within constraints of stack discipline
### 3. [System Level I/O](./03_SystemLevel_IO.md)
### 4. [Network Programming](./04_Network_Programming.md)
