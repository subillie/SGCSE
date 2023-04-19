# ECF (Exceptional Control Flow)

## Control Flow
- Processors do only one thing at a time.  
- kernel(OS) : kernel code가 모인 라이브러리 느낌  

## Processes
- `program`이 memory 상에서 돌기 시작하면 `process`, 실행자는 `processor(= CPU)`
- Process provides each program w/ two key abstractions:
	- Logical control flow : 각 program이 CPU를 단독으로 사용하는 것이 아니라 운영체제가 control하는 것, context switching
	- Private address space : 각 program이 main memory를 단독으로 사용하는 것이 아니라 virtual memory 사용
- ECF : `program state(= process state)`이 아니라 `system state(=processor state)`이 바뀌는 상황
- change in program state : jumps and branches / call and return / ...


## Exceptional Control Flow
|low leval mechanisms|implemented by...|
|:---:|:---|
|Exceptions|hadware, OS software|

|higher level mechanisms|implemented by...|
|:---:|:---|
|Process context switch|OS software, hardware timer|
|Signals|OS software|
|Nonlocal jumps|C runtime library|

## Exceptions
- 각 event는 고유 번호의 `handler(interrupt vector)`가 있음
- event 발생하면, user code -> kernel code -> `exception handling`
- `exception table`의 주소(번호)를 찾아감 -> 해당 exception code 실행 -> 다시 process로 돌아감 (해결하는 동안 process는 동시적으로 실행되고 있음)  
###### ZeroDivisionError 같은 경우, software 수준에서 발생하는 예외 상황

|class|cause|동기/비동기|return behavior|
|:---:|:---|:---:|:---|
|**Interrupt**|Signal from I/O device|Async|Always returns to next instruction|
|**Trap**|Intentional exception|Sync|Always returns to next instruction|
|**Fault**|Unintentional / Potentially recoverable error|Sync|Might return to current instruction|
|**Abort**|Unintentional / Nonrecoverable error|Sync|Never returns|
###### SYNC exception (내 코드가 발생시킴) vs ASYNC exception (외부에서 발생시킴)

- Asynchronuous Exceptions (**`Interrupts`**)
	1. Interrupt pin goes high (CPU가 신호를 받으면)
		- CPU는 signal을 받는 pin들이 있음 -> 그 중 interrupt pin이 존재
	2. (현재 instruction을 끝내고) Passes to handler -> Put in the table
	3. Interrupt handler runs
	4. Handler returns to next instruction
	- e.g.
		- I/O request completes : arrival of packet from network, arrival of data from disk
		- timer interrupt (every few ms)
		- typing ctrl-c
- Synchronous Exceptions
	- **`Traps`**  
		e.g. `system calls`, breakpoint traps, special instructions
		- system call은 대부분 library에 정의되어 있음 : syscall() 함수  
		- System Call Number : system call은 모두 고유 ID 번호가 있음
			- 예) read(0), write(1), open(2), close(3), fork(57), execve(59), exit(60), kill(62)
		- `dual mode application` : disk나 network에서 읽어오는 일은 user가 할 수 없고 OS에 요청
	- **`Faults`**  
		-> re-execute faulting inst or aborts  
		e.g.
		- `page faults` (recoverable)  
			-> copy page from disk to memory -> return and re-execute  
			폰 노이만 구조에서 모든 파일은 main memory 상에서 실행됨  
			실행하려는 파일(내 코드)이 memory 상에 없고 disk에 있으면 page fault 발생 (-> main memory로 가져와서 실행해야 함)
		- `protection faults` (unrecoverable(e.g. 가상메모리, `SIGSEGV`))  
			 -> detect invalid address -> signal process
		- floating point exceptions (e.g. overflow)
	- **`Aborts`**  
		e.g. illegal instruction, parity error, machine check

## Multiprocessing
- Multiprocessing is illusion.
	- `concurrent` : 동시에 작동하는 것처럼 보임 (단일 CPU의 context switch) <-> `sequential`
	- `parallel` : 실제로 동시에 작동 (multi-processor)

### Single CPU
1. CPU executes multiple processes concurrently.
2. Save current registers in memory. (아래 memory 구조 참고)
3. Schedule next processes for execution.
4. Load 'saved registers' and switch address space (context switch)
	- process 하나 돌려놓고 다음꺼 scheduling -> exception -> interrupt code를 통해 계속 상태 체크
### Multiple CPUs (Core: 프로그램의 명령을 실행하는 처리 단위)
- main memory와 some caches 공유, 각 CPU는 각각의 process 실행 가능

## Error Handling
- 거의 모든 system-level 함수들은 fail할 수 있음
- error -> return -1, and set global variable `errno` to indicate cause
	```c
	void unix_ierror(char *msg) {
		fprintf(stderr, "%s: %s\n", msg, strerror(errno)); exit (1); }
	```
- `Stevens-style error-handling wrappers` : only returns if successful

## Process IDs
```c
pid_t getpid(void)	// returns PID of 'current process'
pid_t getppid(void)	// returns PID of 'parent process'
```

## Process States
- `Running`		: 실행중 or chosen to execute (실행을 기다리는 중, kernel에 의해 scheduled될 예정)
- `Stopped`		: execution is suspended and will not be scheduled until further notice
- `Terminated`	: stopped permanently (kill(), return in main, exit())  
	'exit' and 'execve' are called once but never returns  
	<-> 'fork' is called once but returns twice : parent와 child / 처리 순서는 알 수 없음 (OS가 결정)

## Creating Processes
프로세스를 실행한다 == 프로세스를 만든다
```c
int fork(void)	// returns (0) to child process, (child's pid) to parent process
```

|Memory 구조|||
|:---:|:---:|:---:|
|Virtual memory|Stack<br>Heap<br>Data<br>Code|Optimization when fork : <br>Use `copy-on-write` to avoid copying RAM<br>해당 데이터가 있는 `Page table`만 copy<br>(실제 코드를 copy하는 것이 아님)|
||Saved registers|process의 정보가 담겨 있음<br>(각 process 생성 시 함께 생성)|
|CPU|Registers||

## Modeling
Process Graph
- `DAG(Directed Acyclic Graph)` : 비순환 그래프 (방향성은 있는데 cycle이 없는 그래프)  
- Topological sort (순서가 있는 정렬) <=> Feasible total ordering  
	<=> 모두 left to right이라는 의미
- feasible은 (발생 가능한 일) vs infeasible (발생 불가능한 일) 파악하기 위함

## Reaping Child Processes
child는 running, parent는 종료 상태인 경우 : `orphan` -> 
- reaping되면 termination
- reaping되지 않으면 `defunct` (= `zombie`, process가 죽었음에도 디스크 공간 여전히 차지)  
```c
child의 status : running -> zombie -> termination
zombie가 되는 경우
1. child가 끝나기 전 parent가 없어짐 // `init` reap childs
2. child 종료 후에 wait()
// child가 무한루프면 init에 의해 reaped되지도 않음, 인위적으로 kill해줘야 함
```
```c
int wait(int *child_status)	// returns pid -> 어떤 child가 죽었는지 확인 가능
// child의 EXIT status를 넘겨받으면 terminate child
// if (child_status != NULL)
// WIFEXITED, WEXITSTATUS -> exit status 확인 가능
// WIFSIGNALED, WTERMSIG, WIFSTOPPED, WSTOPSIG, WIFCONTINUED -> child가 죽은 이유 등 확인 가능

pid_t waitpid(pid_t pid, int &status, int options)
// pid = -1 이면 임의의 pid

int execve(char *filename, char *argv[], char *envp[])
```

