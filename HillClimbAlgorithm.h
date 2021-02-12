#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Triangles.h"
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <iomanip>

class HillClimbAlgorithm
{
private:
	int sizeX;
	int sizeY;

	sf::VertexArray currentTriangles;
	Triangles* buffer;

	GLubyte* red;
	GLubyte* green;
	GLubyte* blue;
	GLubyte* mRed;
	GLubyte* mGreen;
	GLubyte* mBlue;

	float prevFitness;
	float currentFitness;
	int mutationLimit;
	int triangleLimit;

	sf::RenderTexture* texture;

	void loadModelBuffers(const sf::Texture& model);
	void loadAlgorithmBuffers(const sf::Texture&);
	void addTriangle();
	std::pair<sf::VertexArray, float> mutateTriangle();
	void mutate();
	void addOrDiscardToCanvas(const std::pair<sf::VertexArray, float>& bestSolution);
	void keepOrDiscardMutation(float& modifiedFitnessBest, sf::VertexArray& modifiedTriangles);
	float calculateFitness() const;

public:
	HillClimbAlgorithm(int limit, int mutationLimit);
	void render(sf::RenderTarget* target) const;
	void update();
	void loadBuffers(const sf::Texture& model);
	bool imageIsDifferentEnough() const;
	bool isComplete() const;
	~HillClimbAlgorithm();
};

