#include "stdafx.h"
#include "Source\Track\BorderTrackBase.hpp"
#include "Source\Track\CenterTrackBase.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"


BorderTrackBase::BorderTrackBase(std::list<BorderTrackSegment> const & listOfBorderTrackSegments)
	: mListOfBorderTrackSegments(listOfBorderTrackSegments)
{
}



BorderTrackBase& BorderTrackBase::doForAllSectors(std::function<void(BorderTrackSegment &, BorderTrackSegment &)> task)
{
	if (!mListOfBorderTrackSegments.empty())
	{
		std::list<BorderTrackSegment>::iterator itOne = mListOfBorderTrackSegments.begin();
		std::list<BorderTrackSegment>::iterator itTwo = itOne;
		++itTwo;

		while (itTwo != mListOfBorderTrackSegments.end())
		{
			task(*itOne, *itTwo);
			++itOne;
			++itTwo;
		}
		task(mListOfBorderTrackSegments.back(), mListOfBorderTrackSegments.front());
	}
	return *this;
}


CenterTrackBase BorderTrackBase::getCenterTrackBase() const
{
	std::function<CenterTrackSegment(BorderTrackSegment const &, BorderTrackSegment const &)> trafoBorderToCenterSegments = 
		[](BorderTrackSegment const & segment1, BorderTrackSegment const & segment2) -> CenterTrackSegment
	{
		sf::Vector2f center = mySFML::Simple::meanVector(segment1.first, segment1.second);
		sf::Vector2f relVec = (segment1.first - segment1.second) / 2.f;
		return std::make_pair(center, relVec);
	};

	std::list<CenterTrackSegment> listOfCenterTrackSegments(this->calculateForAllSectorsAndPutIntoList(trafoBorderToCenterSegments));
	return CenterTrackBase(listOfCenterTrackSegments);
}


std::list<BorderTrackSegment> const & BorderTrackBase::getListOfBorderTrackSegments() const
{
	return mListOfBorderTrackSegments;
}
void BorderTrackBase::setListOfBorderTrackSegments(std::list<BorderTrackSegment> const & listOfBorderTrackSegments)
{
	mListOfBorderTrackSegments = listOfBorderTrackSegments;
}




BorderTrackBase::const_iterator BorderTrackBase::getIteratorToNearestSegment(sf::Vector2f const & pos) const
{
	bool first = true;
	float smallestDist;
	BorderTrackBase::const_iterator nearestSegment;
	for (BorderTrackBase::const_iterator it = this->cbegin(); it != this->cend(); ++it)
	{
		sf::Vector2f center = mySFML::Simple::meanVector(it->first, it->second);
		if (first)
		{
			first = false;
			nearestSegment = it;
			smallestDist = mySFML::Simple::lengthOf(pos - center);
		}
		else
		{
			float newDist = mySFML::Simple::lengthOf(pos - center);
			if (newDist < smallestDist)
			{
				smallestDist = newDist;
				nearestSegment = it;
			}
		}
	}
	return nearestSegment;
}



std::ostream& operator<<(std::ostream & stream, BorderTrackBase const & borderTrackBase)
{
	for (auto const & borderTrackSegment : borderTrackBase.mListOfBorderTrackSegments)
	{
		stream << borderTrackSegment.first.x << '\t' << borderTrackSegment.first.y << '\t' << borderTrackSegment.second.x << '\t' << borderTrackSegment.second.y << std::endl;
	}
	return stream;
}

