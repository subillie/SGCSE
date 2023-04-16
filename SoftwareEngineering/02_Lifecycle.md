# Software Lifecycle Model  
## Software Process
- A series of steps through which a software product progresses
- Consists of `people`, `overall process`, `intermediate products`, `stages of the process`  
```  
Usual Process Phases :  
Requirements -> Analysis -> Design -> Implementation -> Testing -> Maintenance  
```  

## Process Models
### Hacking
아무런 설계 자료나 코멘트 없이, 오로지 코드 중심 개발 `-> 지양!!`  
```  
Focus on 'implementation details' before
- analysis   : understanding the problem
- design     : developing a solution
- objectives : determining success criteria  
```  

### Waterfall
**`sequential with little feedback`**  
- design the system completely, consistently, and unambiguously on the first attempt  
1. write all the program interfaces and internal detatils correctly `on the first attempt`  
2. integrate the system components in `one large step`  
3. do `system testing as the last step`  
<img src="https://user-images.githubusercontent.com/112736264/228539435-f1c8935e-5aef-403b-b22f-248b6f9cdbd5.png" width="340" height="170"/>  

###### 위 그림이 절대적 절차는 아님. 회사마다 다름.  
- 장점  
```
- Hacking과 달리, 각각의 선후관계가 지켜짐 :
	develop requirements -> design -> write programs -> integrate them -> test
- have progress review at each step
- establish and control product baselines
```
- 단점  
<img src="https://user-images.githubusercontent.com/112736264/228547302-02e007a9-3aa4-487e-81f0-9d0174e973c0.png" width="340" height="170"/>  

```  
- 고객은 처음부터 모든 필요사항을 다 말해야 함
- 개발자는 user domain에서의 전문가여야 함
- 각각의 선후관계가 꼭 지켜져야 함
	- complete, stable requirements before designing
	- completed design before coding
	- Large integration and testing effort as final step
	- No demonstrations of system capabilities until system is finished
```  

### Incremental  
**`staged development`**  
- Design is partitioned into a series of `prioritized` builds  
```  
- Each build adds capabilities to the existing base in priority order
	- builds produce demonstrable versions
- Most critical parts are built first -> tested, demonstrated most often  
```
<img src="https://user-images.githubusercontent.com/112736264/228720896-a437f018-a661-43e9-b144-ab122136ecdc.png" width="200" height = "150">  

- 예시  
	a six month incremental project -> (Analysis/Design : Implementation = 2 : 4 months)  

|Version|Goal: rework < 20%|bi-weekly demos|
|:---:|:---|:---:|
|0.1|input reader|Demo1|
|0.2|plus lexical analyzer|Demo2|
|0.3|plus symbol table|Demo3|
|0.4|plus code generator|Demo4|
|0.5|plus linker table|Demo5|
|0.6|plus error messages|Demo6|
|0.7|plus optimizer|Demo7|
|0.8|plus writer|Customer Version 1.0|  

- 장점    
	- early and continuing demonstrations of progress
	- `Components built first` are tested most
	- 다음 버전 요구사항 수정 가능, systematic planning 가능
	- can make `trade-offs features and schedule`, during development or at delivery time
		- GOOD: 80% 기능의 완성본 (product delivery on schedule w/ most important features working)
		- BAD: 100% 기능의 미완성본 (nothing to deliver)
	- can provide `incremental deliveries` to customers
		- `early feedback` from users

### Evolutionary  
**`exploratory development`**  
- used when requirements **cannot be specified in advance**
- Using evolution approach indicates a **high-risk project**
```
Evolutionary cycles end when...
canceled when infeasible / product is completed, delivered / converted to incremental approach
```  
<img src="https://user-images.githubusercontent.com/112736264/232274555-4ce40340-9585-4298-9ff2-3e6399b3958a.png" width="280" height = "130">  

### Spiral  
**`risk management and verification`**  
- 장점
	- risk 관리가 포함된 모델
- 단점
	- 프로젝트 기간이 오래 걸림
	- 반복 단계가 길어질수록 프로젝트 관리가 어려움
	- 현재는 잘 사용하지 않음  

<img src="https://user-images.githubusercontent.com/112736264/232275460-55c2689c-6b71-465d-9144-0e15457e515f.png" width="280" height = "130">  

### USDP (Unified Software Development Process)  
**`complex, object-oriented`**  
- a process to work w/ the UML
- 3원칙  
	`use-case driven`, `architecture-centric`, `incremental, iterative`
- `PHASES` structure  
	inception(인지), elaboration(개발), construction(건축), transition(전환)  
	<img src="https://user-images.githubusercontent.com/112736264/232276564-f04f6fa1-7078-49f7-a073-94799d0c7151.png" width="250" height = "80"> 
	###### Several iterations make up a phase<br>(= Each phase consissts of several iterations)
- core `WORKFLOWS` structure tasks  
	requirements, analysis, design, implementation, test  

```
'Phases' structure the process 'by time'
'Workflows' structure the process 'by type of task'
```
<img src="https://user-images.githubusercontent.com/112736264/232276813-279bca0a-0279-4826-8f31-a51d7241a8d7.png" width="250" height = "130">  

### XP (Extreme Programming)  
**`light-weight, small and quick`**  
- Agile, lightweight process for `small teams`
- motivation : Change was expensive(옛날) -> quick, cheap(현재)
- `lots of small iterations`
- `based on incremental model`
- `on-site customer` + `open workspace`
	1. Potential features 식별(requirements phase와 유사)
		- Initial features are chosen based on cost-benefit analysis
	2. `Pair programming` : implementation proceeds by programming pairs
	3. Design은 덜 중요
	4. `Ego-less programming` : no specialization among team members
		- Collective owenership (coding standars)
- Iteration  
	- **Customer picks use cases** w/ highest priority  
	- Split them into **tasks**  
	- **Develop test cases** for each task  
	- **Implement task** and **evolve design** of whole system
- Time Scale
	```
	Test case(hours) -> Task(days) -> Iteration(1-4 weeks) -> Release(months)
	```
- 예시  
	```
	Plan together (고객, 프로그래머) -> Contunuous interation -> simplest design -> lots of testing
	(e.g. unit tests before implementation, automic testing, system tests by customer)
	```
- 장점
	- risk를 줄임, low overhead
	- 변하는 요구사항에 대처가 용이함
	- testing, refactoring -> keeps system simple
- 단점
	- requires customer to be present
	- not for big, complex project
	- not for programs that can't be tested automatically
