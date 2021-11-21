#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>

int main()
{
    int w = 1200;
    int h = 900;

    int mouseX=0;
    int mouseY=0;
    float mouseSensitivity = 0.5f;

    bool button[6] = { false, false, false, false, false, false };

    sf::Vector3f position = sf::Vector3f(-5.0f, 0.0f, 0.0f);

	bool mouseHidden = true;

    sf::RenderWindow window(sf::VideoMode(w, h), "RayTracing", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    sf::Texture tex;
    tex.create(w, h);
    sf::Sprite spr(tex);

    sf::Shader shader1;
    shader1.loadFromFile("shader1.frag", sf::Shader::Fragment);
    if (!shader1.isAvailable()) {
        std::cout << "The shader is not available\n";
    }

    shader1.setUniform("u_resolution", sf::Vector2f(w, h));

    double time = 0;

    while(window.isOpen()) {
        sf::Event event;
        //exit
        while(window.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				window.close();
            }
            else if (event.type == sf::Event::MouseMoved) {
                if (mouseHidden) {
                    int mx = event.mouseMove.x - w / 2;
                    int my = event.mouseMove.y - h / 2;
                    mouseX += mx;
                    mouseY += my;
                    sf::Mouse::setPosition(sf::Vector2i(w / 2, h / 2), window);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                window.setMouseCursorVisible(mouseHidden);
                mouseHidden=!mouseHidden;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) button[0] = true;
                else if (event.key.code == sf::Keyboard::A) button[1] = true;
                else if (event.key.code == sf::Keyboard::S) button[2] = true;
                else if (event.key.code == sf::Keyboard::D) button[3] = true;
                else if (event.key.code == sf::Keyboard::Space) button[4] = true;
                else if (event.key.code == sf::Keyboard::LShift) button[5] = true;
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::W) button[0] = false;
                else if (event.key.code == sf::Keyboard::A) button[1] = false;
                else if (event.key.code == sf::Keyboard::S) button[2] = false;
                else if (event.key.code == sf::Keyboard::D) button[3] = false;
                else if (event.key.code == sf::Keyboard::Space) button[4] = false;
                else if (event.key.code == sf::Keyboard::LShift) button[5] = false;
            }

            if (mouseHidden) {

                float mx = ((float)mouseX / w - 0.5f) * mouseSensitivity;
                float my = ((float)mouseY / h - 0.5f) * mouseSensitivity;



                //movement::
                sf::Vector3f dir = sf::Vector3f(0.0f, 0.0f, 0.0f);
                sf::Vector3f dirTemp;
                if (button[0]) dir = sf::Vector3f(1.0f, 0.0f, 0.0f);
                else if (button[2]) dir = sf::Vector3f(-1.0f, 0.0f, 0.0f);
                if (button[1]) dir += sf::Vector3f(0.0f, -1.0f, 0.0f);
                else if (button[3]) dir += sf::Vector3f(0.0f, 1.0f, 0.0f);
                dirTemp.z = dir.z * cos(-my) - dir.x * sin(-my);
                dirTemp.x = dir.z * sin(-my) + dir.x * cos(-my);
                dirTemp.y = dir.y;
                dir.x = dirTemp.x * cos(mx) - dirTemp.y * sin(mx);
                dir.y = dirTemp.x * sin(mx) + dirTemp.y * cos(mx);
                dir.z = dirTemp.z;
                position = position + (dir * 0.07f);
                if (button[4]) position.z -= 0.07;
                else if (button[5]) position.z += 0.07;

                shader1.setUniform("u_mouse", sf::Vector2f(mx, my));
                shader1.setUniform("u_pos", position);
                shader1.setUniform("u_time", (float)time);
            }
        }

        window.clear();
        window.draw(spr, &shader1);
        window.display();
        //time += 0.03;
    }

    return 0;
}
