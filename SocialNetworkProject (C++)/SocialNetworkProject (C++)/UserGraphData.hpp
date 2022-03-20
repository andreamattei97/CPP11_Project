#ifndef USER_GRAPH_DATA_HPP
#define USER_GRAPH_DATA_HPP

#include <list>

enum TraversalState
{
	NotVisited=0,Enqueued=1,Visited
};
class UserGraphData
{
public:
	UserGraphData(int owner);

	int GetOwner() const;
	int parent1=-1;
	int parent2=-1;
	std::list<int> sons;
	TraversalState state=NotVisited;

private:
	int mOwner;

};

#endif