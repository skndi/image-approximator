#include "HillClimbAlgorithm.h"


HillClimbAlgorithm::HillClimbAlgorithm(int limit, int mutationLimit) {
    currentFitness = 0;
    prevFitness = 0;
    this->triangleLimit = limit;
    this->mutationLimit = mutationLimit;
}

void HillClimbAlgorithm::loadBuffers(const sf::Texture& model) {
    loadModelBuffers(model);
    loadAlgorithmBuffers(model);
}

void HillClimbAlgorithm::loadModelBuffers(const sf::Texture& model) {
    sizeX = model.getSize().x;
    sizeY = model.getSize().y;

    sf::Context context;
    sf::Texture::bind(&model);
    mRed = new GLubyte[static_cast<long long>(sizeX) * sizeY]();
    mGreen = new GLubyte[static_cast<long long>(sizeX) * sizeY]();
    mBlue = new GLubyte[static_cast<long long>(sizeX) * sizeY]();

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, mRed);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_GREEN, GL_UNSIGNED_BYTE, mGreen);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BLUE, GL_UNSIGNED_BYTE, mBlue);
}

void HillClimbAlgorithm::loadAlgorithmBuffers(const sf::Texture& model) {
    buffer = new Triangles(sizeX, sizeY);
    texture = new sf::RenderTexture();
    texture->create(sizeX, sizeY);
    red = new GLubyte[sizeX * sizeY]();
    green = new GLubyte[sizeX * sizeY]();
    blue = new GLubyte[sizeX * sizeY]();
}

void HillClimbAlgorithm::update() {
    addTriangle();
    std::pair<sf::VertexArray, float> bestSolution = mutateTriangle();
    addOrDiscardToCanvas(bestSolution);
}

void HillClimbAlgorithm::render(sf::RenderTarget* target) const {
    target->draw(currentTriangles);
}

void HillClimbAlgorithm::addTriangle() {
    buffer->addTriangle();
}

std::pair<sf::VertexArray, float> HillClimbAlgorithm::mutateTriangle() {
    float modifiedFitnessBest = 0;
    int mutationCount = 0;
    sf::VertexArray bestTriangles = buffer->getVertexArray();

    while (mutationCount < mutationLimit) {
        mutate();
        keepOrDiscardMutation(modifiedFitnessBest, bestTriangles);
        mutationCount++;
    }
    return std::make_pair(bestTriangles, modifiedFitnessBest);
}

void HillClimbAlgorithm::addOrDiscardToCanvas(const std::pair<sf::VertexArray, float>& bestSolution) {

    if (bestSolution.second > currentFitness) {
        currentTriangles = bestSolution.first;
        currentFitness = bestSolution.second;
        std::cout << currentFitness << '\n';
    }
    else {
        buffer->removeTriangle();
    }
    buffer->getVertexArray() = currentTriangles;
}

void HillClimbAlgorithm::mutate() {
    buffer->mutate();
    texture->clear();
    buffer->render(texture);
    texture->display();
}

void HillClimbAlgorithm::keepOrDiscardMutation(float& modifiedFitnessBest, sf::VertexArray& bestTriangles) {
    float modifiedFitness = calculateFitness();

    if (modifiedFitness > modifiedFitnessBest) {
        bestTriangles = buffer->getVertexArray();
        modifiedFitnessBest = modifiedFitness;
    }
    else {
        buffer->updateTriangles(bestTriangles);
    }
}

float HillClimbAlgorithm::calculateFitness() const {

    sf::Texture::bind(&(texture->getTexture()));

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, red);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_GREEN, GL_UNSIGNED_BYTE, green);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BLUE, GL_UNSIGNED_BYTE, blue);

    float calculatedFitness = 0;
    int index = 0;

    for (int i = 0; i < sizeY; i++) {
        for (int j = 0; j < sizeX; j++) {
            calculatedFitness += abs(*(mRed + index) - *(red + index)) +
            abs(*(mGreen + index) - *(green + index)) + abs(*(mBlue + index) - *(blue + index));
            index++;
        }
    }

    calculatedFitness /= sizeX * sizeY * 3 * (float)2.55;
    return 100 - calculatedFitness;
}

bool HillClimbAlgorithm::imageIsDifferentEnough() const {
    bool diff = false;
    if (abs(currentFitness - prevFitness) > 0.1) {
        diff = true;
    }
    return diff;
}

bool HillClimbAlgorithm::isComplete() const{
    bool isReady = false;
    if ((currentTriangles.getVertexCount() / 3) >= triangleLimit) {
        isReady = true;
    }
    return isReady;
}

HillClimbAlgorithm::~HillClimbAlgorithm() {

    delete texture;
    delete[] blue;
    delete[] red;
    delete[] green;
    delete[] mRed;
    delete[] mGreen;
    delete[] mBlue;
}