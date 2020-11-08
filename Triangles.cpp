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

	sf::Vector2f firstC = randVector2f0(sizeX, sizeY);
	sf::Vector2f secondC = randVector2f(sf::Vector2f(firstC), 10);
	sf::Vector2f thirdC = randVector2f(sf::Vector2f(firstC), 10);
	sf::Color color = randColor();

	sf::Vector2f centroid = sf::Vector2f(((firstC.x + secondC.x + thirdC.x) / 3), ((firstC.y + secondC.y + thirdC.y) / 3));
	centroids.push_back(centroid);

	triangles.append(sf::Vertex(firstC, color));
	triangles.append(sf::Vertex(secondC, color));
	triangles.append(sf::Vertex(thirdC, color));

	currentTriangle = 0;
	currentCentroid = 0;
}

void Triangles::addTriangle() {

	currentTriangle += 3;
	currentCentroid += 1;

	sf::Vector2f firstC = randVector2f0(sizeX, sizeY);
	sf::Vector2f secondC = randVector2f(sf::Vector2f(firstC), 15);
	sf::Vector2f thirdC = randVector2f(sf::Vector2f(firstC), 15);
	sf::Color color = randColor();

	sf::Vector2f centroid = sf::Vector2f(((firstC.x + secondC.x + thirdC.x) / 3), ((firstC.y + secondC.y + thirdC.y) / 3));
	centroids.push_back(centroid);

	triangles.append(sf::Vertex(firstC, color));
	triangles.append(sf::Vertex(secondC, color));
	triangles.append(sf::Vertex(thirdC, color));
}

void Triangles::removeTriangle() {

	currentTriangle -= 3;
	centroids.pop_back();
	currentCentroid--;
	triangles.resize(currentTriangle);
}

void Triangles::translate(int variation) {

	for (int i = 0; i < 3; i++) {
		sf::Vector2f trans = randVector2f(-variation, +variation);

		if (triangles[currentTriangle + i].position.x > sizeX) {

			if (trans.x < 0) triangles[currentTriangle + i].position.x += trans.x;
			else continue;

		}
		else if (triangles[currentTriangle + i].position.x < 0) {

			if (trans.x > 0) triangles[currentTriangle + i].position.x += trans.x;
			else continue;
		}
		else triangles[currentTriangle + i].position.x += trans.x;

		if (triangles[currentTriangle + i].position.y > sizeY) {
			if (trans.y < 0) {
				triangles[currentTriangle + i].position.y += trans.y;
			}
			else continue;
		}
		else if (triangles[currentTriangle + i].position.y < 0) {
			if (trans.y > 0) triangles[currentTriangle + i].position.y += trans.y;
			else continue;
		}
		else triangles[currentTriangle + i].position.y += trans.y;
	}
}

void Triangles::changeColor() {

	int rValue = randInt(5);
	int gValue = randInt(5);
	int bValue = randInt(5);

	for (int i = 0; i < 3; i++) {

		triangles[currentTriangle + i].color.r += rValue;
		triangles[currentTriangle + i].color.g += gValue;
		triangles[currentTriangle + i].color.b += bValue;
	}
}

void Triangles::rotate(int angleDeg) {

	float angle = Utility::rad(angleDeg);

	for (int i = 0; i < 3; i++) {

		float currentTriangleX = triangles[currentTriangle + i].position.x;
		float currentTriangleY = triangles[currentTriangle + i].position.y;

		float currentCentroidX = centroids[currentCentroid].x;
		float currentCentroidY = centroids[currentCentroid].y;

		float xOut = ((cos(angle) * /*triangles[currentTriangle + i].position.x*/ currentTriangleX) - (sin(angle) * /*triangles[currentTriangle + i].position.y*/ currentTriangleY) + (/*centroids[currentCentroid].x*/ currentCentroidX) - (cos(angle) * (/*centroids[currentCentroid].x*/ currentCentroidX)) + (sin(angle) * (/*centroids[currentCentroid].y*/ currentCentroidY)));
		float yOut = ((sin(angle) * /*triangles[currentTriangle + i].position.x*/ currentTriangleX) + (cos(angle) * /*triangles[currentTriangle + i].position.y*/ currentTriangleY) + (/*centroids[currentCentroid].y*/ currentCentroidY) - (sin(angle) * (/*centroids[currentCentroid].x*/ currentCentroidX)) - (cos(angle) * (/*centroids[currentCentroid].y*/ currentCentroidY)));

		triangles[currentTriangle + i].position.x = xOut;
		triangles[currentTriangle + i].position.y = yOut;

	}
}

void Triangles::mutate() {

	translate(8);
	changeColor();
	rotate(randInt(7));
	scale(4);
}

void Triangles::scale(int variation) {

	sf::Vector2f scale = randVector2f(-variation, +variation);

	for (int i = 0; i < 3; i++) {
		
		if (triangles[currentTriangle + i].position.x * scale.x > sizeX || triangles[currentTriangle + i].position.x * scale.x < 0) {}
		else triangles[currentTriangle + i].position.x *= scale.x;

		if (triangles[currentTriangle + i].position.y * scale.y > sizeY || triangles[currentTriangle + i].position.y * scale.y < 0) {}
		else triangles[currentTriangle + i].position.y *= scale.y;
	}
}

sf::Vector2f Triangles::randVector2f(int min, int max) {

	unif = std::uniform_real_distribution<float>(min, max);

	return sf::Vector2f(unif(rng), unif(rng));
}

sf::Vector2f Triangles::randVector2f(sf::Vector2f interval, int variation) {
	sf::Vector2f coords;

	unif = std::uniform_real_distribution<float>(interval.x - variation, interval.x + variation);
	coords.x = unif(rng);

	unif = std::uniform_real_distribution<float>(interval.y - variation, interval.y + variation);
	coords.y = unif(rng);

	return coords;
}

sf::Vector2f Triangles::randVector2f0(int xMax, int yMax) {

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

/*Triangles& Triangles::operator =(const Triangles& other) {

	if (this != &other) {
		this->rng = other.rng;
		this->unif = other.unif;
		this->triangles = other.triangles;
		uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
		rng.seed(ss);
	}

	return *this;
}

Triangles::Triangles(const Triangles& other) {

	*this = other;
}
*/

int Triangles::getCount() {

	return centroids.size();
}

sf::VertexArray& Triangles::getVertexArray() {

	return triangles;
}