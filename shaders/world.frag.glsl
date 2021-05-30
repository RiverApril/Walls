#version 410

#define MAX_LIGHT_COUNT 32
#define NORMAL_BIAS 0.01
#define CEL_QTY 8

struct Material{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
};

struct PointLight{
    vec3 position;
    vec3 specular;
    vec3 diffuse;
    vec3 attenuation; //quad, linear, const
};

in vec3 transferPosition;
in vec4 transferColor;
in vec3 transferNormal;
in vec2 transferUV;

uniform vec3 eye;

uniform Material material;

uniform PointLight pointLights[MAX_LIGHT_COUNT];
uniform int pointLightCount;
uniform int pointLightFarPlane;

uniform bool overrideColor;
uniform vec4 colorOverride;

uniform sampler2D activeTexture;
uniform samplerCubeArrayShadow depthMap;

out vec4 fragColor;

float celShade(float c){
    return floor(c * CEL_QTY) / CEL_QTY;
}

vec3 celShade(vec3 c){
    return vec3(celShade(c.r), celShade(c.g), celShade(c.b));
}

float calculateShadow(vec3 surfaceToLight, int lightIndex) {
    float currentDepth = length(surfaceToLight) / pointLightFarPlane;
    float shadow = texture(depthMap, vec4(surfaceToLight, lightIndex), currentDepth);

    return shadow;
}

void main() {
    
    if(overrideColor){
        
        fragColor = colorOverride;
        
    }else{
        
        vec3 lightColor = vec3(0);
        
        vec3 normal = transferNormal;
        
        vec3 position = transferPosition;
        
        for(int i = 0; i < pointLightCount; i++){
            
            vec3 surfaceToLight = pointLights[i].position - position;
            
            float dist = distance(pointLights[i].position, position);
            
            float NdotL = max(dot(normal, normalize(surfaceToLight)), 0.0);
            
            if(NdotL > 0){
            
                float a = pointLights[i].attenuation.x;
                float b = pointLights[i].attenuation.y;
                float c = pointLights[i].attenuation.z;
                float att = 1.0 / (a*dist*dist + b*dist + c);

                float shadow = calculateShadow(position - pointLights[i].position + normal * NORMAL_BIAS, i);

                vec3 diffuseLight = pointLights[i].diffuse * att * (material.diffuse * NdotL);
                
                vec3 V = eye - position;
                vec3 H = normalize(surfaceToLight + V);
                
                float NdotH = max(dot(normal, H), 0.0);
                
                vec3 specularLight = pointLights[i].specular * att * material.specular * pow(NdotH, material.shininess);

                lightColor = max(lightColor, (1.0 - shadow) * (diffuseLight + specularLight));
            }
        
        }
        
        // lightColor = celShade(lightColor);
        
        lightColor = max(lightColor, material.ambient);
        
        fragColor = texture(activeTexture, transferUV) * vec4(lightColor * transferColor.rgb, transferColor.a);
    }
}
