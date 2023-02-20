#version 330

in vec3 Normal;
in vec3 Position;
in vec2 TexCoord;

out vec4 FragColor;

struct PointLight {
    vec3 Color;
    vec3 Position;
};

uniform int gUseTexture;
uniform vec4 gTexColor;
uniform sampler2D gSampler;
uniform vec3 gEyePosition;
uniform PointLight gPointLight;

float gAmbientLight = 0.22f;
float gDiffuseIntensity = 0.64f;
float gSpecularIntensity = 0.82f;
float gSpecularPower = 8.0f;
float gConstant = 0.36f;
float gLinear = 0.1f;
float gExp = 0.1f;

vec4 calcDiffuseLight(vec3 color, vec3 normal, vec3 direction, float intensity) {
    float factor = dot(normalize(normal), normalize(-direction));
    if (factor > 0) {
        return vec4(color * intensity * factor, 1.0f);
    } else {
        return vec4(vec3(0.0f), 0.0f);
    }
}

vec4 calcSpecularLight(vec3 color, vec3 normal, vec3 direction, vec3 eyePosition, float intensity, float power) {
    float sign = dot(normal, -direction);
    if (sign > 0) {
        vec3 vertexToEye = normalize(eyePosition - Position);
        vec3 reflection = normalize(reflect(direction, normal));
        float factor = dot(vertexToEye, reflection);
        if (factor > 0) {
            factor = pow(factor, power);
            return vec4(color * intensity * factor, 1.0f);
        } else {
            return vec4(vec3(0.0f), 0.0f);
        }
    }
    return vec4(vec3(0.0f), 0.0f);
}

vec4 calcPointLight(PointLight light) {
    vec3 lightDirection = Position - light.Position;
    float distance = length(lightDirection);
    vec4 diffuseLignt = calcDiffuseLight(light.Color, Normal, lightDirection, gDiffuseIntensity);
    vec4 specularLight = calcSpecularLight(light.Color, Normal, lightDirection, gEyePosition, gSpecularIntensity, gSpecularPower);
    float attenutation = gConstant + gLinear * distance + gExp * distance * distance;
    return (diffuseLignt + specularLight) / attenutation;
}

void main()
{
    vec4 light = vec4(vec3(gAmbientLight), 1.0f);
    light += calcPointLight(gPointLight);
    if (gUseTexture > 0) {
        FragColor = light * texture2D(gSampler, TexCoord.st) * gTexColor;
    } else {
        FragColor = light * gTexColor;
    }
}