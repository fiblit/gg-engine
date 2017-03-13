#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shine; 
};
struct Light {
	vec4 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//attentuation
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;

uniform mat4 view;
uniform Light light;

out vec4 color;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

void main() {
	float epsilon = 0.00001;

	// Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightPos, lightDir;
	float attenuation;
	if (abs(light.pos.w) < 0.0 + epsilon) {
		lightPos = mat3(view) * light.pos.xyz;
		lightDir = normalize(-lightPos);
		attenuation = 1.0f;
	}
	else {
		lightPos = vec3(view * light.pos);
		lightDir = (lightPos - FragPos);
		float distance = length(lightDir);
		lightDir = normalize(lightDir);
		attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	}

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// Specular
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // reflect by a's head at b's tail
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	vec3 result = (ambient + diffuse + specular) * attenuation;
	color = vec4(result, 1.0f);
}
