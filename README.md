# basic-IoTNetwork-2024
IoT 개발자과정 IoT네트워크 프로그래밍 리포지토리

## 1일차(2024-06-11)
- 네트워크의 구성
    - cmd > ipconfig
    - IPv4
    - IPv6 - IPv4가 고갈되었을때를 대비하여 만들어둔 것
    - 기본 게이트웨이 - 컴퓨터로 들어오는 출입문
    
    - 전송하는 방식
        - TCP 
        - UDP

- 소말리아
    - 소 -> 소켓(socket)
    - 말 -> 바인더(bind)
    - 리 -> 리슨(listen)
    - 아 -> 엑셉트(accept)

- 네트워크 프로그래밍이란?
    - 소켓이란 것을 기반으로 프로그래밍을 하기 때문에 소켓 프로그래밍이라고도 함.
    - 네트워크로 연결된 둘 이상의 컴퓨터 사이에서의 데이터 송수신 프로그램의 작성을 의미함.

- 소켓
    - 소켓에 대한 간단한 이해
        - 네트워크(인터넷)의 연결 도구
        - 운영체제에 의해 제공이 되는 소프트웨어적인 장치
        - 소켓은 프로그래머에게 데이터 송수신에 대한 물리적, 소프트웨어적 세세한 내용을 신경 쓰지 않게 한다.

    - 소켓의 생성과정
        - 1단계 - 소켓생성
        - 2단계 - IP주소와 PORT번호 할당
        - 3단계 - 연결요청 가능상태로 변경
        - 4단계 - 연결요청에 대한 수락

- 윈도우 기반으로 구현(pass)

- 프로토콜 체계
    - PF_INET만 알면됨
    - PF_INET - IPv4 인터넷 프로토콜 체계

- 소켓의 타입
    - 연결지향형 소켓(TCP)
        - 중간에 데이터가 소멸되지 않고 목적지로 전송된다.
        - 전송 순서대로 데이터가 수신된다.
        - 전송되는 데이터의 경계가 존재하지 않는다.
    - 비연결지향형 소켓(UDP)
        - 전송된 데이터는 손실의 우려가 있고, 파손의 우려가 있다.
        - 전송되는 데이터의 경계가 존재.
        - 한번에 전송할 수 있는 데이터의 크기가 제한된다.

- 인터넷 주소
    - IPv4 - 4바이트 주소체계
    - IPv6 - 16바이트 주소체계

- 클래스 별 네트워크 주소와 호스트 주소의 경계
    - 클래스 A의 첫번째 바이트 범위 0~127
    - 클래스 B의 첫번째 바이트 범위 128~191
    - 클래스 C의 첫번째 바이트 범위 192~223

- 구조체 sockaddr_in의 멤버
    - 멤버 sin_family
        - 주소체계 정보저장
        - AF_INET - IPv4 인터넷 프로토콜에 적용하는 주소체계
        - AF_INET6 - IPv6 인터넷 프로토콜에 적용하는 주소체계
        - AF_LOCAL - 로컬 통신을 위한 유닉스 프로토컬의 주소체계
    - 멤버 sin_port
        - 16비트 PORT번호 저장
        - 네트워크 바이트 순서로 저장
    - 멤버 sin_addr
        - 32비트 IP주소 정보저장
        - 네트워크 바이트 순서로 저장
        - 멤버 sin_addr의 구조체 자료형 in_addr 사실상 32비트 정수자료형
    - 멤버 sin_zero
        - 특별한 의미를 지니지 않는 멤버
        - 반드시 0으로 채워야 한다.

``` C
    serv_addr.sin_famliy=AF_INET; // IPv4 인터넷 프로토콜을 적용하겠다는 의미
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));
```

``` C
    printf("전달하는 인자개수 : %d\n", argc);
            printf("실행파일 : %s\n", argv[0]);
            
            for(int i = 1; i < argc; i++) {
                    printf("전달인자: %s\n", argv[i]);
            }
```
- 실행결과
        <img src="https://raw.githubusercontent.com/been2525/basic-IoTNetwork-2024/main/image/example001.png" width="730">

- CPU가 데이터를 메모리에 저장하는 방식
    - 빅 엔디안 - 상위 바이트의 값을 작은 번지수에 저장하는 방식 - 하위 바이트값을 큰 번지수에 저장
    - 리틀 엔디안 - 상위 바이트의 값을 큰 번지수에 저장하는 방식 - 하위 바이트값을 작은 번지수에 저장

- 바이트 순서의 변환
    - unsigned short htons(unsigned short);
    - unsigned short ntohs(unsigned short);
    - unsigned long htonl(unsigned long);
    - unsigned long ntohl(unsigned long);
        - htons에서 h는 호스트 바이트 순서를 의미
        - htons에서 n은 네트워크 바이트 순서를 의미
        - htons에서 s는 자료형 short를 의미
        - htonl에서 l은 자료형 long을 의미

## 2일차 (2024-06-12)
- TCP/IP 프로토콜 스택
    - 인터넷 기반의 데이터 송수신을 목적으로 설계된 스택
    - 큰 문제를 작게 나눠서 계층화 한 결과
    - 데이터 송수신의 과정을 네개의 영역으로 계층화 한 결과
    - 각 스택 별 영역을 전문화하고 표준화함
    - 7계층으로 세분화가 되며, 4계층으로도 표현함

- LINK 계층의 기능 및 역할
    - 물리적인 영역의 표준화 결과
    - LAN, WAN, MAN과 같은 물리적인 네트워크 표준 관련 프로토콜이 정의된 영역

- IP 계층의 기능 및 역할
    - IP는 Internet protocol을 의미
    - 경로의 설정과 관련이 있는 프로토콜

- TCP/UDP 계층의 기능 및 역할
    - 실제 데이터의 송수신과 관련 있는 계층
    - 전송계층으라고도 함.
    - TCP는 데이터의 전송을 보장하는 프로토콜, UDP는 보장하지 않는 프로토콜

- APPLICATION 계층
    -

- 연결요청 대기상태로의 진입
```c
#include <sys/type.h>

int listen(int sock, int backlog);
```

- 클라이언트의 연결요청 수락
```c
#include <sys/socket.h>

int accept(int sock,struct sockaddr * addr, socklen_t * addrlen);
```

- Tcp 클라이언트의 기본적인 함수호출 순서
    1. socket() - 소켓생성
    2. connect() - 연결요청
    3. read()/write()  - 데이터 송수신
    4. close() - 연결종료
```c
#include <sys/socket.h>

int connect(int sock, const struct sockaddr * servaddr, socklen_t addrlen);
```

- TCP 소켓에 존재하는 입출력 버퍼
    - 입출력 버퍼는 TCP 소켓 각각에 대해 별도로 존재한다.
    - 입출력 버퍼는 소켓생성시 자동으로 생성된다.
    - 소켓을 닫아도 출력버퍼에 남아있는 데이터는 계속해서 전송이 이뤄진다.
    - 소켓을 닫으면 입력버퍼에 남아있는 데이터는 소멸되어버린다.

- UDP 소켓의 특성과 동작원리
    - UDP 소켓은 SEQ,ACK와 같은 메시지 전달을 하지않는다.
    - 그리고 연결의 설정과 해제의 과정도 존재하지 않는다.
    - 때문에 데이터의 분실 및 손실의 위험이 있다.
    - 그러나 확인의 과정이 존재하지 않기 때문에 데이터의 전송이 빠르다.
    - 따라서 안전성보다 성능이 중요시 될 때에는 UDP를 사용한다.

- UDP의 데이터 송수신
    - TCP는 1대1의 연결을 필요로 하지만, UDP는 연결의 개념이 존재하지 않는다.
    - 서버 소켓과 클리이언트 소켓의 구분이 없다.