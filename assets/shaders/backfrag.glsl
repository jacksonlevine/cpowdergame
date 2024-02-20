#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
void main() {
    vec4 color = texture(ourTexture, TexCoord);
    FragColor = vec4(color.r, color.r, color.r, 1.0);
}