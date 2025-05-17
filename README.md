<img src="./docs/Logo.png" alt="StartScene" width="100px">

# Shrine Battle

<br>

## Ⅰ. 프로젝트 소개

<img src="./docs/Entry.png" alt="Entry" width="600px">

### 개요

- GAS를 적용한 Unreal 5.4 개인 포트폴리오 입니다.

- 웨이브에 맞춰 적이 등장하고, 이를 무찌르며 모든 웨이브를 클리어 시 승리합니다.



### 기간 및 인원

- 2024.02 ~ 2024.4 (약 2개월)
- 1인 개발

<br>

## II. 참고 영상

### [유튜브 소개 영상](https://youtu.be/ISgb-UXCs9w?si=DeqBLjNdQxQivVgW)


<br>

## III. 기능 상세

### 플레이어 

#### 플레이어 이동 및 구르기
<br/>
<img src="./docs/roll.gif" alt="move" width="600px">

<br/>

#### 플레이어 공격

<br/>
<img src="./docs/BasicAttack.gif" alt="basic attack" width="600px">

<br/>
<일반 공격>
<p>


<img src="./docs/JumpAttack.gif" alt="jump attack" width="600px">

<br/>
<점프 공격>
<p>

<img src="./docs/Skill.gif" alt="skill" width="600px">

<br/>
<스킬 R>
<p>

#### 아이템 소비

<img src="./docs/stone.gif" alt="stone" width="600px">

#### 방어

<img src="./docs/guard.gif" alt="guard" width="600px">

#### 포커싱

<img src="./docs/enemyfocusing.gif" alt="enemyfocusing" width="600px">
<br/>
포커싱 하는 몬스터로 시선을 고정. 마우스를 움직여 포커싱 대상을 변경할 수 있습니다.

### 몬스터
<img src="./docs/enemies.png" alt="enemies" width="600px">

- 근접 몬스터 : 일반 공격과 대기 로직
- 원거리 몬스터 : 원거리 공격과 플레이어와의 거리 유지
- 보스 몬스터 : 높은 체력과 방어 불가능 공격, 일정 체력 소모 시 몬스터 소환


### 게임 로직

3개의 웨이브 마다 몬스터가 스폰되어 플레이어를 공격합니다. 웨이브를 전부 클리어 하기 전에 플레이어 사망 시 실패, 모든 웨이브를 클리어하면 성공입니다.

<img src="./docs/winpage.png" alt="winpage" width="300px">
<img src="./docs/failPage.png" alt="failPage" width="280px">