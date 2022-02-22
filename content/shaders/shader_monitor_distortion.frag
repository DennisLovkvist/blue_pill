

  
/*
 * Author: Dennis Lövkvist
 * Version: 1.0 (2020-05-01)
 */

#version 120

uniform sampler2D texture;
uniform sampler2D hmap;

float W[13] =float[]( 

    0.002216,
    0.008764,
   0.026995,
   0.064759,
   0.120985,
   0.176033,
   0.199471,
   0.176033,
   0.120985,
   0.064759,
   0.026995,
   0.008764,
   0.002216
);
float K[13] = float[]( -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6);


void main()
{ 
    
    vec4 col = vec4(0,0,0,1); 
    float width = 1920/2;  

    vec4 pixel = texture2D(hmap, gl_TexCoord[0].xy);

    float h = 1 * pixel.b + pixel.r;
    float lol = (cos(pixel.r) - cos(pixel.b))/100;

    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[0]  / width),0)) * W[0];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[1]  / width),0)) * W[1];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[2]  / width),0)) * W[2];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[3]  / width),0)) * W[3];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[4]  / width),0)) * W[4];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[5]  / width),0)) * W[5];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[6]  / width),0)) * W[6];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[7]  / width),0)) * W[7];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[8]  / width),0)) * W[8];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[9]  / width),0)) * W[9];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[10] / width),0)) * W[10];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[11] / width),0)) * W[11];
    col += texture2D(texture, gl_TexCoord[0].xy + lol + vec2(h*(K[12] / width),0)) * W[12];  
   
    
	gl_FragColor = col*(1-(pixel.r + pixel.b));	

}