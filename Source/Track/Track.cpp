#include "stdafx.h"
#include "Source\Track\Track.hpp"

#include "Source\Car\Car.hpp"
#include "Source\Collision\CollisionArea.hpp"

#include <array>
#include <functional>


//////////////
//Constructors

//Default Constructor
Track::Track()
	: Track(std::list<BorderTrackSegment>())
{
}

//Constructor for TrackSegment Lists
Track::Track(std::list<BorderTrackSegment> const & listOfTrackSegments, sf::Color const & color)
	: mTrackColor(color)
{
	this->setTrack(listOfTrackSegments);
}

//Construtor for TrackMiddles and Widths Lists
Track::Track(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths, sf::Color const & color)
	: Track(std::move(this->convertIntoListOfBorderTrackSegments(listOfPositionsAndWidths)), color)
{
}


////////
//Render

//Render
void Track::render(sf::RenderWindow* renderWindow)
{
	renderWindow->draw(mVertexArrayOfTrack);
}


////////
//Setter

//Set Track
void Track::setTrack(std::list<BorderTrackSegment> const & listOfTrackSegments)
{
	mListOfTrackSegments = listOfTrackSegments;
	std::cout << "Check for validity of the track: " << this->checkIfTrackIsValid() << std::endl;
	this->refreshVertexArray();
}

//Set Color
void Track::setColor(sf::Color const & color)
{
	mTrackColor = color;
	this->refreshVertexArray();
}


////////
//Getter

//Get Color
sf::Color Track::getColor() const
{
	return mTrackColor;
}

//Get Lists of Lines
std::pair<std::list<Line>, std::list<Line>> Track::getListsOfLines() const
{
	std::list<Line> listOfLines1;
	std::list<Line> listOfLines2;
	std::list<BorderTrackSegment>::const_iterator trackSegmentIt1 = mListOfTrackSegments.begin();
	std::list<BorderTrackSegment>::const_iterator trackSegmentIt2 = ++mListOfTrackSegments.begin();
	while (trackSegmentIt2 != mListOfTrackSegments.end())
	{
		listOfLines1.push_back(Line(trackSegmentIt1->first, trackSegmentIt2->first));
		listOfLines2.push_back(Line(trackSegmentIt1->second, trackSegmentIt2->second));
		++trackSegmentIt1;
		++trackSegmentIt2;
	}
	listOfLines1.push_back(Line(trackSegmentIt1->first, mListOfTrackSegments.front().first));
	listOfLines2.push_back(Line(trackSegmentIt1->second, mListOfTrackSegments.front().second));
	return std::move(std::make_pair(std::move(listOfLines1), std::move(listOfLines2)));
}

////////////////////
//Collision Checking

bool Track::checkCollisionWith(Car const & car) const
{
	sf::VertexArray const & carVertexArray = car.getVertexArrayReference();
	std::array<Line, 4u> arrayOfCarLines{
		Line(carVertexArray[0].position, carVertexArray[1].position),
		Line(carVertexArray[1].position, carVertexArray[2].position),
		Line(carVertexArray[2].position, carVertexArray[3].position),
		Line(carVertexArray[3].position, carVertexArray[0].position)
	};
	std::list<BorderTrackSegment>::const_iterator trackSegIt1 = mListOfTrackSegments.begin();
	std::list<BorderTrackSegment>::const_iterator trackSegIt2 = ++mListOfTrackSegments.begin();
	while (trackSegIt2 != mListOfTrackSegments.end())
	{
		Line line1(trackSegIt1->first, trackSegIt2->first);
		Line line2(trackSegIt1->second, trackSegIt2->second);
		for (auto const & line : arrayOfCarLines)
		{
			if (line1.intersects(line))
			{
				return true;
			}
			if (line2.intersects(line))
			{
				return true;
			}
		}
		++trackSegIt1;
		++trackSegIt2;
	}
	Line line1(trackSegIt1->first, mListOfTrackSegments.begin()->first);
	Line line2(trackSegIt1->second, mListOfTrackSegments.begin()->second);
	for (auto const & line : arrayOfCarLines)
	{
		if (line1.intersects(line))
		{
			return true;
		}
		if (line2.intersects(line))
		{
			return true;
		}
	}
	return false;
}


//////////////////////////
//Private intern functions

//Refresh VertexArray
void Track::refreshVertexArray()
{
	mVertexArrayOfTrack.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	for (auto trackSegment : mListOfTrackSegments)
	{
		mVertexArrayOfTrack.append(sf::Vertex(trackSegment.first, mTrackColor));
		mVertexArrayOfTrack.append(sf::Vertex(trackSegment.second, mTrackColor));
	}
	if (!mListOfTrackSegments.empty())
	{
		mVertexArrayOfTrack.append(sf::Vertex(mListOfTrackSegments.front().first, mTrackColor));
		mVertexArrayOfTrack.append(sf::Vertex(mListOfTrackSegments.front().second, mTrackColor));
	}
}


bool Track::checkIfTrackIsValid() const
{
	//Get Lists Of Lines
	auto listsOfLines = std::move(this->getListsOfLines());
	std::list<Line> listOfLines1 = std::move(listsOfLines.first);
	std::list<Line> listOfLines2 = std::move(listsOfLines.second);

	//Check for overlap between any but not neighboured Lines
	//At first: list1 and list2
	for (auto const & line1 : listOfLines1)
	{
		for (auto const & line2 : listOfLines2)
		{
			if (line1.intersects(line2))
			{
				return false;
			}
		}
	}

	//Create Lambda for usage with both lists
	std::function<bool(std::list<Line> const &)> checkForIntersectionInListOfLines = 
		[](std::list<Line> const & listOfLines) -> bool
	{
		for (std::list<Line>::const_iterator lineIt1 = listOfLines.begin(); lineIt1 != listOfLines.end(); ++lineIt1)
		{
			std::list<Line>::const_iterator lineIt2 = lineIt1;
			if (++lineIt2 == listOfLines.end())
			{
				break;
			}
			//Now check intersection with neighbour
			sf::Vector2f intersectionPoint(0.f, 0.f);
			if (lineIt1->intersects(*lineIt2, intersectionPoint))
			{
				sf::Vector2f sharedPoint(lineIt1->vertex2);
				if (mySFML::Simple::lengthOf(sharedPoint - intersectionPoint) > 1.0E-05f)
				{
					return true;
				}
			}
			while (++lineIt2 != listOfLines.end())
			{
				auto checkIt = lineIt2;
				if (++checkIt == listOfLines.end())
				{
					if (lineIt1 == listOfLines.begin())
					{
						break; //Exclude case in which first line checks with last line
					}
				}
				//Check other intersections
				if (lineIt1->intersects(*lineIt2))
				{
					return true;
				}
			}
		}
		return false;
	};

	//Secondly: list1 and list1
	if (checkForIntersectionInListOfLines(listOfLines1))
	{
		return false;
	}

	//Thirdly: list2 and list2
	if (checkForIntersectionInListOfLines(listOfLines2))
	{
		return false;
	}
	
	//Check here maybe for direction conservation (Inner product of neighbouring lines should be greater than 0 or even more)


	//At the end, return true in the no-intersections case
	return true;
}


/////////////////////////////////
//Private static intern functions

//Convert List of Positions and Widths into List of Track Segments
std::list<BorderTrackSegment> Track::convertIntoListOfBorderTrackSegments(std::list<CenterWidthTrackSegment> const & listOfPositionsAndWidths)
{
	//Exclude pathological cases
	if (listOfPositionsAndWidths.empty())
	{
		return std::list<BorderTrackSegment>();
	}

	//Setup
	std::list<BorderTrackSegment> listOfTrackSegments;
	std::list<CenterWidthTrackSegment>::const_iterator pairIt = listOfPositionsAndWidths.begin();
	std::list<CenterWidthTrackSegment>::const_iterator nextIt = pairIt;
	++nextIt;

	//Construct TrackSegments
	while (nextIt != listOfPositionsAndWidths.end())
	{
		sf::Vector2f pos1 = pairIt->first;
		sf::Vector2f pos2 = nextIt->first;
		float width = pairIt->second;
		sf::Vector2f relVec = pos2 - pos1;
		sf::Vector2f normalVec = mySFML::Simple::normalize(mySFML::Create::createOrthogonalVector(relVec));
		listOfTrackSegments.push_back(std::make_pair(pos1 + width * normalVec, pos1 - width * normalVec));
		++pairIt;
		++nextIt;
	}

	//Construct last TrackSegment
	sf::Vector2f pos1 = pairIt->first;
	sf::Vector2f pos2 = listOfPositionsAndWidths.front().first;
	float width = pairIt->second;
	sf::Vector2f relVec = pos2 - pos1;
	sf::Vector2f normalVec = mySFML::Simple::normalize(mySFML::Create::createOrthogonalVector(relVec));
	listOfTrackSegments.push_back(std::make_pair(pos1 + width * normalVec, pos1 - width * normalVec));

	//Return listOfTrackSegments
	return listOfTrackSegments;
}


/////////////////////////
//Public static functions

//Construct Circle Track
std::list<CenterWidthTrackSegment> Track::constructCircleTrack(sf::Vector2f const & center, float radius, unsigned int pointCount, float width)
{
	std::list<CenterWidthTrackSegment> list;
	for (unsigned int i = 0; i < pointCount; ++i)
	{
		list.push_back(std::make_pair(radius * mySFML::Create::createNormalVectorWithAngle(static_cast<float>(i) / (pointCount + 1u) * 2.f * myMath::Const::PIf) + center, width));
	}

	return std::move(list);
}