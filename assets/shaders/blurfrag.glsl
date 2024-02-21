#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
uniform vec4 colorPalette[16];
uniform float blurries[16];

vec4 tfc(vec2 tc) {
    float theByte = texture(ourTexture, tc).r * 255.0;
    int colorIndex = int(theByte) & 0x0F; // 0x0F is 00001111, so we get the 4 lower bytes only
    vec4 color = colorPalette[colorIndex];

    if(tc.x < 0.01f && tc.y < 0.01f) {
        return vec4(0,0,0,0);
    }

    if(blurries[colorIndex] == 1) {
        return color;
    } else {
        return vec4(0,0,0,0);
    }

}

void main() {


    
    vec3 iResolution = vec3(400, 300, 0);

    float Pi = 6.28318530718; // Pi*2
    
    // GAUSSIAN BLUR SETTINGS {{{
    float Directions = 24.0; // BLUR DIRECTIONS (Default 16.0 - More is better but slower)
    float Quality = 4.0; // BLUR QUALITY (Default 4.0 - More is better but slower)
    float Size = 6.0; // BLUR SIZE (Radius)
    // GAUSSIAN BLUR SETTINGS }}}
   
    vec2 Radius = Size/iResolution.xy;
    

    // Pixel colour
    vec4 Color = tfc(TexCoord);
    
    // Blur calculations
    for( float d=0.0; d<Pi; d+=Pi/Directions)
    {
		for(float i=1.0/Quality; i<=1.0; i+=1.0/Quality)
        {
			Color += tfc( TexCoord+vec2(cos(d),sin(d))*Radius*i);		
        }
    }
    
    // Output to screen
    Color /= Quality * Directions - 15.0;

    if(Color.rgb == vec3(0,0,0)) {
        discard;
    }

    Color.a = (Color.r + Color.g + Color.b) / 1.0;
    Color = Color*20;
    FragColor =  vec4(Color.rgb, 0.5);

}