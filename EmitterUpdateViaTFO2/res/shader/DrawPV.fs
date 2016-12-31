uniform sampler2D U_MainTexture;

varying vec2 GS_Texcoord;
varying vec4 GS_Color;

void main()
{
	gl_FragColor = vec4(0.7, 0.5, 0.5, texture2D(U_MainTexture, GS_Texcoord).a * GS_Color.z);
	//gl_FragColor = vec4(1.0);
} 