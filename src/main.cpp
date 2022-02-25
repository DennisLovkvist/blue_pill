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

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768
#define SYMBOL_WIDTH 16
#define SYMBOL_HEIGHT 16
#define GLYPH_WIDTH 12

#define PI 3.141

const int SYMBOLS_X_AXIS = SCREEN_WIDTH / SYMBOL_WIDTH;
const int SYMBOLS_Y_AXIS = SCREEN_HEIGHT / SYMBOL_HEIGHT;
const int SYMBOL_COUNT = SYMBOLS_X_AXIS*(SYMBOLS_Y_AXIS+1);

void BuildGlyphMap(sf::RenderTexture *glyph_map)
{
    sf::Font font;
    if (!font.loadFromFile("content/matrix.ttf"))
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

    for (size_t i = 0; i < 63; i++)
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

void GenerateRoundedEdge(sf::VertexArray* vertices,int x,int y,float r,int radius, int triangle_count,int vertex_count,sf::Color distortion_color)
{
    float step = (PI/2)/(triangle_count);

    for (size_t i = 0; i < vertex_count; i++)
    {
        sf::Vector2f* vertex_position = &(*vertices)[i].position;
        sf::Color* color = &(*vertices)[i].color;
        if(i%3 == 0)
        {
            vertex_position->x = x;
            vertex_position->y = y;  
            *color = sf::Color::Black;          
        }
        else if(i%3 == 1)
        {
            vertex_position->x = x+std::cos(r)*radius;
            vertex_position->y = y+std::sin(r)*radius; 
            *color = distortion_color;           
        }
        else if(i%3 == 2)
        {
            r+= step;
            vertex_position->x = x+std::cos(r)*radius;
            vertex_position->y = y+std::sin(r)*radius;
            *color = distortion_color;  
            
        }     
    }
}
void GenerateBlocks(sf::VertexArray* vertices,int inner_x, int inner_y,int radius)
{
    sf::Color color_low = sf::Color::Red;
    sf::Color color_high = sf::Color::Black;

    sf::Vector2f* vertex_position = &(*vertices)[0].position;
    sf::Color* color = &(*vertices)[0].color;
    
    //Middle rectangle
    vertex_position->x = inner_x;
    vertex_position->y = inner_y;
    *color = color_high;

    vertex_position = &(*vertices)[1].position;
    color = &(*vertices)[1].color;
    vertex_position->x = SCREEN_WIDTH - inner_x;
    vertex_position->y = inner_y;
    *color = color_high;

    vertex_position = &(*vertices)[2].position;
    color = &(*vertices)[2].color;
    vertex_position->x = SCREEN_WIDTH - inner_x;
    vertex_position->y = SCREEN_HEIGHT - inner_y;
    *color = color_high;

    vertex_position = &(*vertices)[3].position;
    color = &(*vertices)[3].color;
    vertex_position->x = inner_x;
    vertex_position->y = SCREEN_HEIGHT - inner_y;  
    *color = color_high;    


    //Left rectangle
    color_low = sf::Color::Red;
    vertex_position = &(*vertices)[4].position;
    color = &(*vertices)[4].color;
    vertex_position->x = inner_x-radius;
    vertex_position->y = inner_y;
    *color = color_low;

    vertex_position = &(*vertices)[5].position;
    color = &(*vertices)[5].color;
    vertex_position->x = inner_x;
    vertex_position->y = inner_y;
    *color = color_high;

    vertex_position = &(*vertices)[6].position;
    color = &(*vertices)[6].color;
    vertex_position->x = inner_x;
    vertex_position->y = SCREEN_HEIGHT - inner_y;
    *color = color_high;

    vertex_position = &(*vertices)[7].position;
    color = &(*vertices)[7].color;
    vertex_position->x = inner_x-radius;
    vertex_position->y = SCREEN_HEIGHT - inner_y;
    *color = color_low;

    //Right rectangle
    color_low = sf::Color::Blue;
    vertex_position = &(*vertices)[8].position;
    color = &(*vertices)[8].color;
    vertex_position->x = SCREEN_WIDTH - inner_x;
    vertex_position->y = inner_y;
    *color = color_high;

    vertex_position = &(*vertices)[9].position;
    color = &(*vertices)[9].color;
    vertex_position->x = SCREEN_WIDTH - inner_x+radius;
    vertex_position->y = inner_y;
    *color = color_low;

    vertex_position = &(*vertices)[10].position;
    color = &(*vertices)[10].color;
    vertex_position->x = SCREEN_WIDTH - inner_x+radius;
    vertex_position->y = SCREEN_HEIGHT - inner_y;
    *color = color_low;

    vertex_position = &(*vertices)[11].position;
    color = &(*vertices)[11].color;
    vertex_position->x = SCREEN_WIDTH - inner_x;
    vertex_position->y = SCREEN_HEIGHT - inner_y;
    *color = color_high;



    //Bottom rectangle
    vertex_position = &(*vertices)[12].position;
    color = &(*vertices)[12].color;
    vertex_position->x = inner_x;
    vertex_position->y = inner_y;
    *color = sf::Color::Black;

    vertex_position = &(*vertices)[13].position;
    color = &(*vertices)[13].color;
    vertex_position->x = SCREEN_WIDTH - inner_x;
    vertex_position->y = inner_y;
    *color = sf::Color::Black;

    vertex_position = &(*vertices)[14].position;
    color = &(*vertices)[14].color;
    vertex_position->x = SCREEN_WIDTH - inner_x;
    vertex_position->y = inner_y-radius;
    *color = sf::Color::Blue;

    vertex_position = &(*vertices)[15].position;
    color = &(*vertices)[15].color;
    vertex_position->x = inner_x;
    vertex_position->y = inner_y-radius;
    *color = sf::Color::Red;



    
    //Top rectangle
    vertex_position = &(*vertices)[16].position;
    color = &(*vertices)[16].color;
    vertex_position->x = inner_x;
    vertex_position->y = SCREEN_HEIGHT - inner_y;
    *color = sf::Color::Black;

    vertex_position = &(*vertices)[17].position;
    color = &(*vertices)[17].color;
    vertex_position->x = SCREEN_WIDTH - inner_x;
    vertex_position->y = SCREEN_HEIGHT - inner_y;
    *color = sf::Color::Black;

    vertex_position = &(*vertices)[18].position;
    color = &(*vertices)[18].color;
    vertex_position->x = SCREEN_WIDTH - inner_x;
    vertex_position->y = SCREEN_HEIGHT - inner_y+radius;
    *color = sf::Color::Blue;

    vertex_position = &(*vertices)[19].position;
    color = &(*vertices)[19].color;
    vertex_position->x = inner_x;
    vertex_position->y = SCREEN_HEIGHT - inner_y+radius;
    *color = sf::Color::Red;

    

}
int main()
{
   sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT), "SFML works!", sf::Style::Fullscreen);

	sf::View view = window.getDefaultView();
	sf::Clock clock;
	float dt = 1.f / 60.f;
	float accumulator = 0.f;
	float t = 0;

    sf::RenderTexture glyph_map;
    BuildGlyphMap(&glyph_map);  


    sf::RenderTexture distortion_map;
    distortion_map.create(SCREEN_WIDTH,SCREEN_HEIGHT);

    sf::Sprite distortion_map_sprite;
    distortion_map_sprite.setTexture(distortion_map.getTexture());

    int inner_x = 50;
    int inner_y = 50;
    int radius = 60;
    int triangle_count = 10;
    int vertex_count = 3*triangle_count;
    sf::VertexArray shape = sf::VertexArray(sf::PrimitiveType::Triangles,3*vertex_count);

    sf::VertexArray blocks = sf::VertexArray(sf::PrimitiveType::Quads,4*5);

    GenerateBlocks(&blocks,inner_x,inner_y,radius);

    sf::VertexArray side = sf::VertexArray(sf::PrimitiveType::Quads,4);  
    side[0].position.x = 0;
    side[0].position.y = 0;
    side[1].position.x = SCREEN_WIDTH/2;
    side[1].position.y = 0;
    side[2].position.x = SCREEN_WIDTH/2;
    side[2].position.y = SCREEN_HEIGHT;
    side[3].position.x = 0;
    side[3].position.y = SCREEN_HEIGHT;

    for (size_t i = 0; i < 4; i++)
    {
        side[i].color = sf::Color::Red;
    }
    

    distortion_map.draw(side);

    side[0].position.x = SCREEN_WIDTH/2;
    side[0].position.y = 0;
    side[1].position.x = SCREEN_WIDTH;
    side[1].position.y = 0;
    side[2].position.x = SCREEN_WIDTH;
    side[2].position.y = SCREEN_HEIGHT;
    side[3].position.x = SCREEN_WIDTH/2;
    side[3].position.y = SCREEN_HEIGHT;

    for (size_t i = 0; i < 4; i++)
    {
        side[i].color = sf::Color::Blue;
    }

    distortion_map.draw(side);


    distortion_map.draw(blocks);

    GenerateRoundedEdge(&shape,inner_x,SCREEN_HEIGHT-inner_y,PI/2,radius,triangle_count,vertex_count,sf::Color::Red);   
    distortion_map.draw(shape);

    GenerateRoundedEdge(&shape,SCREEN_WIDTH - inner_x,SCREEN_HEIGHT-inner_y,0.0f,radius,triangle_count,vertex_count,sf::Color::Blue);   
    distortion_map.draw(shape);

    GenerateRoundedEdge(&shape,inner_x,inner_y,PI,radius,triangle_count,vertex_count,sf::Color::Red);   
    distortion_map.draw(shape);

    GenerateRoundedEdge(&shape,SCREEN_WIDTH-inner_x,inner_y,-PI/2,radius,triangle_count,vertex_count,sf::Color::Blue);   
    distortion_map.draw(shape);

    sf::RenderTexture render_target;
    render_target.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    sf::Sprite render_target_sprite;
    render_target_sprite.setTexture(render_target.getTexture());
    render_target_sprite.setScale(1,-1);
    render_target_sprite.setPosition(0,SCREEN_HEIGHT);


    sf::RenderTexture render_target_2;
    render_target_2.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    sf::Sprite render_target_2_sprite;
    render_target_2_sprite.setTexture(render_target_2.getTexture());
    render_target_2_sprite.setScale(1,-1);
    render_target_2_sprite.setPosition(0,SCREEN_HEIGHT);


    sf::Shader shader_monitor_distortion;
	shader_monitor_distortion.loadFromFile("content/shaders/shader_monitor_distortion.frag", sf::Shader::Fragment);
    shader_monitor_distortion.setUniform("hmap",distortion_map.getTexture());

    sf::Shader shader_monitor_glitch;
	shader_monitor_glitch.loadFromFile("content/shaders/shader_monitor_glitch.frag", sf::Shader::Fragment);
    


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
        vertex_2->position.y = y+SYMBOL_HEIGHT;

        vertex_3->position.x = x;
        vertex_3->position.y = y+SYMBOL_HEIGHT;

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


    float r0 = 0;
                float r1 = 0;
    
float offset_x = 0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
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

           

            render_target.draw(fade);
            render_target.draw(*vertices,&glyph_map.getTexture());



            render_target_2.clear(sf::Color::Black);
            render_target_2.draw(render_target_sprite,&shader_monitor_distortion);



            if(r0 <= 10)
            {
                r0 = 1080;
                r1 = 1 + rand() % (( 10 + 1 ) - 1);  
                offset_x = 0.01f;    
            }
            else
            {
                if(1 + rand() % (( 100 + 1 ) - 1) < 50)
                {
                    offset_x = 0;
                }
                else
                {
                    offset_x = (float)(1 + rand() % (( 100 + 1 ) - 1)-50)/20000.0f;
                }

                float n = 1 + rand() % (( 25 + 1 ) - 1)/10.0;
                r0 -= n;
                
            }

            std::cout << (r0/1080.0f) << std::endl;

shader_monitor_glitch.setUniform("offset_x",offset_x);
            shader_monitor_glitch.setUniform("glitch_y0",(r0/1080.0f));
                shader_monitor_glitch.setUniform("glitch_y1",((r0+r1)/1080.0f));   


window.clear(sf::Color::Black);
                window.draw(render_target_2_sprite,&shader_monitor_glitch);

            //distortion_map_sprite.setColor(sf::Color(255,255,255,30));
           // window.draw(distortion_map_sprite);
			window.display();
            
            t += dt;

			accumulator -= dt;
		};
        
		accumulator += clock.getElapsedTime().asSeconds();

		clock.restart();		
	}

    return 0;

}