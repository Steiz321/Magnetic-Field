#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <math.h>
#define MYU 4*PI*pow(10,-7)
#define PI 3.141592653589793238462643

using namespace std;

double calcHx(double a, double x, double z, double sigma) {
    return (sigma / (4 * PI * MYU)) * log((pow(x + a, 2) + pow(z, 2)) / (pow(x - a, 2) + pow(z, 2)));
}

double calcHz(double a, double x, double z, double sigma) {
    return (sigma / (2 * PI * MYU)) * (atan((a - x) / z) + atan((a + x) / z));
}

struct Coords {
    int x;
    int z;
    int hx;
    int hz;
};

struct Magnet {
    double xMid;
    double zUp;
    double zDown;
};


int main() {
    system("color F");

    cout << "----------------------------------------------------------------------------------------" << endl;
    cout << "\t\t\tFAQ\t\t" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;
    cout << " That application can simulate magnetic field due to input parameters.\n\n After run an application you'll be able to choose screen resolution,\n you can select it by entering numbers 1-3(respectively) and pressing\n Enter.\n\n Next, you need to enter the size of the magnets(in pixels).\n This is done by entering any numbers greater than zero,\n according to the instructions in the console.\n\n The next step is to select or reverse (change poles) the second magnet relative to\n the first or not.\n\n You will also need to enter a density for each of the magnets (for easy viewing of the model\n it is recommended to use the range 0.0001).\n\n After entering the data you will simulate the magnetic field relative to the parameters you have selected.\n\n You can move the magnets with the keys on the keypad.\n\nFor the first magnet it's the keys:\n W - up, S - down, A - left, D - right \n For another magnite :  \n Arrows up, down, left, right." << endl;
    cout << "----------------------------------------------------------------------------------------" << endl << endl;
    system("pause");
    cout << endl;
    
    int consoleWidth;
    int consoleHeight;
    int number;
    cout << "---- Choose window resolution: ----" << endl;
    cout << " 1. 1300 x 1100 (Recommended size of magnets in the range 130 x 290)" << endl << " 2. 1000 x 900 (Recommended size of magnets in the range 90 x 250)" << endl << " 3. 700 x 600 (Recommended size of magnets in the range 60 x 160)" << endl;
    cout << " > ";
    cin >> number;

    int matSize;
    int koef;
    bool flag;

    switch (number) {
    case 1: {
        consoleWidth = 1300;
        consoleHeight = 1100;
        matSize = 70;
        koef = 2;
    }
        break;
    case 2: {
        consoleWidth = 1000;
        consoleHeight = 900;
        matSize = 50;
        koef = 1;
    }
        break;
    case 3: {
        consoleWidth = 700;
        consoleHeight = 600;
        matSize = 40;
        koef = 1;
    }
        break;
    default:
        cout << endl << "Invalid value entered!";
        return 0;
    }

    cout << endl;

    int number2;
    cout << "---- Turn the second magnet relative to the first? ----" << endl;
    cout << "1. Do not Reverse" << endl;
    cout << "2. Invert" << endl;
    cout << " > ";
    cin >> number2;
    switch (number2) {
        case 1: 
            flag = false;
            break;
        
        case 2: 
            flag = true;
            break;
        
    }

    cout << endl;

    int magnetWidth1;
    int magnetWidth2;
    int magnetHeight1;
    int magnetHeight2;
    double sigma1;
    double sigma2;
    cout << "---- Enter the size of the magnets ----" << endl << endl;
    cout << " Width of the first magnet:" << endl << " > ";
    cin >> magnetWidth1;
    cout << " Height of the first magnet:" << endl << " > ";
    cin >> magnetHeight1;
    cout << " Width of the second magnet:" << endl << " > ";
    cin >> magnetWidth2;
    cout << " Height of the second magnet:" << endl << " > ";
    cin >> magnetHeight2;
    cout << " Density for the first magnet:" << endl << " > ";
    cin >> sigma1;
    cout << " Density for the second magnet:" << endl << " > ";
    cin >> sigma2;
    
    Coords **cords = new Coords*[matSize];
    for (int i = 0; i < matSize; i++) {
        cords[i] = new Coords[matSize];
    }
    
    sf::RenderWindow window(sf::VideoMode(consoleWidth, consoleHeight), "Magnetic Field", sf::Style::Close | sf::Style::Titlebar);
    sf::RectangleShape firstMagnet(sf::Vector2f(magnetWidth1, magnetHeight1));
    sf::RectangleShape secondMagnet(sf::Vector2f(magnetWidth2, magnetHeight2));
    firstMagnet.setOrigin(magnetWidth1 / 2, magnetHeight1 / 2);
    firstMagnet.setPosition(window.getSize().x / 2 - magnetWidth1 * 1.3, window.getSize().y / 2);
    secondMagnet.setOrigin(magnetWidth2 / 2, magnetHeight2 / 2);
    secondMagnet.setPosition(window.getSize().x / 2 + magnetWidth2 * 1.3, window.getSize().y / 2);
    sf::Texture magnetTexture1;
    sf::Texture magnetTexture2;

    magnetTexture1.loadFromFile("./images/MagnetTexture.png");
    if (!flag) {
        magnetTexture2.loadFromFile("./images/MagnetTexture.png");
    }
    else {
        magnetTexture2.loadFromFile("./images/MagnetTextureReverse.png");
    }

    firstMagnet.setTexture(&magnetTexture1);
    secondMagnet.setTexture(&magnetTexture2);

    for (int i = 0; i < matSize; i++) {
        for (int j = 0; j < matSize; j++) {
            cords[i][j].x = i * 20;
            cords[i][j].z = j * 20;
        }
    }
    sf::VertexArray line(sf::Lines, 2);

    int deltaX, deltaZDown, deltaZUp, deltaX2, deltaZDown2, deltaZUp2;

    Magnet magnet1, magnet2;
    
    while (window.isOpen())
    {
        sf::Event evnt;
        while (window.pollEvent(evnt)) {
            if (evnt.type == evnt.Closed)
                window.close();
        }
        magnet1.xMid = firstMagnet.getPosition().x;
        magnet1.zUp = firstMagnet.getPosition().y - magnetHeight1/2;
        magnet1.zDown = firstMagnet.getPosition().y + magnetHeight1/2;

        magnet2.xMid = secondMagnet.getPosition().x;
        magnet2.zUp = secondMagnet.getPosition().y - magnetHeight2/2;
        magnet2.zDown = secondMagnet.getPosition().y + magnetHeight2/2;

        for (int i = 0; i < matSize; i++) {
            for (int j = 0; j < matSize; j++) {
                deltaX = cords[i][j].x - magnet1.xMid;
                deltaZUp = magnet1.zUp - cords[i][j].z;
                deltaZDown = magnet1.zDown - cords[i][j].z;

                deltaX2 = cords[i][j].x - magnet2.xMid;
                deltaZUp2 = magnet2.zUp - cords[i][j].z;
                deltaZDown2 = magnet2.zDown - cords[i][j].z;

                if (!flag) {
                    cords[i][j].hx = cords[i][j].x + calcHx(magnetWidth1 / 2, deltaX, deltaZUp, sigma1) - calcHx(magnetWidth1 / 2, deltaX, deltaZDown, sigma1) + calcHx(magnetWidth2 / 2, deltaX2, deltaZUp2, sigma2) - calcHx(magnetWidth2 / 2, deltaX2, deltaZDown2, sigma2);
                    cords[i][j].hz = cords[i][j].z - calcHz(magnetWidth1 / 2, deltaX, deltaZUp, sigma1) + calcHz(magnetWidth1 / 2, deltaX, deltaZDown, sigma1) - calcHz(magnetWidth2 / 2, deltaX2, deltaZUp2, sigma2) + calcHz(magnetWidth2 / 2, deltaX2, deltaZDown2, sigma2);
                }
                else {
                    cords[i][j].hx = cords[i][j].x + calcHx(magnetWidth1 / 2, deltaX, deltaZUp, sigma1) - calcHx(magnetWidth1 / 2, deltaX, deltaZDown, sigma1) - calcHx(magnetWidth2 / 2, deltaX2, deltaZUp2, sigma2) + calcHx(magnetWidth2 / 2, deltaX2, deltaZDown2, sigma2);
                    cords[i][j].hz = cords[i][j].z - calcHz(magnetWidth1 / 2, deltaX, deltaZUp, sigma1) + calcHz(magnetWidth1 / 2, deltaX, deltaZDown, sigma1) + calcHz(magnetWidth2 / 2, deltaX2, deltaZUp2, sigma2) - calcHz(magnetWidth2 / 2, deltaX2, deltaZDown2, sigma2);
                }
                
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            firstMagnet.setPosition(firstMagnet.getPosition().x - 2 * koef, firstMagnet.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            firstMagnet.setPosition(firstMagnet.getPosition().x + 2 * koef, firstMagnet.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            firstMagnet.setPosition(firstMagnet.getPosition().x, firstMagnet.getPosition().y - 2 * koef);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            firstMagnet.setPosition(firstMagnet.getPosition().x, firstMagnet.getPosition().y + 2 * koef);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            secondMagnet.setPosition(secondMagnet.getPosition().x - 2 * koef, secondMagnet.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            secondMagnet.setPosition(secondMagnet.getPosition().x + 2 * koef, secondMagnet.getPosition().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            secondMagnet.setPosition(secondMagnet.getPosition().x, secondMagnet.getPosition().y - 2 * koef);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            secondMagnet.setPosition(secondMagnet.getPosition().x, secondMagnet.getPosition().y + 2 * koef);
        }

        window.clear();
        for (int i = 0; i < matSize; i++) {
            for (int j = 0; j < matSize; j++) {
                line[0].position = sf::Vector2f(cords[i][j].x, cords[i][j].z);
                line[0].color = sf::Color(sf::Color::Blue);
                line[1].position = sf::Vector2f(cords[i][j].hx, cords[i][j].hz);
                line[1].color = sf::Color(sf::Color::Red);
                window.draw(line);
            }
        }
        window.draw(firstMagnet);
        window.draw(secondMagnet);
        window.display();
    }
        
    for (int i = 0; i < matSize; i++) {
        delete[]cords[i];
    }

    return 0;
}