#version 330

in vec3 transferPosition;
in vec4 transferColor;
in vec2 transferUV;

uniform sampler2D activeTexture;

out vec4 fragColor;

void main() {

    fragColor = transferColor * texture(activeTexture, transferUV);
    
}
