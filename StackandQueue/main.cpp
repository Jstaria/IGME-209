#include <iostream>

using namespace std;

#define max_size 100 // maximum size of stack

struct Stack {
	int values[max_size];
	int count = 0;

	bool push(int value) {
		if (count == max_size) 
			return false;

		values[count] = value;
		count++;
		return true;
	}

	int pop() {
		if (count == 0) 
			return -1;

		count--;
		return values[count];
	}

	void print() {
		cout << "Stack elements from bottom to top:";

		for (size_t i = 0; i < count; i++)
		{
			cout << values[i] << " ";
		}
		cout << endl;
	}
};

struct Queue {
	int values[max_size];
	int count = 0;

	bool Enqueue(int value) {

		if (count == max_size)
			return false;

		values[count] = value;
		count++;
		return true;
	}

	int Dequeue() {
		if (count == 0)
			return false;

		int firstValue = values[0];

		for (size_t i = 0; i < count - 1; i++)
		{
			values[i] = values[i + 1];
		}

		count--;

		return firstValue;
	}

	void print() {
		cout << "Queue elements from bottom to top:";

		for (size_t i = 0; i < count; i++)
		{
			cout << values[i] << " ";
		}
		cout << endl;
	}
};

//int main() {
//	Stack stack;
//
//	stack.push(1);
//	stack.push(2);
//	stack.push(3);
//	stack.push(4);
//	stack.push(5);
//	stack.print();
//
//	stack.pop();
//	stack.pop();
//	stack.print();
//
//	stack.push(5);
//	stack.print();
//
//	Queue queue;
//
//	queue.Enqueue(1);
//	queue.Enqueue(2);
//	queue.Enqueue(3);
//	queue.Enqueue(4);
//	queue.Enqueue(5);
//	queue.print();
//
//	queue.Dequeue();
//	queue.Dequeue();
//	queue.print();
//
//	queue.Enqueue(5);
//	queue.Dequeue();
//	queue.print();
//}