# Project Phase I: Building and Testing My Shell  

## Extern Functions

**`ls`**  
list the directory contents  

**`mkdir`, `rmdir`**  
create and remove directory using shell  

**`touch`, `cat`**  
create and read the contents of a file  
<br>

## Built-in Commands
*Reference : [Bash Builtin Commands 공식 문서](https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html)

**`cd`**  
navigate the directories in shell  

```c
// 구현한 함수 설명
```
<br>

**`echo`**  
print the contents of a file  
###### Output the args, separated by spaces, terminated with a newline. The return status is 0 unless a write error occurs.
|option||
|:---:|:---|
|-n|the trailing newline is suppressed|
|-e|interpretation of the following backslash-escaped characters is enabled|
|-E|disables the interpretation of these escape characters, <br> even on systems where they are interpreted by default|

```c
// 구현한 함수 설명
```
<br>

**`exit`**  
terminate all the child processes and quit the shell  

###### `exit` 은 exit이란 문자열을 출력하고 주어진 값을 unsigned char 형식으로 exit한다. <br> 이때, 주어진 값은 종료 상태에 저장된다. 만약 인자가 없다면 직전 종료상태로 간주한다.

```shell
> bash
> delphinus@Delphinus-Laptop:~$ exit 25
exit
> echo $?
25
```

```shell
> bash
> delphinus@Delphinus-Laptop:~$ exit 256
exit
> echo $?
0
```

- 문자나 long long을 벗어날 때

```shell
> bash
> delphinus@Delphinus-Laptop:~$ exit a
exit
bash: exit: a: numeric argument required
> echo $?
255
``` 

```shell
> bash
> delphinus@Delphinus-Laptop:~$ exit 55555555555555555555555
exit
bash: exit: 55555555555555555555555: numeric argument required
> echo $?
255
```

- 두 개 이상의 인자가 들어올 경우

```shell
> bash
> delphinus@Delphinus-Laptop:~$ exit 5 2
exit
bash: exit: too many arguments
> delphinus@Delphinus-Laptop:~$ echo $?
1
```

- 두 개 이상의 인자가 들어왔는데 첫 인자가 long long을 벗어나거나 문자일 때

```shell
> bash-3.2$ exit adf 332
exit
bash: exit: adf: numeric argument required
```

```shell
> bash-3.2$ exit 44444444444444444444444444444444444 32
exit
bash: exit: 44444444444444444444444444444444444: numeric argument required
```
<br>

**`history`**  
track shell commands executed since the shell started  
###### 빌트인 함수는 아니지만, 빌트인 함수처럼 구현 및 실행됨 <br> The history command should keep track of command sexecuted since shell was executes. <br> The history comand of the default shell provides many functions, but in this project, only two funcions below need to be implemented.
|||
|:---:|:---|
|!!|print the lastest executed command, then execute the command <br> (It doesn't update history log)|
|!#|print the command on the # line, then execute the command|
