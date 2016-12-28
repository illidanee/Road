uniform sampler2D U_MainTexture;

void main()
{
	gl_FragColor = vec4(0.7, 0.5, 0.5, texture2D(U_MainTexture, gl_PointCoord.xy).a);
} 