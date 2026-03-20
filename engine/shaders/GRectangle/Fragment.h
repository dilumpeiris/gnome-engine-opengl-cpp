#pragma once

namespace GRectShader {
const char *fragmentSrc = R"(
                #version 330 core
        
                out vec4 FragColor;
                
                in vec3 ourColor;
                in vec2 TexCoord;
        
                uniform sampler2DArray spriteArray;

                uniform int frameIndex;
                void main() {
        
                FragColor = texture(spriteArray, vec3(TexCoord, frameIndex));
                }
            )";
}