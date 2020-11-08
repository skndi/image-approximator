#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include "Triangles.h"
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <iomanip>

class HillClimbAlgorithm
{
private:
	std::string name;

	sf::VertexArray finalTriangles;
	sf::VertexArray modifiedTrianglesBest;
	Triangles* buffer;
	
	int sizeX;
	int sizeY;

	GLubyte* red;
	GLubyte* green;
	GLubyte* blue;
	
	GLubyte* mRed;
	GLubyte* mGreen;
	GLubyte* mBlue;

	sf::Texture model;

	float prevFitness;
	float currentFitness;
	float modifiedFitness;
	float modifiedFitnessBest;
	int mCount;
	int mutationLimit;
	int limit;
	int imgNumber;

	sf::RenderTexture* texture;
	sf::RenderWindow* window;

	void mutate();

	void render();

	void update();

	void checkEvent() const;

	float calculateFitness();

	void chooseMutation();

	void saveToImage();

public:
	HillClimbAlgorithm(std::string path, int limit, int mutationLimit);
	void start();
	~HillClimbAlgorithm();
	
};

