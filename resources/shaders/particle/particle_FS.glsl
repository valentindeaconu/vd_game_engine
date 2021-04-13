#version 410 core

in vec2 fTexCoords;
in float fLifePercentage;

out vec4 fColor;

#include <atlas.glsl>

uniform AtlasSampler2D uAtlasSampler;

float Map(float value, float fromL, float fromH, float toL, float toH) {
    return (value - fromL) * (toH - toL) / (fromH - fromL) + toL;
}

void main() {
    vec4 color = vec4(0);

    if (uAtlasSampler.Size > 1) {
        int N = (uAtlasSampler.Size * uAtlasSampler.Size) - 1;
        if (fLifePercentage < 1.0f) {
            float st = 1.0f / float(N);

            for (int i = 1; i <= N; ++i) {
                float minThreshold = (st * (i - 1));
                float maxThreshold = (st * i);

                if (fLifePercentage < maxThreshold) {
                    vec4 leftColor = AtlasSample(uAtlasSampler, (i - 1), fTexCoords);
                    vec4 rightColor = AtlasSample(uAtlasSampler, i, fTexCoords);

                    float localPercentage = Map(fLifePercentage, minThreshold, maxThreshold, 0.0f, 1.0f);

                    color = mix(leftColor, rightColor, localPercentage);

                    break;
                }
            }
        } else {
            color = AtlasSample(uAtlasSampler, N, fTexCoords);
        }
    } else {
        color = AtlasSample(uAtlasSampler, 0, fTexCoords);
    }
    
    if (color.a < 0.1f) {
        discard;
    }

    fColor = color;
}