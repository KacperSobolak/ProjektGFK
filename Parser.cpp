#include "Parser.h"

void Parser::FileParser(const std::string& filename) {
    std::ifstream file(filename);
    int width, height, FrameNmbr, FrameTime;
    std::string command;
    char comma;
    file >> width >> comma >> height;
    std::cout << "Frame size: " << width << " x " << height << std::endl;
    file >> std::ws;
    while (file >> FrameNmbr >> comma >> FrameTime) {
        std::cout << "\nNext Frame: " << FrameNmbr << " with time: " << FrameTime << " ms" << std::endl;
        file >> std::ws;
        float a, b, c, d, thickness;
        bool flag = false;
        //sf::Color FillingColor = sf::Color(0, 0, 0), PenColor = sf::Color(0, 0, 0);
        while (file >> command) {
            if (command == "stop" || command == "ST")
                break;
            else if (command == "punkt" || command == "PT") {
                file >> a >> comma >> b;
                std::cout << "Drawing point: " << a << " " << b << std::endl;
            }
            else if (command == "linia" || command == "LN") {
                file >> a >> comma >> b >> comma >> c >> comma >> d;
                std::cout << "Drawing line: " << "(" << a << ", " << b << ") " << "(" << c << " " << d << ")" << std::endl;
            }
            else if (command == "prostokat" || command == "PR") {
                file >> a >> comma >> b >> comma >> c >> comma >> d >> flag;
                std::cout << "Drawing rectangle: " << "(" << a << ", " << b << ") " << "(" << c << " " << d << ")"
                    " Wypelnienie: " << flag << std::endl;
            }
            else if (command == "elipsa" || command == "EL") {
                file >> a >> comma >> b >> comma >> c >> comma >> d >> flag;
                std::cout << "Drawing elipse: " << "(" << a << ", " << b << ") " << "(" << c << " " << d << ")"
                    " Wypelnienie: " << flag << std::endl;
            }
            else if (command == "rozmiar_piora" || command == "RP") {
                file >> thickness;
                std::cout << "Changing Pen size to: " << thickness << std::endl;
            }
            else if (command == "kolor_piora" || command == "KP") {
                file >> a >> b >> c;
                std::cout << "Changing Pen colour to (" << a << ", " << b << ", " << c << ") " << std::endl;
            }
            else if (command == "kolor_wypelnienia" || command == "KW") {
                file >> a >> b >> c;
                std::cout << "Changing Fill colour to (" << a << ", " << b << ", " << c << ")" << std::endl;
            }

        }
    }
}