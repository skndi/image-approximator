#pragma once
#include <string>
#include "HillClimbAlgorithm.h"

class ImageApproximator {
private:
	std::string name;
	HillClimbAlgorithm algo;
	sf::RenderWindow* window;
	int width;
	int height;
	int imgNumber;
	sf::Texture model;
	std::string createName(std::string path) const;
	void checkEvent() const;
	void saveToImage() const;
	void loadModelFromImage(std::string path);
	void update();
	void render();

public:
	ImageApproximator(std::string path, int limit, int mutationLimit);
	void start();
	~ImageApproximator();
};
