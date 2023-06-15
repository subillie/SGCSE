# Software Testing

## Testing이란?
- software 전반에 걸쳐 이루어지는 활동
- 개발 활동에 matching되는 testing 활동 존재

## V model 
- 각 test 활동은 각 단계마다 수행
- 앞 개발단계부터 test를 준비해야함

|개발|Testing|
|:---:|:---:|
|아키텍처 설계<br>상세 설계<br>코딩|요구사항을 어떻게 test해야 할지<br>System testing부터 다음 test 준비<br>요구사항 분석이 완료되면 test 항목, 계획 수립|

Software testing 
- 초기: Error 검출이 목적  
- 90s: 예상 결과와 실제결과의 문제를 탐지 -> 기능관점에서 수행  
- 2010s: test에 비기능적 요소 추가  

Testing vs debugging
- Testing : unknown defect 발견
- Debugging : Known defect 수정

Verification vs Validation
- Verification: 어떤 일을 제대로 하고 있는가?
- Validation: 제대로 된 일을 하고 있는가?

```
Error : 설계자의 오류로  
Fault: 시스템에 fault를 심게 되고  
Failure: 시스템의 동작 이상이 감지됨 (사용자의 발견)
```

## Static vs Dynamic testing
`Static` : 구현 단계 이전에도 test 수행 가능
- Static – static analysis : system의 구조, 내용, 의미, 문맥적으로 도구를 이용해 평가
- Static – Review : 내용적 측면에서 평가. 사람이 개입

`Dynamic` : Test code가 별도 존재해야함
- Dynamic – dynamic analysis : 실행 도중에 behavior 분석
- Dynamic – dynamic testing : test item의 수행이 필요

## Test principle

1. It’s impossible to test a program completely 
- 완전한 testing은 불가능하다

2. Testing can’t show that bugs don’t exist 
- 버그가 존재함은 확인할 수 있으나, 버그가 존재하지 않음을 확인할 수는 없다.

3. The more bugs you find, the more bugs there are 
- 결함이 많이 발견되었던 곳이 결함이 제일 많이 발견된다.

4. Pesticide Paradox
- 같은 test case를 계속 사용하면, 결함 발견이 어려워진다.

5. Early Testing 
- 조기 결함 발견이 중요하다
- 개발 단계 뒤에서 발견될수록 고치는 비용이 늘어난다 

6. Absence of errors – fallacy 
- 품질정의 안에 고객의 business need를 충족하는 부분도 포함해야 한다.
7. Testing is context dependent
- 제품마다 오작동에 대한 피해가 다름을 감안하여 test 수행

## Testing
1. Test level
2. Test type
3. Test design approach
4. others

### Unit Testing
Unit : 소프트웨어의 가장 작은 독립적으로 테스트 가능한 단위
- test 수행을 위해 stub, driver가 필요
- code level에서 수행되기 때문에 개발 당사자에 의해 수행

### Test integration
- Big Bang  
Unit을 한번에 통합하고 test 수행
- Top-down or Bottom-up  
위에서부터 또는 아래서부터 통합 진행
- sandwich  
Top-down과 Bottom-up의 조합
- backbone  
중요 모듈부터 시작하여 리스크 순으로 통합  
*SW 아키텍쳐를 고려하여 테스트 대상과 테스트 방법과 통합순서, 테스트 환경의 사전계획 필요

### Test type

`System Testing`
- 전체 시스템을 대상으로 테스트 수행
- 실 환경과 동일하거나 유사한 환경에서 테스트 수행
- operation profile(주요 사용기능)을 기반으로 테스트 수행이 바람직
- 독립적인 테스터에 의해 수행 

`Acceptance Testing`
- 전체 시스템이 실사용 환경이나 그와 유사한 환경에서 테스트됨

`Maintenance Testing`
- 시스템이 릴리즈 된 이후, 시스템 변경이 발생했을 때 수행
- 변경된 부분에 대한 테스트도 중요하지만, 전체시스템에 대한 영향평가 필요  
*`regression test` – 변경으로 인해 잘 수행되던 동작에 결함이 발생되었는지 확인하기 위함

### Test Design approach

1. `Specification-based testing` (Black-box testing)
- 요구사항, 기능명세, 품질요구사항에 기반
- SW의 내부 구조에 대한 지식 필요없음
- 모든 단계에서 적용 가능
- 실제 코드가 얼마나 테스트 되었는지 알 수 없음
- 완전한 테스팅 불가

2. `Structure-based testing` (White-box testing)
- 내부 흐름, 구조 등 SW 구현코드에 기반
- 테스트시에 프로그래밍 스킬 요구
- 주로 개발자의 unit test시 주로 수행
- 경로를 모두 테스트하는 것은 불가능
- 요구사항에 대한 미구현 오류 발견 불가

3. `Experience-based testing`
- tester의 경험을 기반
- error guessing : 어느 부분에서 에러가 많이 나는지 경험적 추측
- exploratory testing : 제품이해와 동시에 test 설계
- checklist-based testing: checklist를 만들어 test
- defect-based test Technique : 시장에서 이미 알려진 결함에 대한 test

4. `Script Testing`
- 사전에 설계, 문서화된 테스트 수행  

5. `Exploratory Testing`
- 정의된 테스트 목적에 대해, 시스템에 대한 학습, 테스트 설계, 테스트 수행이 동시에 이루어짐  

6. `Risk-Based Testing`
- risk가 큰 것에 대해 빨리 testing 진행
- risk exposure = Technical risk * Business risk 
    - Technical risk : 발생 가능성 (likelihood) 
    - Business risk : 영향 (impact)

### Software Testing Process 
- Test planning 
- Test monitoring and control 
- Test analysis 
- Test design 
- Test implementation 
- Test execution 
- Test completion  

*Test Design  
개발자에게 test를 맡기는 것을 지양 -> test 팀을 따로 운영
