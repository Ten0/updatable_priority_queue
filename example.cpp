#include <iostream>
#include "updatable_priority_queue.h"

int main() {
	better_priority_queue::updatable_priority_queue<int,int> pQ;

	pQ.push(0, 30); // inserts key 0 at priority 30
	//           {0, 30}

	pQ.push(0, 29); // does nothing because key is already in
	//           {0, 30}

	pQ.set (1, 11); // inserts key 1 at priority 11
	//           {0, 30}
	//      {1, 11}

	pQ.set (1, 10); // updates key 1 : set prority to 10
	//           {0, 30}
	//      {1, 11}

	pQ.push(2, 20);
	//           {0, 30}
	//      {1, 11}  {2, 20}
	pQ.push(3, 15);
	//           {0, 30}
	//      {3, 15}  {2, 20}
	//  {1, 11}

	pQ.update(1, 21);
	//           {0, 30}
	//      {1, 21}  {2, 20}
	//  {3, 15}
	
	while(!pQ.empty()) {
		std::cout << pQ.pop_value().key << std::endl;
	}
	// Outputs 0, 1, 2, 3

	return 0;
}
