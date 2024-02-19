#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
uniform vec3 colorPalette[16];
void main() {
    float theByte = texture(ourTexture, TexCoord).r * 255.0;
    int colorIndex = int(theByte) & 0x0F; // 0x0F is 00001111, so we get the 4 lower bytes only
    vec3 color = colorPalette[colorIndex];
    FragColor = texture(ourTexture, TexCoord);
}