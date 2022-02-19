 /*
 * Author: Dennis Lövkvist
 * Version: 1.0 (2022-02-19)
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <random>
#include <string>

int main()
{
   sf::RenderWindow window(sf::VideoMode(1920,720), "SFML works!", sf::Style::Close);

	sf::View view = window.getDefaultView();
	view.move(-1280/2,-720/2);
	sf::Clock clock;
	float dt = 1.f / 60.f;
	float accumulator = 0.f;
	float t = 0;


    sf::Font font;
    if (!font.loadFromFile("content/arial.ttf"))
    {
        window.close();
        std::cout << "Cannot find font at " << ("content/matrix.ttf") << std::endl << "Press [ENTER] to exit." << std::endl;
        std::cin.ignore(); 
        return 0;
    }
    int symbol_width = 12;
    sf::RenderTexture glyph_map;
    glyph_map.create(1332, 18);

    sf::Text characters;
    characters.setLetterSpacing(10);
    characters.setFont(font);
    char c[63] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    

	glyph_map.clear(sf::Color(255,255,255,0));
    for (size_t i = 0; i < 62; i++)
    {
        characters.setString(c[i]);
        characters.setCharacterSize(16); 
        characters.setFillColor(sf::Color::Green);
        int width = characters.getLocalBounds().width;


        int x = (symbol_width/2)-(width/2);

        characters.setPosition((symbol_width*i+x),0);
        

        glyph_map.draw(characters);
    }
    


    

    sf::Sprite glyph_map_sprite;
    glyph_map_sprite.setTexture(glyph_map.getTexture());
    
    window.draw(glyph_map_sprite);


    int symbol_count = 1000;
    int *speed = new int[symbol_count];

    sf::VertexArray* va = new sf::VertexArray(sf::PrimitiveType::Quads,symbol_count*4);


    for (size_t i = 0; i < symbol_count; i++)
    {
        int index = i * 4;
        int x = 1 + rand() % (( 1920 + 1 ) - 1);
        int y = 1 + rand() % (( 720 + 1 ) - 1);
        (*va)[index].position = sf::Vector2f(x,y);    
        (*va)[index+1].position = sf::Vector2f(x+symbol_width,y);
        (*va)[index+2].position = sf::Vector2f(x+symbol_width,y+16);
        (*va)[index+3].position = sf::Vector2f(x,y+16);


        int offset_x = 1 + rand() % (( 63 + 1 ) - 1)*symbol_width;
        (*va)[index].texCoords = sf::Vector2f(offset_x,0);
        (*va)[index+1].texCoords = sf::Vector2f(offset_x+symbol_width,0);
        (*va)[index+2].texCoords = sf::Vector2f(offset_x+symbol_width,16);
        (*va)[index+3].texCoords = sf::Vector2f(offset_x,16);

        speed[i] = (rand() % 6) + 2;
    }
    
    


    

    sf::RenderStates rs;
    rs.texture = &glyph_map.getTexture();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if(event.type == sf::Event::KeyPressed)
			{
                if(event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
				if(event.key.code == sf::Keyboard::W)
                {

                }
			}
			if(event.type == sf::Event::KeyReleased)
			{
				if(event.key.code == sf::Keyboard::A)
                {
					
                }
			}
		}
		while (accumulator >= dt)
		{


            for (size_t i = 0; i < symbol_count; i++)
            {
                int index = i * 4;

                if((*va)[index+1].position.y > 720)
                {                    
                    int x = 1 + rand() % (( 1920 + 1 ) - 1);
                    int y = -16;
                    (*va)[index].position = sf::Vector2f(x,y);    
                    (*va)[index+1].position = sf::Vector2f(x+symbol_width,y);
                    (*va)[index+2].position = sf::Vector2f(x+symbol_width,y+16);
                    (*va)[index+3].position = sf::Vector2f(x,y+16);


                    int offset_x = 1 + rand() % (( 63 + 1 ) - 1)*symbol_width;
                    (*va)[index].texCoords = sf::Vector2f(offset_x,0);
                    (*va)[index+1].texCoords = sf::Vector2f(offset_x+symbol_width,0);
                    (*va)[index+2].texCoords = sf::Vector2f(offset_x+symbol_width,16);
                    (*va)[index+3].texCoords = sf::Vector2f(offset_x,16);

                    speed[i] = (rand() % 6) + 2;
                }
                else
                {
                    
                    int s = speed[i];
                    (*va)[index].position.y += s;    
                    (*va)[index+1].position.y += s;    
                    (*va)[index+2].position.y += s;    
                    (*va)[index+3].position.y += s; 
                }
                
            }
			
			float time_step = 1.0f / 60.0f;
            //window.clear(sf::Color(40,40,40));
            sf::VertexArray lol = sf::VertexArray(sf::PrimitiveType::Quads,4);

            lol[0].position = sf::Vector2f(0,0);
            lol[1].position = sf::Vector2f(1920,0);
            lol[2].position = sf::Vector2f(1920,720);
            lol[3].position = sf::Vector2f(0,720);

            lol[0].color = lol[1].color = lol[2].color = lol[3].color = sf::Color(0,0,0,40);
            window.draw(lol);
            window.draw(*va,&glyph_map.getTexture());
			window.display();
			accumulator -= dt;
		};
		accumulator += clock.getElapsedTime().asSeconds();

		clock.restart();		
	}

    return 0;

}