#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

uniform vec3 sunDir; 
uniform vec3 viewPos; // NEEDED: The position of the camera in world space

void main()
{
	// Setup common variables
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 objectColor = Color;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-sunDir); 
	
	// 1. Ambient Lighting
	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * lightColor;
	
	// 2. Diffuse Lighting
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	// 3. Specular Lighting (Phong Model)
	float specularStrength = 0.5; // How intense the shiny highlight is
	
	// Calculate the reflection vector (reflect expects vector pointing FROM light to surface)
	vec3 reflectDir = reflect(-lightDir, norm);  
	
	// Calculate the direction from the fragment to the camera
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// Calculate the specular component using a shininess exponent 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColor;  
	
	// 4. Combine Phong components
	vec3 result = (ambient + diffuse + specular) * objectColor;
	
	// FIX: Clamp the final RGB values between 0.0 and 1.0
	vec3 clampedResult = min(result, vec3(1.0));
	
	FragColor = vec4(clampedResult, 1.0);
}
