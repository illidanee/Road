varying vec2 V_Texcoord;

void main()
{
	float distance = length(V_Texcoord - vec2(0.5)) * 10.0;
	float alpha = exp(-distance * distance);
	if (alpha < 0.01) discard;
	gl_FragColor = vec4(0.2, 0.5, 0.3, alpha);
}