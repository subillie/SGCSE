# Software Architecture
복잡하고 큰 software의 구체적 설계 전에 밑그림을 그리는 작업

Architecture 설계 : Overall 설계, 큰 틀에서의 설계  
COMET method 설계 : Detail한 부분을 설계

큰 틀의 생각없이 설계 -> 피사의 사탑
- 전체 구조의 틀을 잡는 작업 필요  
e.g. 개집, 집, 빌딩을 설계하는 것은 차원이 다른 이슈, 기술, 경험 필요  

해당 설계에 맞는 skill, 공부 필요  
복잡하고 대형 소프트웨어일수록 전체 architecture, 구조를 잡는 기술이 가장 중요  

## Architecture란?
- 아키텍처는 정책이다 (high level에서의 decision이 들어가 있기 때문)
- 소프트웨어 시스템에 대한 상위레벨의 구조적/행위적 모델
- 소프트웨어 솔루션 구조에 대한 커다란 그림을 보여줌
- 어떠한 component들을 연결하여 software를 개발할 것이라고 하는 도면

## 패턴이란?
- 특정 설계 컨텍스트에서 반복해서 발생하는 특정한 문제를 기술하고, 그 해결책으로서 검증된 방법을 제시한다.
- 패턴이름, 문제점, 해결책, 결과 4부분으로 구성됨
- input -> 하나의 obj만 creation되게 하자 -> 경험적 해결법 필요

### 왜 패턴을 익혀야 하는가
Chess master
- 규칙을 익힌다.
- 그 후 원칙들을 익힌다
- 반복되는 패턴을 학습한다.

Software Design Master
- PL, algorithm, data architect와 같은 규칙을 익힌다 (code pattern)
- 프로그래밍 기법, 모듈, 객체지향과 같은(architecture pattern) 원칙을 익힌다.
- 반복적인 문제에 대한 패턴을 많이 아는~~?

### 패턴의 효용
- 실세계의 문제 해결
- 문제영역의 전문지식 전달
- 설계의 결정과 이론적 근거를 문서화
- 마스터들의 지혜와 경험을 재사용

### Style 
Component, Connectors, Constraint로 구성됨  

1. `pipe and filter`
- pipe에서 input이 들어오고 filter에서 일을 처리함의 반복
- sequential하게 진행되는 architecture style

2. `Event based (implicit invocation)`
- 각 module이 announce를 날리면 event가 activate 해당 event에 영향을 받는 module이 일함
- 초분산화, decentralized

3. `layered systems`
- 한 단계씩 쌓아가면서 일을 하는 pattern
- 화살표의 방향은 connect의 방향에 따라 다름
- ISO 7 layer가 여기에 포함

4. `repository`
- 작업 진행 상황을 repository에 공유하면서 각 component들이 일을 하는 pattern

5. `rule-based systems`
- AI쪽에 사용되며 rule 기반으로 작동하는 pattern
- knowledge base : 기반이 되는 rule 
- rule scheduler : knowledge base를 기반으로 rule이 어떻게 실행되어야 하는지 결정
- working memory : 과정의 중간 값을 저장
- rule interpreter : rule scheduler기반으로 output을 내보내고, input이 들어왔을 때 해석하여 요청  

## Architecture
- 다양한 style의 pattern을 가지고 상위레벨의 개발할 software의 밑그림을 그리는 작업
- 같은 기능을 하는 software를 다른 architecture로 구현이 가능

## Software Architecture의 이점

1. System integrity  
전체 틀을 잡으면 software의 원 모습을 유지할 수 있음 (순수성)
2. Control of complexity  
System을 module단위로 분할
3. Predictability  
Software에서 예상되는 문제를 architecture 단에서 미리 예측할 수 있음
4. Testability  
System의 어느 부분을 testing 해야 하는지
5. Reuse  
module등의 재사용성
6. Communication  
개발자와 고객 간의 communication을 용이하게 함
7. Organization and project management  
관리를 용이하게 함
