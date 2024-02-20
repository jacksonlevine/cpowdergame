#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
uniform vec4 colorPalette[16];
void main() {
    float theByte = texture(ourTexture, TexCoord).r * 255.0;
    int colorIndex = int(theByte) & 0x0F; // 0x0F is 00001111, so we get the 4 lower bytes only
    vec4 color = colorPalette[colorIndex];
    if(color.rgb == vec3(0,0,0)) {
        discard;
    }
    FragColor = color;

}