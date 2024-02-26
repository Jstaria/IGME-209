#include <iostream>
#include <stack>
#include <queue>

using namespace std;

int main() {
	
	// Declare variables
	stack<int> intStack;
	queue<int> intQueue;

	// Push integers
	intStack.push(5);
	intStack.push(10);
	intStack.push(15);
	intStack.push(20);
	intStack.push(25);

	// Enqueue variables from stack to queue

	cout << "Stack ints: ";

	while (intStack.size() > 0)
	{
		cout << intStack.top() << " ";
		intQueue.push(intStack.top());
		intStack.pop();

	}

	cout << endl;
	
	// Dequeue and print integers

	cout << "Queue ints: ";

	while (intQueue.size() > 0)
	{
		cout << intQueue.front() << " ";
		intQueue.pop();

	}
}