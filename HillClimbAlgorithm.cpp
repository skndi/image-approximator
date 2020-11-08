#include "HillClimbAlgorithm.h"


HillClimbAlgorithm::Climber(std::string path, int limit, int mutationLimit) {

    name = path;
    for (int i = 0; i < 4; i++) {
        name.pop_back();
    }

    currentFitness = 0;
    modifiedFitness = 0;
    prevFitness = 0;
    modifiedFitnessBest = 0;
    mCount = 0;
    imgNumber = 0;

    this->limit = limit;
    this->mutationLimit = mutationLimit;

    sf::Image temp;
    temp.loadFromFile(path);
    temp.flipVertically();
    model.loadFromImage(temp);

    sizeX = model.getSize().x;
    sizeY = model.getSize().y;
    //model.flipVertically();

    sf::Context context;
    sf::Texture::bind(&model);
    mRed = new GLubyte[sizeX * sizeY]();
    mGreen = new GLubyte[sizeX * sizeY]();
    mBlue = new GLubyte[sizeX * sizeY]();

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, mRed);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_GREEN, GL_UNSIGNED_BYTE, mGreen);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BLUE, GL_UNSIGNED_BYTE, mBlue);

    buffer = new Triangles(model.getSize().x, model.getSize().y);

    texture = new sf::RenderTexture();
    sf::RectangleShape r(sf::Vector2f(768, 432));
    r.setFillColor(sf::Color(sf::Color::White));
    texture->create(sizeX, sizeY);

    texture->clear();
    texture->draw(r);
    texture->display();

    red = new GLubyte[sizeX * sizeY]();
    green = new GLubyte[sizeX * sizeY]();
    blue = new GLubyte[sizeX * sizeY]();
}

void HillClimbAlgorithm::start() {
    window = new sf::RenderWindow(sf::VideoMode(sizeX, sizeY), "Climb!");

    
    while (window->isOpen())
    {
        checkEvent();
        
        render();

        update();

        if (abs(currentFitness - prevFitness) > 0.1) {

            prevFitness = currentFitness;
            saveToImage();
            imgNumber++;
        }
        if ((finalTriangles.getVertexCount() / 3) >= limit) {
            saveToImage();
            window->close();
        }
    }
}

void HillClimbAlgorithm::mutate() {
    
    buffer->mutate();
    mCount++;
}

void HillClimbAlgorithm::checkEvent() const {

    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed) window->close();

    }
}

float HillClimbAlgorithm::calculateFitness(){

    sf::Texture::bind(&(texture->getTexture()));

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, red);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_GREEN, GL_UNSIGNED_BYTE, green);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BLUE, GL_UNSIGNED_BYTE, blue);

    float calculatedFitness = 0;

    int index = 0;

    for (int i = 0; i < sizeY; i++) {

        for (int j = 0; j < sizeX; j++) {

            calculatedFitness += abs(*(mRed + index) - *(red + index));
            calculatedFitness += abs(*(mGreen + index) - *(green + index));
            calculatedFitness += abs(*(mBlue + index) - *(blue + index));

            index++;
        }

    }

    calculatedFitness /= sizeX * sizeY * 3 * (float)2.55;
    return 100 - calculatedFitness;
}

void HillClimbAlgorithm::chooseMutation() {

    if (modifiedFitnessBest > currentFitness) {

        finalTriangles = modifiedTrianglesBest;
        currentFitness = modifiedFitnessBest;
        std::cout << currentFitness << '\n';
    }

    else {
        buffer->removeTriangle();
        buffer->getVertexArray() = finalTriangles;
        modifiedTrianglesBest = finalTriangles;
    }
}

void HillClimbAlgorithm::update() {
    while (mCount < mutationLimit) {
        
        mutate();

        texture->clear();
        texture->draw(buffer->getVertexArray());
        texture->display();

        modifiedFitness = calculateFitness();

        if (modifiedFitness > modifiedFitnessBest) {

            modifiedTrianglesBest = buffer->getVertexArray();
            modifiedFitnessBest = modifiedFitness;

        }
        else buffer->getVertexArray() = modifiedTrianglesBest;
        modifiedFitness = currentFitness;
    }

    chooseMutation();

    modifiedFitness = 0;
    modifiedFitnessBest = 0;

    buffer->addTriangle();
    mCount = 0;
}

void HillClimbAlgorithm::render() {

    window->clear();
    window->draw(finalTriangles);
    window->display();
}

void HillClimbAlgorithm::saveToImage() {

    sf::Image img;

    img.create(sizeX, sizeY);
    sf::RenderTexture tx;
    tx.create(sizeX, sizeY);

    tx.clear();
    tx.draw(finalTriangles);
    tx.display();

    
    img = tx.getTexture().copyToImage();

    std::string str = name + "_image_" + std::to_string(imgNumber) + ".jpg";

    img.saveToFile(str);
}

HillClimbAlgorithm::~HillClimbAlgorithm() {

    delete window;
    delete texture;
    delete[] blue;
    delete[] red;
    delete[] green;
    delete[] mRed;
    delete[] mGreen;
    delete[] mBlue;
}