# ECF (Exceptional Control Flow)

ECF exists at all levels of a system
|||
|:---:|:---|
|Exceptions|Hardware and operating system kernel software|
|Process Context Switch|Hardware timer and kernel software|
|Signals|Kernel software and application software|
|Nonlocal jumps|Application code|  

## Shell Programs
|||
|:---:|:---|
|sh|Origianl Unix Shell|
|csh/tcsh|BSD Unix C shell|
|bash|"Bourne-Again" shell (default linux shell)|  

## Process Groups
- job (= process group) : processes created in a single command line
	- foreground job은 항상 1개 이상, background job은 항상 0개 이상
- group pid = parent pid
```c
getpgrp() // returns process group of current process
setpgid() // changes process group of a process
```
```bash
> job1; job2; job3; # 한 번에 여러 개의 job을 수행시킬 수 있음 (; || &&)
> /bin/kill -9 (해당 프로세스의 pid)
> /bin/kill -9 -(해당 프로세스의 group pid)
	# Send SIGKILL to every process in process group
```

## Signals
- small message that notifies a process that an event of some type has occurred in the system
- `kill -l` command로 `ID(1-64)` 알 수 있음
- Kernel `sends(== delivers, != receives)` signals  
	-> destination process `receives(== reacts)` signals  
	-> Predefined Default Actions
	1. `Ignore` : do nothing
	2. `Stop(Suspend)` : until restarted by SIGCONT
	3. `Terminate` : with optional core dump -> 시그널 내부적으로 exit 탑재되어 있다는 의미  
		(e.g. SIGINT, SIGFPE, SIGKILL, SIGSEGV, SIGALRM)
	- `Catch` : by executing a user-level function called signal handler
		- `SIGKILL, SIGSTOP은 caught or ignored될 수 없음`

## Signals from Keyboard
- return, exit은 정상 종료 / 나머지는 비정상종료
- `ctrl-c` : SIGINT, terminate process
- `ctrl-z` : SIGTSTP, stop(suspend) process  
-> a process가 아니라 process group에 signal 보냄 (= to every process in the foreground process group)

```bash
> ps w
STAT (process state) Legend
- 첫번째 글자
# S : sleeping
# T : stopped
# R : running
- 두번째 글자
# s : session leader
# + : foreground process group
```

## Pending/Blocked Signals
- `A pending signal is received at most once`
- `Signals are not queued`
	- If a process has a pending signal of type k, <br> then subsequent signals of type k that are sent to that process are discarded  
	- You can't use signals to count events, such as children terminating

|Bit Vertor|set|cleared|
|:---:|:---:|:---:|
|Pending bit vectors|signal is deliverd|signal is received|
|Blocked bit vectors|sigprocmask (signal mask)|sigprocmask (signal mask)|
###### index 번호는 signal과 동일
```
signal 수신 -> pending bit vector sets (to 1) ->
1. context switching이 일어나는 시점
2. OS의 scheduler가 주기적으로 체크
-> set된 bit vector의 interrupt handler로 jump -> kernel -> user code
```

`pnb` = pending & ~blocked (set of pending non-blocked signals)
```c
If (pnb == 0) // pass control to next inst
Else // pnb의 least nonzero bit을 찾아서, 프로세스가 해당 시그널을 수신하도록 함
// -> 특정 action 유발 -> pnb의 모든 nonzero bit에 대해 repeat
```

## Signal Handlers
- separate logical flow (not process)
- runs concurrently with the kmain program
```c
handler_t *signal(int signum, handler_t *handler)
// default action을 overriding할 수 있음 (= 나의 handler로 바꿀 수 있음) except SIGKILL, SIGSTOP
// 'SIG_IGN' : ignore(무시)
// 'SIG_DFL' : revert(재정의) to the default action
// user-level signal handler : 'installing' the handler
// -> executing handler = 'catching' or 'handling' the signal
signal wrapper : using 'sigaction'
1. handler 한 번 설치하면 재설치할 필요 없음
2. slow system call(e.g. read) 중 interrupt 발생하는 경우 고려할 필요 없음
```
<img src="https://user-images.githubusercontent.com/112736264/233095774-4df6b975-6b81-4fc3-b802-e31ddad24f5a.png" width="300" height="150"/>  

## Nested Signal Handlers
시그널 중첩 : handler 함수 내에서 또 다른 handler 함수가 불림 -> recursion (global 변수 사용 불가)  
- Implicit blocking mechanism : 같은 종류의 시그널은 중첩되어 발생 불가 (자동 block 처리)
- Explicit blocking and unblocking mechanism
	```c
	sigprocmask()	// 수행 중 다른 종류의 시그널이 들어왔을 때 수동 blocking
	sigemptyset(&mask)	// mask를 모두 clear (0으로 채움)
	sigfillset(&mask)	// mask를 모두 set (1로 채움)
	sigaddset(&mask, SIGINT)	// mask를 bit vector에 add
	sigdelset(&mask, SIGINT)
	```  
## 6 Guidelines for Writing Safe Handlers
1. handler를 최대한 simple하게 하기
	- handler가 할 일을 일부 main에서 처리 (e.g. Set a `global flag` and return)
2. Call only `async-signal-safe` funcions in handlers
	- reentrant
	- use only variables (i.e. use local stack)
	- non-interruptible by signals
	- 가능 : _exit, write, wait, waitpid, sleep, kill
	- 불가능 : exit, printf, sprintf, malloc
3. Save and restrre `errno` on entry and exit (overwrite하지 않도록)
	```c
	void handler (int sig) {
		int olderrno = errno;
		...
		errno = olderrno;
	}
	```
4. Protect accesses to shared data structures by `temporarily blocking all signals`
5. Declare global variables as `volatile`
	- to prevent compiler from storing them in a register
	- register에 저장하여 최적화하지 말고 매번 memory 영역을 참조해라 -> 속도는 느려질 수 있지만 안전함
6. Declare global flags as `volatile sig_atomic_t`
	- flag : variable that is only read or written (e.g. flag = 1, not flag++)

## Synchronization Error (Race Condition)
```c
int main(int argc, char **argv) {

	int pid;
	sigset_t mask_all, mask_one, prev_one;

	Sigfillset(&mask_all);
	Sigemptyset(&mask_one);
	Sigaddset(&mask_one, SIGCHLD);
	Signal(SIGCHLD, handler);
	initjobs();

	while (1) {
		Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);	// Block SIGCHLD
		if (pid = Fork() == 0) {						// Child process
			Sigprocmask(SIG_SETMASK, &prev_one, NULL);	// Unblock SIGCHLD for another fork from chld
			Execve();
		}
		Sigprocmask(SIG_BLOCK, &mask_all, NULL);		// Parent process
		addjob(pid);									// Add the child to the job list
		Sigprocmask(SIG_SETMASK, &prev_one, NULL);		// Unblock SIGCHLD
	}
	exit(0);
}
```

## Explicitly Waiting for Signals
```bash
> job
# ctrl-z -> stopped (SIGTSTP을 받은 상태)
# bg or fg -> SIGCONT (continue!) -> running
```
```c
while (!pid);			// correct, but wasteful
while (!pid) pause();	// race condition (pause는 어떤 시그널이든 들어오면 풀림)
while (!pid) sleep(1);	// safe, but slow

int sigsuspend(const siget_t *mask)	// atomic(uninterruptable)!!
// 아래와 로직이 동일함 (전 상태를 가지고 있다가 되돌려줌)
sigprocmask(SIG_BLOCK, &mask, &prev);
pause();
sigprocmask(SIG_SETMASK, &prev, NULL);
```
```c
int main(int argc, char **argv) {

	sigset_t mask, prev;
	Signal(SIGCHLD, sigchld_handler);
	Signal(SIGINT, sigint_handler);
	Sigemptyset(&mask);
	Sigaddset(&mask);

	while (1) {
		Sigprocmask(SIG_BLOCK, &mask, &prev);	// Block SIGCHLD
		if (Fork() == 0) exit (0);	// Child
		int pid = 0;
		while (!pid)
			Sigsuspend(&prev);
		Sigprocmask(SIG_SETMASK, &prev, NULL);
		...
	}
	exit(0);
}
```

## Nonlocal Jumps
매우 위험하고 안 좋은 함수지만 signal error handling에 유용함
문제점
- can cause memory leak (no deallocation)
- works within stack discipline
```c
// 꼭 setjmp -> longjmp 순서여야 함
int setjmp(jmp_buf env)
void longjmp(jmp_buf env, int retval)
```