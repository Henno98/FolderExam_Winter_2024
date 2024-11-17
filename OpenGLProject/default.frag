#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the normal from the Vertex Shader
in vec3 Normal;


// Gets the color of the light from the main function
uniform vec3 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

void main()
{
	// ambient lighting
	float ambient = 0.2f;
	vec3 Ambient = ambient*lightColor;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	vec3 Diffuse = diffuse*lightColor;

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 32);
	vec3 specular = specAmount * specularLight * lightColor;

	// outputs final color
	vec3 result =  (Ambient+Diffuse+specular)*color;
	FragColor = vec4(result,1.0f);
}