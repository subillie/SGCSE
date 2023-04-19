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
```c
handler_t *signal(int signum, handler_t *handler)
// default action을 overriding할 수 있음 (= 나의 handler로 바꿀 수 있음)
// except SIGKILL, SIGSTOP
```
- `SIG_IGN`: ignore(무시)
- `SIG_DFL` : revert(재정의) to the default action