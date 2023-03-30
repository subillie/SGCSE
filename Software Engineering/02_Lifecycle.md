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
Hacking focuses attention on implementation details before
- analysis : understanding the problem
- design : developing a solution
- objectives : determining success criteria  
```  

### Waterfall
**`sequential with little feedback`**  
design the system completely, consistently, and unambiguously on the first attempt  
1. write all the program interfaces and internal detatils correctly `on the first attempt`  
2. integrate the system components in `one large step`  
3. do `system testing as the last step`  
<img src="https://user-images.githubusercontent.com/112736264/228539435-f1c8935e-5aef-403b-b22f-248b6f9cdbd5.png" width="340" height="170"/>  

###### 위 그림이 절대적 절차는 아님. 회사마다 다름.  
- Advantages of Waterfall Approach  
```  
- Hacking과 달리, 각각의 선후관계가 지켜짐 :
	develop requirements -> design -> write programs -> integrate them -> test
- have progress review at each step
- establish and control product baselines
```  
- Problems w/ the Waterfall Model  
<img src="https://user-images.githubusercontent.com/112736264/228547302-02e007a9-3aa4-487e-81f0-9d0174e973c0.png" width="340" height="170"/>  
```  
- 고객은 처음부터 모든 필요사항을 다 말해야함
- 개발자는 user domain에서의 전문가여야 함
- 각각의 선후관계가 꼭 지켜져야 함
	- Requirements nust be complete and stable before designing
	- All design must be completed before coding
	- Large integration and testing effort as final step
	- No demonstrations of system capabilities until system is finished
```  

### Incremental  
**`staged development`**  
Design is partitioned into a series of `prioritized` builds  
```  
- Each build adds capabilities to the existing base in priority order
	- builds produce demonstrable versions
- Most critical parts are built first  
	- and tested && demonstrated most often  
```
<img src="https://user-images.githubusercontent.com/112736264/228720896-a437f018-a661-43e9-b144-ab122136ecdc.png" width="200" height = "200">  
###### example (a six month incremental project)  
Analysis/Design 2 months && Implementation 4 months
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

**`Advantages of Incremental Development`**    
- allows early and continuing demonstrations of progress
- `Components built first` are tested most
- can incorporate some changes to requirements in later versions
- can make trade-offs features and schedule
	- during development
	- at delivery time
- can provide incremental deliveries to customers  

### Evolutionary  
**`exploratory development`**  

### Spiral  
**`risk management and verification`**  

### Unified Software Development Process  
**`complex, object-oriented`**  

### Extreme Programming  
**`light-weight, small and quick`**  
