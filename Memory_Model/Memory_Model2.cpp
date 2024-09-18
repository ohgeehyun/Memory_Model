#include <iostream>
#include <windows.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>

using namespace std;
using int32 = __int32;
using int64 = __int64;
using uint64 = unsigned __int64;

atomic<bool> ready;
int32 value;

void Producer() {

	value = 10;

	ready.store(true,memory_order::memory_order_seq_cst);
}

void Consumer() {
	while (ready.load(memory_order_seq_cst) == false)
		;
	cout << value << '\n';
}

int main2() {
	// Memory Model(정책)
	// 1) Sequentially consistent (seq_cst)
	// 2) Acquire-Release (acquire,release)
	// 3) Relaxed(relaexd)
	// 내려갈수록 자유롭다.
	// 1) seq_cst (가장 엄격 = 컴파일러 최적화 여지 적음 = 직관적) 가시성 및 코드 재배치 해결
	// 2) acquire-release 딱 중간 특징으로는 짝을 맞추어서 사용해야함 memory_order_release 와 memory_orderacquire을 짝을맞춰사용
	// rellease의 의 경우 release명령 이전 메모리 명령들이 , 해당 명령 이후로 재배치 되는 것을 금지.(절취선 cpu마음대로 코드 재배치를 이 절취선 이후에는 하지마라)
	// 그리고 acquire로 같은 변수를 읽는 쓰레드가있다면 release 이전의 명령들이 -> acquire하는 순간에 관찰 가능(가시성 보장)
	// 3) relaxed(자유롭다 = 컴파일러 최적화 여지 많음. 직관적이지않음.) 가시성 해결 no 코드재배치도 cpu가 해버림. 가장 기본 조건 (동일 객체에 대한 관전 순서만 보장)
	
	// 기본적 atomic 은 seq_cst를 사용하여 가장 엄격하게 작업을한다. 
	// 근래 intel이나 amd cpu의 경우  애당초 순차적 일관성을 보장해서 seq_cst를 써도 별다른 부하가 없다.
	// ARM(보통 임베디드에서 사용)의 경우 꽤 차이가 있다고 한다.
	// 그럼 atomic만 메모리 모델 을 사용할수있는가? 그건 아니라고 한다. cpu자체에 cpu 베리어라는 기능이 있기 때문에 atomic_thread_fence(메모리정책); 이런 식으로 사용 가능하지만 굳이 ?
	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();

}