#include "ImageApproximator.h"

ImageApproximator::ImageApproximator(std::string path, int limit, int mutationLimit) : algo(limit, mutationLimit){
    name = createName(path);
    loadModelFromImage(path);
    algo.loadBuffers(model);
    width = model.getSize().x;
    height = model.getSize().y;
    imgNumber = 0;
}

std::string ImageApproximator::createName(std::string path) const {
    std::string name = path;
    for (int i = 0; i < 4; i++) {
        name.pop_back();
    }
    return name;
}

void ImageApproximator::start() {
    window = new sf::RenderWindow(sf::VideoMode(width, height), "Approximator");
    while (window->isOpen()){
        checkEvent();
        update();
        render();  
    }
}

void ImageApproximator::update() {
    algo.update();
    if (algo.imageIsDifferentEnough()) {
        saveToImage();
        imgNumber++;
    }
    if (algo.isComplete()) {
        saveToImage();
        imgNumber++;
        window->close();
    }
}

void ImageApproximator::render() {
    window->clear();
    algo.render(window);
    window->display();
}

void ImageApproximator::checkEvent() const {

    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed) window->close();

    }
}

void ImageApproximator::saveToImage() const {
    sf::Image img;
    img.create(width, height);
    sf::RenderTexture tx;
    tx.create(width, height);

    tx.clear();
    algo.render(&tx);
    tx.display();

    img = tx.getTexture().copyToImage();
    std::string str = name + "_image_" + std::to_string(imgNumber) + ".jpg";
    img.saveToFile(str);
}

void ImageApproximator::loadModelFromImage(std::string path) {
    sf::Image temp;
    temp.loadFromFile(path);
    temp.flipVertically();
    model.loadFromImage(temp);
}

ImageApproximator::~ImageApproximator() {
    delete window;
}
