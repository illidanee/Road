
varying vec3 V_Normal;

void main()
{
	//ambient
	vec4 ambientLight = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 ambientMeterial = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 ambientColor = ambientLight * ambientMeterial;

	//diffuse
	vec3 lightPos = vec3(0.0, 10.0, 0.0);
	vec3 L = lightPos;
	L = normalize(L);
	vec3 N = normalize(V_Normal);
	vec4 diffuseLight = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 diffuseMeterial = vec4(0.6, 0.6, 0.6, 1.0);
	vec4 diffuseColor = diffuseLight * diffuseMeterial * max(0.0, dot(L, N));


    gl_FragColor=ambientColor + diffuseColor;
}