
void main()
{
	vec4 ambientLight = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 ambientMeterial = vec4(0.5, 0.5, 0.5, 1.0);
	vec4 ambientColor = ambientLight * ambientMeterial;
    gl_FragColor=ambientColor;
}