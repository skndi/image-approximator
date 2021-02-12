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

	sf::Vector2f randVector2f(float xMax, float yMax);
	float randFloat(float min, float max);
	sf::Vector2f randVector2fScaled(int xMin, int xMax);
	sf::Vector2f randVector2f(sf::Vector2f xInterval, float variation);
	sf::Color randColor();
	int randInt(int variation);

	void translate(float variation);
	void changeColor(int variation);
	void rotate(float angle);
	void scale(float variation);

public:
	Triangles(int x, int y);

	void render(sf::RenderTarget* target);
	void addTriangle();
	void removeTriangle();
	void updateTriangles(const sf::VertexArray& triangles);
	void mutate();
	int getCount();
	sf::VertexArray getVertexArray();
};

