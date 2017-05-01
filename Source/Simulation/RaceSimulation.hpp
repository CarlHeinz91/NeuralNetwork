#ifndef RACESIMULATION_HPP
#define RACESIMULATION_HPP

#include "SFML\Graphics.hpp"

#include "Source\Track\Track.hpp"
#include "Source\Car\Car.hpp"

#include <list>


class RaceSimulation
{
private:
	Track mTrack;

	std::list<Car> mListOfCars;

public:
	RaceSimulation();
	~RaceSimulation();

	void render(sf::RenderWindow * renderWindow);
	void update(sf::Time const & time, sf::RenderWindow const * renderWindow);

public:
	Track const & getTrackReference() const;


private:
	static std::list<std::pair<sf::Vector2f, float>> constructCircleTrack();



};





#endif //RACESIMULATION_HPP

