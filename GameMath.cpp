#include "GameMath.h"
#include <iostream>




double GameMath::distance(const sf::Vector2f &from,const sf::Vector2f &to)
{
	return sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
}

sf::Vector2f GameMath::normalize(const sf::Vector2f& source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return sf::Vector2f(source.x / length, source.y / length);
    else
        return source;
}

GameMath::InterpolateResult GameMath::interpolate(const sf::Vector2f& position, const sf::Vector2f& destination, float stepsize) {
    sf::Vector2f distance = destination - position;
    sf::Vector2f road = normalize(distance);
    road *= stepsize;

    // if one
    if(abs(distance.x) > abs(road.x) || abs(distance.y) > abs(road.y))
        return { road + position,0.f };

    return { road + position,2.f };
}
