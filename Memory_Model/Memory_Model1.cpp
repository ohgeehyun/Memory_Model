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

int32 x = 0;
int32 y = 0;
int32 r1 = 0;
int32 r2 = 0;

atomic<bool> flag = false;
int main()
{
    
    bool lockcheck = flag.is_lock_free();//cpu에서 원자적으로 처리할수있는지 없는지 확인 true = 락이있기때문에 원자적으로 처리불가능 false = 락이 없기때문에 처리가능

    //flag = true; 사용가능하긴하지만 다음 사용자가 보았을때 이 flag가 atomic인지 아닌지 판단이 서지않음 전용 멤버함수를사용하자.
    flag.store(true,memory_order::memory_order_seq_cst);//2번쨰로 받은 인자가 바로 메모리 모델
  
  
    //bool var = flag;   
    bool var = flag.load(memory_order::memory_order_seq_cst);
    
    //이전 flag 값을 prev에 넣고, flag 값을 수정
    {
        bool prev = flag.exchange(true); //읽고 쓰기를 동시에 인자로받은 값을 저장하고 전에 있던 데이터를 반환
        /*
        내부적으로 prev flag값을 넣을려고할때 만약 다른 스레드가 flag에 값을 바꿔버린다면? 고로 exchange를통해 원자성을 보장해야한다. 
        bool prev = flag;
        flag = true;
        */
    }

    //CAS (Compare-And-Swap) 조건부 수정
    {
        bool expected = false;
        bool desired = true;
        flag.compare_exchange_strong(expected,desired);

        /*compare_exchange_strong의 의사코드  
        if (flag == expected)
        {
            expected =false;
            flag = desired;
            return true;
        }
        else 
        {
            expected = flag;
            return false;
        }
        
        compare_exchange_stong과 weak의 차이 동작방식은 같지만 내부적으로 이런저런 이유로 다른 스레드의 이유로 interruption받아서 중간에 실패하면 성공했음에도 false가 나왔다.
        이것을 Suprious Falure(가짜 실패?)라고한다.
        strong은 실패할시 성공할떄까지 더 돌아서 결과를 반환해준다.
        weak는 사용하고싶으면 보통은 while문과 같이사용하여 가짜실패시 성공을 반환하게한다.*/

        flag.compare_exchange_weak(expected, desired);


    }


}


