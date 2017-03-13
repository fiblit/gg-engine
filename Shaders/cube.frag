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
	float fadeOff;	//spot

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
	/* TODO */
	/* organize to:
		do geometric calculations
		do lighting calculations
	*/
	// rename variables to be more coherent
	// restructure to be more coherent

	// Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// Diffuse
	vec3 norm = normalize(Normal);
	float intensity, attenuation;
	vec3 lightDir;
	switch(light.mode) {
		case 0: // directional
			{
			//lightPos = mat3(view) * light.pos
			lightDir = normalize(-light.dir);//-lightPos);
			intensity = 1.0f;
			attenuation = 1.0f;
			}
			break;
		case 1: //point
			{
			vec3 lightPos = vec3(view * vec4(light.pos, 1.0f));
			lightDir = (lightPos - FragPos);
			float distance = length(lightDir);
			lightDir = normalize(lightDir);
			attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
			intensity = attenuation;
			}
			break;
		case 2: //spotlight
			{
			vec3 lightPos = vec3(view * vec4(light.pos, 1.0f));
			vec3 spotDir = mat3(view) * light.dir;
			lightDir = (lightPos - FragPos);
			float distance = length(lightDir);
			lightDir = normalize(lightDir);
			attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));			
			float theta = dot(lightDir, normalize(-spotDir));
			intensity = attenuation * clamp((theta - light.fadeOff) / (light.cutOff - light.fadeOff), 0.0f, 1.0f);
			}
			break;
	}
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// Specular
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // reflect by a's head at b's tail
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	vec3 result = ambient + (diffuse + specular) * intensity;
	color = vec4(result, 1.0f);
}
