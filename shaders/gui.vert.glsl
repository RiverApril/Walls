#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexUV;
layout(location = 3) in vec3 vertexNormal;

out vec3 transferPosition;
out vec4 transferColor;
out vec3 transferUV;
out vec3 transferNormal;


void main() {
	gl_Position = vec4(vertexPosition, 1);
	
	transferPosition = vertexPosition;
    transferColor = vertexColor;
	transferUV = vertexUV;
	transferNormal = vertexNormal;
}
