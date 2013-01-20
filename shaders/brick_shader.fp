
uniform vec3 brickColour, mortarColour;
uniform vec3 brickSize;
uniform vec2 brickPct;

in vec2 mcPosition;
in float lightIntensity;

out vec4 fragColour;

void main()
{
    vec3 colour;
    vec2 position, useBrick;

    position = mcPosition / brickSize;

    if (fract(position.y * 0.5) > 0.5)
        poition.x += 0.5;

    position = fract(position);

    useBrick = step(position, brickPct);

    colour = mix(mortarColour, brickColour, useBrick.x * useBrick.y);
    colour *= lightIntensity;
    fragColour = vec4(colour, 1.0);
}
