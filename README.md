# Updatable priority queue

## TL;DR
This repository provides an implementation of a high-performance updatable priority queue in C++.

### Example

```c++
better_priority_queue::updatable_priority_queue<int,int> pQ;
pQ.push(0, 30);   // or pQ.set(0, 30)
pQ.push(1, 20);
pQ.push(2, 10);

pQ.update(2, 25); // or pQ.set(2, 25)

while(!pQ.empty())
	std::cout << pQ.pop_value().key << ' ';
```
Output :
```
0 2 1 
```

### Important note

As of now, the priority queue can only take values that can be converted to `size_t` as keys, for performance reasons.  
(This allows using a vector instead of an unordered map.)

## Typical C++ replacements for an updatable priority queue
The typical use of a priority queue in C++ is to create a `priority_queue<pair<Priority, Key>>` where `Priority` is the type of your priority (`int`, `float`,...) and `Key` is the value of whatever you want to assign a priority to.

It always bothered me that the `std::priority_queue` data structure did not provide functions to update the priorities, and that I couldn't find a proper implementation that would do exactly this.
How could it though, considering there is no *key/value* notion in the `priority_queue` itself ?

Hence, the typical C++ management of problems that would require such updates would be either :
- to use a `set<pair<Priority, Key>>`, an update then would be to remove the old element and insert a new one.
- to add values in the `priority_queue<pair<Priority, Key>>` without removing the previous invalid ones, and checking when popping them whether they correspond to the most recent priority value.

For instance, this is how we would implement dijkstra :
- With a `set`
```c++
double set_dijkstra(const vector< vector<edge> > &graph, int source, int target) {
	vector<double> min_distance( graph.size(), 1e9 );
	min_distance[ source ] = 0;
	set< pair<double,int> > active_vertices;
	active_vertices.insert( {0,source} );

	while (!active_vertices.empty()) {
		int where = active_vertices.begin()->second;
		if (where == target) return min_distance[where];
		active_vertices.erase( active_vertices.begin() );
		for (auto edge : graph[where]) 
			if (min_distance[edge.to] > min_distance[where] + edge.length) {
				active_vertices.erase( { min_distance[edge.to], edge.to } );
				min_distance[edge.to] = min_distance[where] + edge.length;
				active_vertices.insert( { min_distance[edge.to], edge.to } );
			}
	}
	return 1e9;
}
```

- With a `priority_queue` 
```c++
double pq_dijkstra(const vector< vector<edge> > &graph, int source, int target) {
	vector<double> min_distance( graph.size(), 1e9 );
	min_distance[ source ] = 0;
	priority_queue< pair<double,int> > active_vertices;
	active_vertices.push( {0,source} );

	while (!active_vertices.empty()) {
		int where = active_vertices.top().second;
		int d = -active_vertices.top().first;
		active_vertices.pop();
		if(min_distance[where] < d) continue;
		if (where == target) return min_distance[where];
		for (auto edge : graph[where]) 
			if (min_distance[edge.to] > min_distance[where] + edge.length) {
				min_distance[edge.to] = min_distance[where] + edge.length;
				active_vertices.push( { -min_distance[edge.to], edge.to } );
			}
	}
	return 1e9;
}
```
This second solution performs well on a dijkstra (2 times faster than the set version, because of the depth of the data structure) because all of the updates go the same way (a priority increase), and there are actually not that many updates made.

## The updatable priority queue library

I recently had to implement an algorithm that I felt would perform way better with a proper updatable priority queue implementation : there were many updates, going both up and down.

This is why I implemented this library. The idea is to store in an additional array the position in the heap of every value. 

It also eases implementation of typical algorithms such as dijkstra.  
This is how dijkstra would be implemented with any updatable priority queue :

```c++
double updatable_pq_dijkstra(const vector< vector<edge> > &graph, int source, int target) {
	vector<double> min_distance( graph.size(), 1e9 );
	min_distance[ source ] = 0;
	better_priority_queue::updatable_priority_queue<unsigned long,double> active_vertices;
	active_vertices.set( source, 0 );

	while (!active_vertices.empty()) {
		int where = active_vertices.pop_value().key;
		if (where == target) return min_distance[where];
		for (auto edge : graph[where]) 
			if (min_distance[edge.to] > min_distance[where] + edge.length) {
				min_distance[edge.to] = min_distance[where] + edge.length;
				active_vertices.set( edge.to, -min_distance[edge.to] );
			}
	}
	return 1e9;
}
```

Thanks to the functions that are present in this library, this is how you would actually implement it :

```c++
double updatable_pq_dijkstra(const vector< vector<edge> > &graph, int source, int target) {
	better_priority_queue::updatable_priority_queue<unsigned long,double> pQ;
	pQ.set( source, 0 );
	while (!pQ.empty()) {
		auto pq_node = pQ.pop_value(true); // Remember this node and never add it again
		if (pq_node.key == target) return -pq_node.priority;
		for (auto edge : graph[pq_node.key]) 
			pQ.set(edge.to, pq_node.priority-edge.length, true);
			// set to this priority only if it is an increase
			// returns true if an update was made,
			// so you can save the parents and reconstitute the paths
	}
	return 1e9;
}
```

### Performance
All 4 dijkstra implementations were timed on a random euclidian graph, and it appears that :
- All 3 heap implementations are as fast (`stl's` is a tiny bit faster).
- The `set` implementation takes twice as much time as the heap implementation.

It is however very likely that this library would be much faster than using the basic `std::priority_queue` when implementing an algorithm where priorities would not only increase, but is still untested at this point.
