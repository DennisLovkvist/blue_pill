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

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SYMBOL_WIDTH 16
#define SYMBOL_HEIGHT 16
const int SYMBOLS_X_AXIS = SCREEN_WIDTH / SYMBOL_WIDTH;
const int SYMBOLS_Y_AXIS = SCREEN_HEIGHT / SYMBOL_HEIGHT;
const int SYMBOL_COUNT = SYMBOLS_X_AXIS*(SYMBOLS_Y_AXIS+1);

int main()
{
   sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT), "SFML works!", sf::Style::Close);

	sf::View view = window.getDefaultView();
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
    int glyph_width = 12;
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
        characters.setFillColor(sf::Color::White);
        int width = characters.getLocalBounds().width;

        //OPENGL flips coords during rendering
        characters.setScale(1.0, -1.0);
        characters.setOrigin(0,18);

        int x = (glyph_width/2)-(width/2);

        characters.setPosition((glyph_width*i+x),0);
        glyph_map.draw(characters);
    }
    

    

    sf::Sprite glyph_map_sprite;
    glyph_map_sprite.setTexture(glyph_map.getTexture());
    
    window.draw(glyph_map_sprite);

    float *speed = new float[SYMBOLS_X_AXIS];

    int *focus_index = new int[SYMBOLS_X_AXIS];

    sf::VertexArray* va = new sf::VertexArray(sf::PrimitiveType::Quads,SYMBOL_COUNT*4);

    for (int i = 0; i < SYMBOL_COUNT; i++)
    {
        int index = i * 4;   
        int x = (i % SYMBOLS_X_AXIS)*SYMBOL_WIDTH;   
        int y = (i / SYMBOLS_X_AXIS)*SYMBOL_HEIGHT;    

        
        std::cout << x << "," << y << std::endl; 

        (*va)[index].position = sf::Vector2f(x,y);    
        (*va)[index+1].position = sf::Vector2f(x+glyph_width,y);
        (*va)[index+2].position = sf::Vector2f(x+glyph_width,y+SYMBOL_WIDTH);
        (*va)[index+3].position = sf::Vector2f(x,y+SYMBOL_WIDTH);


        int offset_x = 1 + rand() % (( 62 + 1 ) - 1)*glyph_width;
        (*va)[index].texCoords = sf::Vector2f(offset_x,0);
        (*va)[index+1].texCoords = sf::Vector2f(offset_x+glyph_width,0);
        (*va)[index+2].texCoords = sf::Vector2f(offset_x+glyph_width,SYMBOL_HEIGHT);
        (*va)[index+3].texCoords = sf::Vector2f(offset_x,SYMBOL_HEIGHT);

        

        
    }


    for (size_t i = 0; i < SYMBOLS_X_AXIS; i++)
    {
        focus_index[i] = 1 + rand() % (( SYMBOLS_Y_AXIS + 1 ) - 1);

        speed[i] = (float)(15 + rand() % (( 20 + 15 ) - 1))/10.0f;
        
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


            for (int i = 0; i < SYMBOL_COUNT; i++)
            {
                int index = i * 4;
                int x = (i % SYMBOLS_X_AXIS)*SYMBOL_WIDTH;   
                int y = (i / SYMBOLS_X_AXIS)*SYMBOL_HEIGHT;  

                if((*va)[index+1].position.y > SCREEN_HEIGHT)
                {                    
                    int y = -8;
                    (*va)[index].position.y = y;    
                    (*va)[index+1].position.y = y;
                    (*va)[index+2].position.y = y+16;
                    (*va)[index+3].position.y=y+16;


                    int offset_x = 1 + rand() % (( 63 + 1 ) - 1)*glyph_width;
                    (*va)[index].texCoords = sf::Vector2f(offset_x,0);
                    (*va)[index+1].texCoords = sf::Vector2f(offset_x+glyph_width,0);
                    (*va)[index+2].texCoords = sf::Vector2f(offset_x+glyph_width,16);
                    (*va)[index+3].texCoords = sf::Vector2f(offset_x,16);

                }
                else
                {
                    
                    float s = speed[(i % SYMBOLS_X_AXIS)];
                    (*va)[index].position.y += s;    
                    (*va)[index+1].position.y += s;    
                    (*va)[index+2].position.y += s;    
                    (*va)[index+3].position.y += s; 

                    int lol = (*va)[index].position.y;
                    

                    int pung = (1 + rand() % (( 16 + 1 ) - 1));

                    if(lol % pung == 0)
                    {                            
                        (*va)[index].color.g = (*va)[index+1].color.g = (*va)[index+2].color.g = (*va)[index+3].color.g = (*va)[index].color.g = 1 + rand() % (( 255 + 1 ) - 1);
                        (*va)[index].color.r = (*va)[index+1].color.r = (*va)[index+2].color.r = (*va)[index+3].color.r = (*va)[index].color.g = 1 + rand() % (( 50 + 1 ) - 1);
                        (*va)[index].color.b = (*va)[index+1].color.b = (*va)[index+2].color.b = (*va)[index+3].color.b = (*va)[index].color.g = 1 + rand() % (( 50 + 1 ) - 1);


                        int offset_x = 1 + rand() % (( 63 + 1 ) - 1)*glyph_width;
                        (*va)[index].texCoords = sf::Vector2f(offset_x,0);
                        (*va)[index+1].texCoords = sf::Vector2f(offset_x+glyph_width,0);
                        (*va)[index+2].texCoords = sf::Vector2f(offset_x+glyph_width,16);
                        (*va)[index+3].texCoords = sf::Vector2f(offset_x,16);
                    }

                }
                
            }
			



            for (size_t x = 0; x < SYMBOLS_X_AXIS; x++)
            {
                int index = (x + SYMBOLS_X_AXIS*focus_index[x])*4;

                (*va)[index].color.r = (*va)[index].color.g = (*va)[index].color.b = 255;
                (*va)[index+1].color.r = (*va)[index+1].color.g = (*va)[index+1].color.b = 255;
                (*va)[index+2].color.r = (*va)[index+2].color.g = (*va)[index+2].color.b = 255;
                (*va)[index+3].color.r = (*va)[index+3].color.g = (*va)[index+3].color.b = 255;
            }
            


			float time_step = 1.0f / 60.0f;
            //window.clear(sf::Color(40,40,40));
            
            sf::VertexArray lol = sf::VertexArray(sf::PrimitiveType::Quads,4);

            lol[0].position = sf::Vector2f(0,0);
            lol[1].position = sf::Vector2f(SCREEN_WIDTH,0);
            lol[2].position = sf::Vector2f(SCREEN_WIDTH,SCREEN_HEIGHT);
            lol[3].position = sf::Vector2f(0,SCREEN_HEIGHT);

            lol[0].color = lol[1].color = lol[2].color = lol[3].color = sf::Color(0,0,0,50);
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