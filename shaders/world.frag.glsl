#version 330

struct Material{
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shininess;
};

struct PointLight{
    vec3 position;
    vec3 specular;
    vec3 attenuation; //quad, linear, const
};

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

in vec3 transferPosition;
in vec4 transferColor;
in vec3 transferNormal;
in vec2 transferUV;

uniform vec3 eye;

uniform Material material;

uniform PointLight pointLights[16];
uniform int pointLightCount;

uniform bool overrideColor;
uniform vec4 colorOverride;

uniform sampler2D activeTexture;

out vec4 fragColor;

float celShade(float c){
    return c;
}

vec3 celShade(vec3 c){
    return c;
}

void main() {
    
    if(overrideColor){
        
        fragColor = colorOverride;
        
    }else{
        
        vec3 lightColor = vec3(0);
        
        vec3 normal = normalize(normalMatrix * transferNormal);
        
        vec3 position = vec3(modelMatrix * vec4(transferPosition, 1));
        
        for(int i=0;i<pointLightCount;i++){
            
            vec3 surfaceToLight = pointLights[i].position - position;
            
            float dist = distance(pointLights[i].position, position);
            
            float NdotL = max(dot(normal, normalize(surfaceToLight)), 0.0);
            
            if(NdotL > 0){
            
                float a = pointLights[i].attenuation.x;
                float b = pointLights[i].attenuation.y;
                float c = pointLights[i].attenuation.z;
                float att = 1.0 / (a*dist*dist + b*dist + c);
                
                lightColor += att * (material.diffuse * NdotL);
                
                vec3 V = eye - position;
                vec3 H = normalize(surfaceToLight + V);
                
                float NdotH = max(dot(normal, H), 0.0);
                
                lightColor += pointLights[i].specular * att * material.specular * pow(NdotH, material.shininess);
            }
        
        }
        
        lightColor = celShade(lightColor);
        
        lightColor = max(lightColor, material.ambient);
        
        fragColor = texture(activeTexture, transferUV) * vec4(lightColor * transferColor.rgb, transferColor.a);
    }
}
