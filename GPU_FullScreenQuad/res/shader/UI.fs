uniform sampler2D U_MainTexture;

varying vec2 V_Texture;

void main()
{
	gl_FragColor = texture2D(U_MainTexture, V_Texture);
}