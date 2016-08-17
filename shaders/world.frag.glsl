#version 330

struct PointLight{
    vec3 position;
    vec3 color;
    float intensity;
};

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

in vec3 transferPosition;
in vec4 transferColor;
in vec3 transferNormal;

uniform PointLight pointLights[16];
uniform int pointLightCount;

out vec4 fragColor;

void main() {
    
    vec3 lightColor = vec3(0);
    
    vec3 normal = normalize(normalMatrix * transferNormal);
    
    vec3 position = vec3(modelMatrix * vec4(transferPosition, 1));
    
    for(int i=0;i<pointLightCount;i++){
        
        vec3 surfaceToLight = pointLights[i].position - position;
        
        float dist = distance(pointLights[i].position, position);
        
        float a = 0; float b = 1.0 / pointLights[i].intensity;
        float att = 1.0 / (1.0 + a*dist + b*dist*dist);
        
        lightColor += pointLights[i].color * (att * dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal)));
        
    }
    
    fragColor = vec4(lightColor * transferColor.rgb, transferColor.a);
}
