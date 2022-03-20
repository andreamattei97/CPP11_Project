#include "UserGraphData.hpp"

int UserGraphData::GetOwner() const
{
	return mOwner;
}

UserGraphData::UserGraphData(int owner)
{
	mOwner = owner;
}


