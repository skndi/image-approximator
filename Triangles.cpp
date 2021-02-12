#include "Triangles.h"
#include <iostream>

Triangles::Triangles(int x, int y) {

	triangles = sf::VertexArray(sf::Triangles);
	rng = std::mt19937_64();

	sizeX = x;
	sizeY = y;

	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);

	currentTriangle = 0;
	currentCentroid = -1;
}

void Triangles::render(sf::RenderTarget* target) {
	target->draw(triangles);
}

void Triangles::addTriangle() {
	sf::Vector2f firstC = randVector2fScaled(sizeX, sizeY);
	sf::Vector2f secondC = randVector2f(sf::Vector2f(firstC), 15);
	sf::Vector2f thirdC = randVector2f(sf::Vector2f(firstC), 15);
	sf::Color color = randColor();
	sf::Vector2f centroid = sf::Vector2f(((firstC.x + secondC.x + thirdC.x) / 3), ((firstC.y + secondC.y + thirdC.y) / 3));
	centroids.push_back(centroid);

	triangles.append(sf::Vertex(firstC, color));
	triangles.append(sf::Vertex(secondC, color));
	triangles.append(sf::Vertex(thirdC, color));

	currentTriangle += 3;
	currentCentroid += 1;
}

void Triangles::removeTriangle() {

	currentTriangle -= 3;
	centroids.pop_back();
	currentCentroid--;
	triangles.resize(currentTriangle);
}

void Triangles::mutate() {
	translate(5);
	changeColor(5);
	rotate(randInt(5));
}

void Triangles::translate(float variation) {
	int currentTriangleVertex = currentTriangle - 3;
	for (int i = 0; i < 3; i++) {
		sf::Vector2f trans = randVector2f(-variation, +variation);
		triangles[currentTriangleVertex + i].position.x += trans.x;
		triangles[currentTriangleVertex + i].position.y += trans.y;
	}
}

void Triangles::changeColor(int variation) {
	int currentTriangleVertex = currentTriangle - 3;
	int rValue = randInt(variation);
	int gValue = randInt(variation);
	int bValue = randInt(variation);

	for (int i = 0; i < 3; i++) {

		triangles[currentTriangleVertex + i].color.r += rValue;
		triangles[currentTriangleVertex + i].color.g += gValue;
		triangles[currentTriangleVertex + i].color.b += bValue;
	}
}

void Triangles::rotate(float angleDeg) {
	int currentTriangleVertex = currentTriangle - 3;
	float angle = Utility::rad(angleDeg);

	for (int i = 0; i < 3; i++) {

		float currentTriangleX = triangles[currentTriangleVertex + i].position.x;
		float currentTriangleY = triangles[currentTriangleVertex + i].position.y;

		float currentCentroidX = centroids[currentCentroid].x;
		float currentCentroidY = centroids[currentCentroid].y;

		float xOut = ((cos(angle) * currentTriangleX) - (sin(angle) * currentTriangleY) + (currentCentroidX) - (cos(angle) * (currentCentroidX)) + (sin(angle) * (currentCentroidY)));
		float yOut = ((sin(angle) * currentTriangleX) + (cos(angle) * currentTriangleY) + (currentCentroidY) - (sin(angle) * (currentCentroidX)) - (cos(angle) * (currentCentroidY)));

		triangles[currentTriangleVertex + i].position.x = xOut;
		triangles[currentTriangleVertex + i].position.y = yOut;

	}
}

void Triangles::scale(float variation) {
	int currentTriangleVertex = currentTriangle - 3;
	sf::Vector2f scale = randVector2f(0.5, variation);
	
	for (int i = 0; i < 3; i++) {
			triangles[currentTriangleVertex + i].position.x *= scale.x;
			triangles[currentTriangleVertex + i].position.y *= scale.y;
	}
}

sf::Vector2f Triangles::randVector2f(float min, float max) {

	unif = std::uniform_real_distribution<float>(min, max);

	return sf::Vector2f(unif(rng), unif(rng));
}

sf::Vector2f Triangles::randVector2f(sf::Vector2f interval, float variation) {
	sf::Vector2f coords;

	unif = std::uniform_real_distribution<float>(interval.x - variation, interval.x + variation);
	coords.x = unif(rng);

	unif = std::uniform_real_distribution<float>(interval.y - variation, interval.y + variation);
	coords.y = unif(rng);

	return coords;
}

sf::Vector2f Triangles::randVector2fScaled(int xMax, int yMax) {

	unif = std::uniform_real_distribution<float>(0, 1);

	return sf::Vector2f(unif(rng) * xMax, unif(rng) * yMax);
}

sf::Color Triangles::randColor() {

	unif = std::uniform_real_distribution<float>(0, 1);

	return sf::Color(unif(rng) * 255, unif(rng) * 255, unif(rng) * 255);
}

float Triangles::randFloat(float min, float max) {

	unif = std::uniform_real_distribution<float>(min, max);
	return unif(rng) * max;
}

int Triangles::randInt(int variation) {

	unif = std::uniform_real_distribution<float>(-variation, variation);
	return unif(rng);
}

int Triangles::getCount() {

	return centroids.size();
}

void Triangles::updateTriangles(const sf::VertexArray& triangles) {
	this->triangles = triangles;
}

sf::VertexArray Triangles::getVertexArray() {
	return triangles;
}