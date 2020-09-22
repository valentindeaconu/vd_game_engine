#version 430

layout (location = 0) in vec2 vPosition;

out vec2 tcTexCoord;

uniform mat4 localModel;
uniform mat4 worldModel;

uniform vec3 cameraPosition;

uniform float scaleY;

uniform int lod;
uniform vec2 index;
uniform float gap;
uniform vec2 position;
uniform int lodMorphArea;

uniform sampler2D heightMap;

float morphLatitude(vec2 localPosition) {
    vec2 frac = localPosition - position;

    if (index == vec2(0.0f, 0.0f)) {
        // bottom left
        float morph = frac.x - frac.y;
        if (morph > 0.0f) return morph;
    }
    else if (index == vec2(1.0f, 0.0f)) {
        // bottom right
        float morph = gap - frac.x - frac.y;
        if (morph > 0.0f) return morph;
    }
    else if (index == vec2(0.0f, 1.0f)) {
        // top left
        float morph = frac.x + frac.y - gap;
        if (morph > 0.0f) return -morph;
    }
    else if (index == vec2(1.0f, 1.0f)) {
        // top right
        float morph = frac.y - frac.x;
        if (morph > 0.0f) return -morph;
    }
    return 0.0f;
}

float morphLongitude(vec2 localPosition) {
    vec2 frac = localPosition - position;

    if (index == vec2(0.0f, 0.0f)) {
        // bottom left
        float morph = frac.y - frac.x;
        if (morph > 0.0f) return -morph;
    }
    else if (index == vec2(1.0f, 0.0f)) {
        // bottom right
        float morph = frac.y - (gap - frac.x);
        if (morph > 0.0f) return morph;
    }
    else if (index == vec2(0.0f, 1.0f)) {
        // top left
        float morph = gap - frac.y - frac.x;
        if (morph > 0.0f) return -morph;
    }
    else if (index == vec2(1.0f, 1.0f)) {
        // top right
        float morph = frac.x - frac.y;
        if (morph > 0.0f) return morph;
    }
    return 0.0f;
}

vec2 morph(vec2 localPosition, float height, int morphArea) {
    vec2 morphing = vec2(0.0f, 0.0f);

    vec2 fixPointLatitude = vec2(0.0f, 0.0f);
    vec2 fixPointLongitude = vec2(0.0f, 0.0f);
    float distLatitude;
    float distLongitude;

    if (index == vec2(0.0f, 0.0f)) {
        // bottom left
        fixPointLatitude = position + vec2(gap, 0.0f);
        fixPointLongitude = position + vec2(0.0f, gap);
    }
    else if (index == vec2(1.0f, 0.0f)) {
        // bottom right
        fixPointLatitude = position;
        fixPointLongitude = position + vec2(gap, gap);
    }
    else if (index == vec2(0.0f, 1.0f)) {
        // top left
        fixPointLatitude = position + vec2(gap, gap);
        fixPointLongitude = position;
    }
    else if (index == vec2(1.0f, 1.0f)) {
        // top right
        fixPointLatitude = position + vec2(0.0f, gap);
        fixPointLongitude = position + vec2(gap, 0.0f);
    }

    vec4 worldFixPointLatitude = worldModel * vec4(fixPointLatitude.x, height, fixPointLatitude.y, 1.0f);
    distLatitude = length(cameraPosition - worldFixPointLatitude.xyz);

    vec4 worldFixPointLongitude = worldModel * vec4(fixPointLongitude.x, height, fixPointLongitude.y, 1.0f);
    distLongitude = length(cameraPosition - worldFixPointLongitude.xyz);

    if (distLatitude > morphArea) {
        morphing.x = morphLatitude(localPosition.xy);
    }
    if (distLongitude > morphArea) {
        morphing.y = morphLongitude(localPosition.xy);
    }

    return morphing;
}

void main() {
    vec2 localPosition = (localModel * vec4(vPosition.x, 0.0f, vPosition.y, 1.0f)).xz;

    float height = texture(heightMap, localPosition).r;
    if (lod > 0) {
        vec2 morphing = morph(localPosition, height, lodMorphArea);
        //localPosition += morphing;
        while (morphing == vec2(1.0f)) break;
    }

    height = texture(heightMap, localPosition).r;

    tcTexCoord = localPosition;

    gl_Position = worldModel * vec4(localPosition.x, height, localPosition.y, 1.0f);
}