
/*
 * Author: Dennis Lövkvist
 * Version: 1.0 (2020-05-01)
 */

#version 120

uniform float offset_x;
uniform float glitch_y0;
uniform float glitch_y1;
uniform float time;
uniform sampler2D texture;


float random (vec2 uv)
{
    return fract(sin(dot(uv,vec2(12.9898,78.233)))*43758.5453123);
}
void main()
{ 
    
    vec2 coord = gl_TexCoord[0].xy;

    vec4 pixel;

    if(coord.y > glitch_y0 && coord.y <glitch_y1)
    {   
        float lol = random(coord.xy)/100;

        coord.x -= offset_x;
        pixel = texture2D(texture, coord);

        pixel.r *= 1.4;
        pixel.g *= 1.6;
        pixel.b *= 1.8;
        
    }
    else
    {
        pixel = texture2D(texture, coord);
    }

     

	gl_FragColor = pixel;	


}