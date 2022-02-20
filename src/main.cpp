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
#define GLYPH_WIDTH 12

const int SYMBOLS_X_AXIS = SCREEN_WIDTH / SYMBOL_WIDTH;
const int SYMBOLS_Y_AXIS = SCREEN_HEIGHT / SYMBOL_HEIGHT;
const int SYMBOL_COUNT = SYMBOLS_X_AXIS*(SYMBOLS_Y_AXIS+1);



void BuildGlyphMap(sf::RenderTexture *glyph_map)
{
    sf::Font font;
    if (!font.loadFromFile("content/arial.ttf"))
    {
        std::cout << "Cannot find font at " << ("content/matrix.ttf") << std::endl << "Press [ENTER] to exit." << std::endl;
        std::cin.ignore(); 
    }
    
    glyph_map->create(1332, 18);

    sf::Text characters;
    characters.setLetterSpacing(10);
    characters.setFont(font);
    char c[63] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";   

	glyph_map->clear(sf::Color(255,255,255,0));

    for (size_t i = 0; i < 62; i++)
    {
        characters.setString(c[i]);
        characters.setCharacterSize(16); 
        characters.setFillColor(sf::Color::White);
        int width = characters.getLocalBounds().width;

        //OPENGL flips coords during rendering
        characters.setScale(1.0, -1.0);
        characters.setOrigin(0,18);

        int x = (GLYPH_WIDTH/2)-(width/2);

        characters.setPosition((GLYPH_WIDTH*i+x),0);
        glyph_map->draw(characters);
    }
}


int main()
{
   sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT), "SFML works!", sf::Style::Close);

	sf::View view = window.getDefaultView();
	sf::Clock clock;
	float dt = 1.f / 60.f;
	float accumulator = 0.f;
	float t = 0;

    sf::RenderTexture glyph_map;
    BuildGlyphMap(&glyph_map);  

    float *speed = new float[SYMBOLS_X_AXIS];

    int *focus_index = new int[SYMBOLS_X_AXIS];

    sf::VertexArray* vertices = new sf::VertexArray(sf::PrimitiveType::Quads,SYMBOL_COUNT*4);

    for (int i = 0; i < SYMBOL_COUNT; i++)
    {
        int index = i * 4;   
        int x = (i % SYMBOLS_X_AXIS)*SYMBOL_WIDTH;   
        int y = (i / SYMBOLS_X_AXIS)*SYMBOL_HEIGHT;  
        
        sf::Vertex* vertex_0 = &(*vertices)[index];
        sf::Vertex* vertex_1 = &(*vertices)[index+1];
        sf::Vertex* vertex_2 = &(*vertices)[index+2];
        sf::Vertex* vertex_3 = &(*vertices)[index+3];

        vertex_0->position.x = x;
        vertex_0->position.y = y; 

        vertex_1->position.x = x+GLYPH_WIDTH;
        vertex_1->position.y = y;

        vertex_2->position.x = x+GLYPH_WIDTH;
        vertex_2->position.y = y+SYMBOL_WIDTH;

        vertex_3->position.x = x;
        vertex_3->position.y = y+SYMBOL_WIDTH;

        int offset_x = 1 + rand() % (( 62 + 1 ) - 1)*GLYPH_WIDTH;

        vertex_0->texCoords.x = offset_x;
        vertex_0->texCoords.y = 0;

        vertex_1->texCoords.x = offset_x+GLYPH_WIDTH;
        vertex_1->texCoords.y = 0;

        vertex_2->texCoords.x = offset_x+GLYPH_WIDTH,SYMBOL_HEIGHT;
        vertex_2->texCoords.y = SYMBOL_HEIGHT;

        vertex_3->texCoords.x = offset_x;
        vertex_3->texCoords.y = SYMBOL_HEIGHT;          
    }

    for (size_t i = 0; i < SYMBOLS_X_AXIS; i++)
    {
        focus_index[i] = 1 + rand() % (( SYMBOLS_Y_AXIS + 1 ) - 1);

        speed[i] = (float)(15 + rand() % (( 20 + 15 ) - 1))/10.0f;
        
    }  
    
    sf::VertexArray fade = sf::VertexArray(sf::PrimitiveType::Quads,4);
    fade[0].position = sf::Vector2f(0,0);
    fade[1].position = sf::Vector2f(SCREEN_WIDTH,0);
    fade[2].position = sf::Vector2f(SCREEN_WIDTH,SCREEN_HEIGHT);
    fade[3].position = sf::Vector2f(0,SCREEN_HEIGHT);
    fade[0].color = fade[1].color = fade[2].color = fade[3].color = sf::Color(0,0,0,50);    

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
		}
		while (accumulator >= dt)
		{


            for (int i = 0; i < SYMBOL_COUNT; i++)
            {
                int index = i * 4;
                int x = (i % SYMBOLS_X_AXIS)*SYMBOL_WIDTH;   
                int y = (i / SYMBOLS_X_AXIS)*SYMBOL_HEIGHT;  

                sf::Vertex* vertex_0 = &(*vertices)[index];
                sf::Vertex* vertex_1 = &(*vertices)[index+1];
                sf::Vertex* vertex_2 = &(*vertices)[index+2];
                sf::Vertex* vertex_3 = &(*vertices)[index+3];

                if(vertex_0->position.y > SCREEN_HEIGHT)
                {                    
                    int y = -8;
                    vertex_0->position.y = y;    
                    vertex_1->position.y = y;
                    vertex_2->position.y = y + 16;
                    vertex_3->position.y = y + 16;

                    int offset_x = 1 + rand() % (( 63 + 1 ) - 1) * GLYPH_WIDTH;
                    vertex_0->texCoords.x = offset_x;
                    vertex_0->texCoords.y = 0;

                    vertex_1->texCoords.x = offset_x + GLYPH_WIDTH;
                    vertex_1->texCoords.y = 0;

                    vertex_2->texCoords.x = offset_x + GLYPH_WIDTH;
                    vertex_2->texCoords.y = 16;

                    vertex_3->texCoords.x = offset_x;
                    vertex_3->texCoords.y = 16;
                }
                else
                {                    
                    float s = speed[(i % SYMBOLS_X_AXIS)];
                    vertex_0->position.y += s;    
                    vertex_1->position.y += s;    
                    vertex_2->position.y += s;    
                    vertex_3->position.y += s;   

                    int n = (1 + rand() % (( 16 + 1 ) - 1));                    
                    int symbol_position_y = vertex_0->position.y;

                    if(symbol_position_y % n == 0)
                    {      
                        sf::Color* color_0 = &(vertex_0->color);
                        sf::Color* color_1 = &(vertex_1->color);
                        sf::Color* color_2 = &(vertex_2->color);
                        sf::Color* color_3 = &(vertex_3->color);  

                        color_0->r = color_1->r = color_2->r = color_3->r = 1 + rand() % (( 50 + 1 ) - 1);
                        color_0->g = color_1->g = color_2->g = color_3->g = 1 + rand() % (( 200 + 1 ) - 1);
                        color_0->b = color_1->b = color_2->b = color_3->b = 1 + rand() % (( 50 + 1 ) - 1);

                        int offset_x = 1 + rand() % (( 63 + 1 ) - 1)*GLYPH_WIDTH;
                        vertex_0->texCoords.x = offset_x;
                        vertex_0->texCoords.y = 0;

                        vertex_1->texCoords.x = offset_x+GLYPH_WIDTH;
                        vertex_1->texCoords.y = 0;

                        vertex_2->texCoords.x = offset_x+GLYPH_WIDTH;
                        vertex_2->texCoords.y = 16;

                        vertex_3->texCoords.x = offset_x;
                        vertex_3->texCoords.y = 16;
                    }
                }
            }	
            for (size_t x = 0; x < SYMBOLS_X_AXIS; x++)
            {
                int index = (x + SYMBOLS_X_AXIS*focus_index[x])*4;

                sf::Color* color_0 = &(*vertices)[index].color;
                sf::Color* color_1 = &(*vertices)[index+1].color;
                sf::Color* color_2 = &(*vertices)[index+2].color;
                sf::Color* color_3 = &(*vertices)[index+3].color;

                color_0->r = color_1->r = color_2->r = color_3->r = 255;
                color_0->g = color_1->g = color_2->g = color_3->g = 255;
                color_0->b = color_1->b = color_2->b = color_3->b = 255;
            }
           
            window.draw(fade);
            window.draw(*vertices,&glyph_map.getTexture());
			window.display();

			accumulator -= dt;
		};
        
		accumulator += clock.getElapsedTime().asSeconds();

		clock.restart();		
	}

    return 0;

}