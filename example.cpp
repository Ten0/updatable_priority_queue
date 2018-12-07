#include <iostream>
#include "updatable_priority_queue.h"

int main() {
	priority_queue::updatable_priority_queue<int,int> pQ;
	pQ.push(0, 30);
	pQ.push(1, 10);
	pQ.push(2, 20);
	pQ.push(3, 15);

	pQ.update(1, 16);
	
	// {0, 30}, {2, 20}, {1, 16}, {3, 15}
	
	while(!pQ.empty()) {
		std::cout << pQ.pop_value().key << std::endl;
	}

	return 0;
}
