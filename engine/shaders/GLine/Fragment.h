#pragma once

namespace GLineShader {
const char *fragmentSrc = R"(
                #version 330 core
        
                out vec4 FragColor;
                
                uniform vec4 solidColor;
                uniform int hasColor;

                void main() {
                    if (hasColor == 1) {
                        FragColor = solidColor;
                    } else {
                        // Default fallback
                        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
                    }
                }
            )";
}
