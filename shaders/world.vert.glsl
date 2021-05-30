#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexUV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 transferPosition;
out vec4 transferColor;
out vec3 transferNormal;
out vec2 transferUV;


void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	
	transferPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));;
    transferColor = vertexColor;
    transferNormal = normalize(transpose(inverse(mat3(modelMatrix))) * vertexNormal);
    transferUV.x = vertexUV.x;
    transferUV.y = 1.0f - vertexUV.y;
}
