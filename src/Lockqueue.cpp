#include"Lockqueue.h"
template<typename T>
void Lockqueue<T>::push(T data)
{
    std::lock_guard<std::mutex>(mt);
    line.push(data);
    cont.notify_one();
}
template<typename T>
T Lockqueue<T>::pop()
{
    std::unique_lock<std::mutex>(mt);
    while(line.size()==0)
    {
        cont.wait(mt);
    }
    T data=line.front();
    line.pop();
    return data;
}