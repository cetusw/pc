#include <iostream>

#include "MyTask.h"

MyTask SimpleCoroutine()
{
    co_return "Hello from coroutine!";
}

int main()
{
	const MyTask task = SimpleCoroutine();
    std::cout << task.GetResult() << std::endl;
}
