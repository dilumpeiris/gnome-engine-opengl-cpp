#pragma once

namespace GRectShader {
const char *fragmentSrc = R"(
                #version 330 core
        
                out vec4 FragColor;
                
                in vec3 ourColor;
                in vec2 TexCoord;
        
                uniform sampler2DArray spriteArray;
                uniform sampler2D mainTexture;

                uniform int isAnimated;
                uniform int frameIndex;
                uniform int hasColor;
                uniform vec4 solidColor;

                void main() {
                    if (hasColor == 1) {
                        FragColor = solidColor;
                    } else if (isAnimated == 1) {
                        FragColor = texture(spriteArray, vec3(TexCoord, float(frameIndex)));
                    } else {
                        FragColor = texture(mainTexture, TexCoord);
                    }
                }
            )";
}