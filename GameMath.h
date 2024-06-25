#pragma once
#include <SFML/Graphics.hpp>


namespace GameMath
{
	struct InterpolateResult {
		sf::Vector2f position;
		float leftOverStepSize;
	};

	double distance(const sf::Vector2f &from, const sf::Vector2f &to);

	sf::Vector2f normalize(const sf::Vector2f& source);

	InterpolateResult interpolate(const sf::Vector2f& position, const sf::Vector2f& destination, float stepsize);
};
