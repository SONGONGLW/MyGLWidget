#version 330 core

layout(location = 1) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

out vec2 fragTexCoord;
out vec3 fragTexCoordColor;

uniform bool isTexture;

void main()
{
    if(isTexture)
    {
        gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
    }
    else
    {
        gl_Position = vec4(inPosition.x, -inPosition.y, inPosition.z, 1.0);
    }
    fragTexCoord = inTexCoord;
    fragTexCoordColor = inColor;
}
