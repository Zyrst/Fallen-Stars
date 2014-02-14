uniform float highlightStrength;
uniform sampler2D texture;

void main(void)
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color
    gl_FragColor = vec4(highlightStrength, highlightStrength, highlightStrength, 1.0) * pixel;
}
