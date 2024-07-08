#version 330 core

in vec2 fragTexCoord;
in vec3 fragTexCoordColor;

out vec4 fragColor;

uniform bool isTexture;

uniform sampler2D textureSampler;
uniform sampler2D textureSamplerText;

void main()
{
    if(isTexture)
    {
        // 使用第一个纹理
        vec4 color1 = texture(textureSampler, fragTexCoord);

        // 使用第二个纹理
        vec4 color2 = texture(textureSamplerText, fragTexCoord);

        /**添加颜色 vec4(fragTexCoordColor, 1.0)*/
//        fragColor = mix(color1, color2, 0.5); // 这里将两个纹理混合
        fragColor = color1 /** vec4(fragTexCoordColor, 1.0)*/;
    }
    else
    {
        //指定颜色
        fragColor = vec4(fragTexCoordColor, 1.0f);
    }

}
