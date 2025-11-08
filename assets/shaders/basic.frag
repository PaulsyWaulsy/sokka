#version 330 core

in vec2 v_TexCoord;
out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec4 u_Color; // tint or modulation color

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
