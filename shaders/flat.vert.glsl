#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;

uniform mat4 viewMatrix;

out vec3 transferPosition;
out vec4 transferColor;
out vec2 transferUV;


void main() {
	gl_Position = viewMatrix * vec4(vertexPosition, 1);
	
	transferPosition = vertexPosition;
    transferColor = vertexColor;
    transferUV.x = vertexUV.x;
    transferUV.y = 1.0f - vertexUV.y;
}
