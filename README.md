# RingBuffer
This repository contains the thread safe implementation of  template header only circular buffer

Example:

```C++
int main() {

    buffer::RingBuffer<int> buffer(10, true);
    std::cout << buffer.getCapacity() << std::endl;

    auto res = buffer.enqueue(5);
    res = buffer.enqueue(100);
    res = buffer.enqueue(200);

    auto elem = buffer.front();
    std::cout << elem << std::endl;


    int fromBuffer{-1};
    res = buffer.dequeue(fromBuffer);
    std::cout << fromBuffer << std::endl;


    res = buffer.dequeue(fromBuffer);
    std::cout << fromBuffer << std::endl;

    return 0;
}
```
