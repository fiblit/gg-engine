#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shine; 
};
struct Light {
	int mode;

	vec3 pos;		//point / spot
	vec3 dir;		//spot / dir
	float cutOff;	//spot

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
	// Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightPos, lightDir;
	float attenuation, theta, distance;
	switch(light.mode) {
		case 0: // directional
			//lightPos = mat3(view) * light.pos
			lightDir = normalize(-light.dir);//-lightPos);
			attenuation = 1.0f;
			theta = light.cutOff + 1.0f;
			break;
		case 1: //point
			lightPos = vec3(view * vec4(light.pos, 1.0f));
			lightDir = (lightPos - FragPos);
			distance = length(lightDir);
			lightDir = normalize(lightDir);
			attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
			theta = light.cutOff + 1.0f;
			break;
		case 2: //spotlight
			lightPos = vec3(view * vec4(light.pos, 1.0f));
			vec3 spotDir = mat3(view) * light.dir;
			lightDir = (lightPos - FragPos);
			distance = length(lightDir);
			lightDir = normalize(lightDir);
			attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));			
			theta = dot(lightDir, normalize(-spotDir));
			break;
	}
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// Specular
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // reflect by a's head at b's tail
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	if (theta > light.cutOff) {
		vec3 result = (ambient + diffuse + specular) * attenuation;
		color = vec4(result, 1.0f);
	}
	else
		color = vec4(ambient * attenuation, 1.0f);
}
