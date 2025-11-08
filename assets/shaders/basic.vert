#version 330 core

layout (location = 0) in vec2 a_Position;   // position attribute
layout (location = 1) in vec2 a_TexCoord;   // optional texcoord (if using textures)

uniform mat4 u_Model;            // per-object transform
uniform mat4 u_ViewProjection;   // from your camera

out vec2 v_TexCoord;             // pass to fragment shader

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 0.0, 1.0);
}
