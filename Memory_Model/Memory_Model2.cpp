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
	// Memory Model(��å)
	// 1) Sequentially consistent (seq_cst)
	// 2) Acquire-Release (acquire,release)
	// 3) Relaxed(relaexd)
	// ���������� �����Ӵ�.
	// 1) seq_cst (���� ���� = �����Ϸ� ����ȭ ���� ���� = ������) ���ü� �� �ڵ� ���ġ �ذ�
	// 2) acquire-release �� �߰� Ư¡���δ� ¦�� ���߾ ����ؾ��� memory_order_release �� memory_orderacquire�� ¦��������
	// rellease�� �� ��� release��� ���� �޸� ��ɵ��� , �ش� ��� ���ķ� ���ġ �Ǵ� ���� ����.(���뼱 cpu������� �ڵ� ���ġ�� �� ���뼱 ���Ŀ��� ��������)
	// �׸��� acquire�� ���� ������ �д� �����尡�ִٸ� release ������ ��ɵ��� -> acquire�ϴ� ������ ���� ����(���ü� ����)
	// 3) relaxed(�����Ӵ� = �����Ϸ� ����ȭ ���� ����. ��������������.) ���ü� �ذ� no �ڵ����ġ�� cpu�� �ع���. ���� �⺻ ���� (���� ��ü�� ���� ���� ������ ����)
	
	// �⺻�� atomic �� seq_cst�� ����Ͽ� ���� �����ϰ� �۾����Ѵ�. 
	// �ٷ� intel�̳� amd cpu�� ���  �ִ��� ������ �ϰ����� �����ؼ� seq_cst�� �ᵵ ���ٸ� ���ϰ� ����.
	// ARM(���� �Ӻ���忡�� ���)�� ��� �� ���̰� �ִٰ� �Ѵ�.
	// �׷� atomic�� �޸� �� �� ����Ҽ��ִ°�? �װ� �ƴ϶�� �Ѵ�. cpu��ü�� cpu �������� ����� �ֱ� ������ atomic_thread_fence(�޸���å); �̷� ������ ��� ���������� ���� ?
	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();

}