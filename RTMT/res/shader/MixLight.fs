#version 400 core

uniform sampler2D U_MainTexture;

varying vec3 V_Normal;
varying vec4 V_WorldPos;
varying vec2 V_Texcoord;

subroutine vec4 SurfaceColor();

subroutine uniform SurfaceColor U_SurfaceColor;

subroutine (SurfaceColor) vec4 Ambient()
{
	//ambient
	vec4 ambientLight = vec4(0.6,0.6,0.6, 1.0);
	vec4 ambientMeterial = vec4(0.2,0.2,0.2, 1.0);
	vec4 ambientColor = ambientLight * ambientMeterial;

	return texture2D(U_MainTexture, V_Texcoord) * ambientColor;
}

subroutine (SurfaceColor) vec4 Diffuse()
{
	vec3 lightPos = vec3(10.0, 10.0, 0.0);
	vec3 L = lightPos;
	L = normalize(L);
	vec3 N = normalize(V_Normal);

	//ambient
	vec4 ambientLight = vec4(0.2,0.2,0.2, 1.0);
	vec4 ambientMeterial = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 ambientColor = ambientLight * ambientMeterial;

	//diffuse
	vec4 diffuseLight = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 diffuseMeterial = vec4(0.6, 0.6, 0.6, 1.0);
	vec4 diffuseColor = diffuseLight * diffuseMeterial * max(0.0, dot(L, N));

	return ambientColor + texture2D(U_MainTexture, V_Texcoord) * diffuseColor;
}

subroutine (SurfaceColor) vec4 Specular()
{
	vec3 lightPos = vec3(10.0, 10.0, 0.0);
	vec3 L = lightPos;
	L = normalize(L);
	vec3 N = normalize(V_Normal);

	//ambient
	vec4 ambientLight = vec4(0.2,0.2,0.2, 1.0);
	vec4 ambientMeterial = vec4(0.2, 0.2, 0.2, 1.0);
	vec4 ambientColor = ambientLight * ambientMeterial;

	//diffuse
	vec4 diffuseLight = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 diffuseMeterial = vec4(0.6, 0.6, 0.6, 1.0);
	vec4 diffuseColor = diffuseLight * diffuseMeterial * max(0.0, dot(L, N));

	//Specular
	vec3 reflectDir = normalize(reflect(-L, N));
	vec3 viewDir = normalize(vec3(0.0) - V_WorldPos.xyz);
	vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 specularMeterial = vec4(0.8, 0.8, 0.8, 1.0);
	vec4 specularColor = specularLight * specularMeterial * pow(max(0.0, dot(reflectDir, viewDir)), 128);

	if (diffuseColor.r == 0.0)
	{
		 diffuseColor = vec4(0.0);
	}

	return ambientColor + texture2D(U_MainTexture, V_Texcoord) * diffuseColor + specularColor;
}

void main()
{
	gl_FragData[0] = U_SurfaceColor();
	gl_FragData[1] = vec4(V_WorldPos.xyz, 1.0);
}