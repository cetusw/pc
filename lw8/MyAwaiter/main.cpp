#include <iostream>

#include "MyAwaiter.h"
#include "MyTask.h"

MyTask CoroutineWithAwait(const int x, const int y)
{
	std::cout << "Before await\n";
	const int result = co_await MyAwaiter{ x, y };
	std::cout << result << "\n";
	std::cout << "After await\n";
}

int main()
{
	const auto task = CoroutineWithAwait(30, 12);
	std::cout << "Before resume\n";
	task.Resume();
	std::cout << "After resume\n";
	CoroutineWithAwait(5, 10).Resume();
	std::cout << "End of main\n";
}
