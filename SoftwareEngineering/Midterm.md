1. 소프트웨어 공학이란?  
To build software system  
root on basis of computer science knowledge  
with an economic context  
within a society context  
further society to support people  

2. Software Crisis에 대해 설명해라 (1968 NATO)  
낮은 생산성으로 인 해 소프트웨어 수요 증가에 비해 공급은 따라갈 수 없는 현상  
- late delivery
- 예산 초과
- inconsistent with the specification
- 이해가 어려움
- modify, improve에 비용 많이 듦  
-> Software engineering should use the engineering disciplines (객체지향 방법론으로 극복 X)  

3. Requirements -> Analysis -> Design -> Implementation -> Testing -> Maintanence  
Analysis : 요구사항을 분석(= 이해), 무엇을 만들어야 하는가  
Design : 분석을 바탕으로 설계, 어떻게 만들어야 하는가  

4. ~ 이런 상황에서 어떤 lift-cycle 모델을 사용할 것인가?  

5. Incremental vs Evolutionary 차이점  

6. 객체 지향 방법론의 주요 원리  
- `Abstraction(추상화)` : 다른 entities로부터 구분되는 주요 특징  
- `Encaptulation(캡술화)` : Hide implementation from clients (Clients depend on interphase)  
- `Modularity` : 복잡한 시스템 단순화  
- `Hierarchy(구조화)` : 같은 계층의 요소들은 abstraction 수준 같아야 함  

7. 객체 지향 기술의 장점  
- 유지 보수가 용이함  
- 잘 설계된 객체 재사용 -> 생산성 up  
- 객체의 추상화 -> 점진적 개발 가능  
오답) 데이터들 사이의 관계를 통합적으로 관리할 수 있음  

8. 객체란?  
현재 상태를 나타내는 값 (entity) 가짐, 유일한 식별자 (고유 identity) 가짐  
Class : a collection of objects with the 'same characteristics'  
Operation(= method) : specification and the implementation of a fuction or procedure by a class  
Attribute : a data item of a class  
Information hiding in software design : hiding a design decision that is considered likely to change  

9. 상속(inheritance)이란?  
a mechanism for sharing and reusing code between classes  
상위 class의 속성이나 오퍼레이션을 하위 class에서 바꾸어 재정의할 수 있다.  
다형성 : 동일한 외부 명령에 대해 각 객체가 서로 다른 방식으로 명령을 수행하는 것  
일반화 : 부분-전체 관계에 있는 class 사이의 관계에서도 정의 가능, 추상화의 한 표현 방법    
상위 class는 하위 class의 공통적인 특징을 정의함  

10. 소프트웨어 개발 방법론  
적용되는 방법론이 다르면 산출물도 다름, 방법론 재사용 가능  
객체지향 방법 : 점진적, 반복적 / 표준은 UML / use case diagram은 requirement 단계  

11. What is carried out during `requirements modeling`?  
Functional requirements of the system are described in terms of actors and use cases.  

12. What is carried out during `analysis modeling`?  
Developing static and dynamic models  

13 Use Case란?  
a sequence of interactions between the user and the system (use case 단계에서는 object 도출 X)  
Actor : an external entity
- interact directly with the system -> initiates actions by system (Actor initiates use cases)
  - Human user
  - External I/O device (to physically interact with system)
  - External system (to physically interact with system)
  - Timer  
`Loosely Coupled` : 각 use case 별로 independent하게 설계하는 게 좋음  
`Strongly Cohesive` : 하나의 use case 내 내용물은 서로 dependent하게 설계하는 게 좋음

14. Documenting Use Cases  
- Name
- Summary (짧은 설명)  
- Dependency (on other use cases)
- Actors
- Preconditions (시작 부분에서 참인 조건)
- Description (basic path(most common sequence)의 얕은 설명)
- Alternatives (alternative path(variants of basic path)의 얕은 설명)
- Postcondition (마지막에 참인 조건)  

15. Use Case Guidelines  
- Consider system as a black box  
- Use active voice (Actor inputs to system -> System performs an action)  
- Use case describes sequence of interactions, not small functions  
- Do not use If-Then-Else and Loops in use case description  

16. Inclusion use case  
used to describe functionality that is common to more than one use case  

17. Extension use case  
used to describe a conditional part of different use case that is only executed under certain circumstances  
(Same use case can be extended in different ways)  

16.

