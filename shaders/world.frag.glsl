#version 330

in vec3 transferPosition;
in vec4 transferColor;
in vec3 transferUV;
in vec3 transferNormal;

out vec4 fragColor;

void main() {

    fragColor = transferColor;
}
