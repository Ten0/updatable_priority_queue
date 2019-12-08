#include <bits/stdc++.h>
#include "updatable_priority_queue.h"

using namespace std;

std::random_device g;
int randomRange(int l, int h) {
	uniform_int_distribution<int> d(l, h);
	return d(g);
}


int main() {
	for (int iter=0; iter<100; ++iter) {
		const int n = randomRange(5,5555);
		better_priority_queue::updatable_priority_queue<int,int> pQ;
		for (int i=0; i<n; ++i) {
			int prio = randomRange(0, 9999);
			pQ.push(i, prio);
		}
		for (int i=0; i<9; ++i)
			pQ.update(randomRange(0,n), randomRange(0,9999));
		int m = INT_MAX;
		while (!pQ.empty()) {
			const int p = pQ.pop_value().priority;
			assert(m >= p);
			m = p;
		}
	}

	return 0;
}
