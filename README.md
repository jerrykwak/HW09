# HW09
[9번 과제] 숫자 야구 게임

# HW09 - 숫자 야구 게임 (Number Baseball Game)

Unreal Engine 5.5를 사용하여 제작된 멀티플레이어 숫자 야구 게임입니다.

## 📋 프로젝트 개요

숫자 야구 게임은 서버가 생성한 3자리 비밀 번호를 플레이어들이 추측하여 맞히는 게임입니다. 각 플레이어는 제한된 횟수 내에서 숫자를 추측해야 하며, 실시간 채팅을 통해 다른 플레이어들과 소통할 수 있습니다.

## 🎮 게임 규칙

### 기본 규칙
- 서버가 1~9 사이의 서로 다른 숫자 3개로 구성된 비밀 번호를 생성
- 플레이어들은 채팅을 통해 3자리 숫자를 입력하여 추측
- 각 플레이어는 최대 3번까지 추측 가능
- 0은 사용할 수 없으며, 중복된 숫자도 불가능

### 판정 시스템
- **Strike (S)**: 숫자와 위치가 모두 정확한 경우
- **Ball (B)**: 숫자는 맞지만 위치가 틀린 경우
- **OUT**: 해당하는 숫자가 하나도 없는 경우

### 승부 조건
- **승리**: 3 Strike를 달성한 플레이어
- **무승부**: 모든 플레이어가 추측 횟수를 모두 소진한 경우

## 🏗️ 프로젝트 구조

```
HW09/
├── Source/HW09/
│   ├── Game/
│   │   ├── HW09GameModeBase.cpp/.h     # 게임 로직 및 규칙 관리
│   │   └── HW09GameStateBase.cpp/.h    # 게임 상태 관리
│   ├── Player/
│   │   ├── HW09PlayerController.cpp/.h  # 플레이어 입력 및 네트워크 통신
│   │   └── HW09PlayerState.cpp/.h      # 플레이어 상태 정보
│   └── UI/
│       └── HW09ChatInput.cpp/.h        # 채팅 입력 UI
├── Config/                             # 엔진 설정 파일들
└── Content/                           # 블루프린트 및 에셋들
```

## 🔧 주요 기능

### 네트워킹
- **멀티플레이어 지원**: 여러 플레이어가 동시에 게임 참여 가능
- **서버-클라이언트 구조**: 게임 로직은 서버에서 처리
- **RPC 통신**: 채팅 메시지 및 게임 결과 동기화

### 게임 시스템
- **비밀 번호 생성**: 랜덤한 3자리 고유 번호 생성
- **입력 검증**: 올바른 형식의 숫자만 게임으로 인식
- **실시간 판정**: 즉시 Strike/Ball 결과 표시
- **자동 게임 리셋**: 게임 종료 시 새로운 라운드 시작

### UI 시스템
- **채팅 인터페이스**: 실시간 채팅 및 게임 입력
- **알림 시스템**: 게임 상태 및 결과 표시
- **플레이어 정보**: 이름 및 추측 횟수 표시

## 🚀 시작하기

### 필요 환경
- Unreal Engine 5.5
- Visual Studio 2022 (Windows)
- .NET Framework

### 설치 및 실행
1. 저장소 클론
```bash
git clone [repository-url]
cd HW09
```

2. 프로젝트 파일 생성
```bash
# HW09.uproject 우클릭 -> "Generate Visual Studio project files"
```

3. 프로젝트 빌드
```bash
# Visual Studio에서 HW09.sln 열기
# Build -> Build Solution (Ctrl+Shift+B)
```

4. 게임 실행
```bash
# Unreal Editor에서 Play 버튼 클릭
# 또는 Standalone Game으로 실행
```

## 🎯 사용법

### 게임 시작
1. 게임이 시작되면 플레이어는 자동으로 서버에 접속
2. 접속한 플레이어는 "Player1", "Player2" 등의 이름이 자동 할당
3. 화면 하단의 채팅창을 통해 소통 및 게임 참여

### 숫자 추측
1. 채팅창에 3자리 숫자 입력 (예: "123", "456")
2. Enter 키를 눌러 전송
3. 시스템이 자동으로 판정하여 결과 표시 (예: "123 -> 1S2B")

### 채팅
- 3자리 숫자가 아닌 모든 입력은 일반 채팅으로 처리
- 다른 플레이어들과 실시간 소통 가능

## 📡 네트워크 아키텍처

### 서버 (GameMode)
- 비밀 번호 생성 및 관리
- 플레이어 추측 검증
- 게임 상태 관리
- 승부 판정

### 클라이언트 (PlayerController)
- 사용자 입력 처리
- UI 관리
- 서버와의 RPC 통신

### 복제 시스템
- PlayerState: 플레이어 정보 및 추측 횟수
- GameState: 전체 게임 상태
- 알림 메시지: 모든 클라이언트에 동기화

## 🔍 코드 주요 구조

### HW09GameModeBase
```cpp
// 비밀 번호 생성
FString GenerateSecretNumber();

// 입력 검증
bool IsGuessNumberString(const FString& InNumberString);

// 결과 판정
FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
```

### HW09PlayerController
```cpp
// 서버 RPC - 채팅 메시지 전송
UFUNCTION(Server, Reliable)
void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

// 클라이언트 RPC - 결과 수신
UFUNCTION(Client, Reliable)
void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
```

## 🎨 확장 가능성

- **커스텀 룸 시스템**: 방 생성 및 참여 기능
- **랭킹 시스템**: 승률 및 통계 기능
- **난이도 조절**: 자리 수 및 추측 횟수 변경
- **스킨 시스템**: UI 테마 및 이펙트
- **관전 모드**: 게임 진행 상황 관람

## 📝 라이선스

이 프로젝트는 교육 목적으로 제작되었습니다.

---

**개발 환경**: Unreal Engine 5.5, C++
**장르**: 멀티플레이어 퍼즐 게임
**플랫폼**: PC (Windows, Linux 지원)
