#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include <random>
#include "Utility.h"

class Triangles
{
private:
	std::mt19937_64 rng;
	std::uniform_real_distribution<float> unif;
	sf::VertexArray triangles;
	std::vector<sf::Vector2f> centroids;

	int sizeX;
	int sizeY;

	int currentTriangle;
	int currentCentroid;

	sf::Vector2f randVector2f(int xMax, int yMax);
	float randFloat(float min, float max);
	sf::Vector2f randVector2f0(int xMin, int xMax);
	sf::Vector2f randVector2f(sf::Vector2f xInterval, int variation);
	sf::Color randColor();
	int randInt(int variation);

	void translate(int variation);
	void changeColor();
	void rotate(int angle);
	void scale(int variation);

public:
	Triangles(int x, int y);

	void addTriangle();
	void removeTriangle();

	void mutate();

	int getCount();

	sf::VertexArray& getVertexArray();
};

