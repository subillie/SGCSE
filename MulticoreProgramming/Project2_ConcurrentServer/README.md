# Concurrent Server

## Event-based

[[Reference] System Programming](https://velog.io/@tonyhan18/%EC%8B%9C%EC%8A%A4%ED%85%9C-%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D-13-1-Concurrent-Programming)  

- 단일 process, connfd or listenfd를 포함한 array를 가지고 있음
- Determine which file descriptor (connfd's or listenfd) has pending inputs
	- check fds using `select` or epoll function
	- arrival of pending input is an `event`
	- 해당 input에 맞는 action 수행
		- If `listenfd` has input, `accept` connection  
			-> then add new connfd to array
		- `Service` all `connfd's` with pending inputs

[[Reference] Event-based Concurrency](https://velog.io/@kmin-283/Event-based-Concurrency-Advanced)  

어떤 일이 일어날 때까지 기다리는 것, event가 발생하면 event의 type에 따라 `event handler`로 동작을 수행한다.  
즉, 어떤 event를 수행할 것인지에 대한 행동이 마치 scheduling과 같은 기능을 한다.  
Node.js같은 server-side framework, 고성능 웹서버나 검색엔진에서 자주 쓰이는 방법이다.
- Pros
	- Can single-step with debugger (control overhead가 없음)
	- `One logical control flow and address space`
		- 단일 thread이고 event에 따라 처리하기 때문에 lock이 필요 없고 deadlock이 발생하지 않으며, concurrent program에서 나타나는 문제들이 없음
- Cons 
	- Significantly more complex to code than process or thread based designs (구현 난이도가 높음)
	- Hard to provide fine-grained concurrency
		- fine-graind concurrency를 제공하지 않는다. server에 접속한 client를 handling하다가 중간에 다른 file descriptor에 온 pending input을 처리할 수 없다. 그래서 [coarse-graind](https://icthuman.tistory.com/entry/FineGrained-vs-CoarseGrained)하게 처리한다. 그래서 `한 작업이 계속 작업중이면 다른 pending input을 처리하지 못 한다`.
	- Cannot take advantage of multi-core, because of single thread of control  
		e.g. block되는 system call을 호출하는 event가 발생한 경우 문제 발생 (file을 읽어서 내용을 return하는 경우 등등)
		- problem :  
			thread-based server에서는 한 thread가 파일을 열고 읽는 동안 다른 thread가 프로그램을 구동하면 되지만, event-based server에서는 thread가 하나 뿐이기 때문에 block이 발생하고, 모든 서버의 작업이 block 동안 멈추게 된다.
		- solution :  
			Asynchronous I/O control block

## Thread-based
[[Reference] System Programming](https://velog.io/@tonyhan18/%EC%8B%9C%EC%8A%A4%ED%85%9C-%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D-13-1-Concurrent-Programming)  