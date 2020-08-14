#version 330 core

struct Material 
{
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light 
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform Material u_Material;
uniform Light u_Light;
uniform vec3 u_CameraPos;

void main()
{	
	//ambient light
    vec3 ambient = u_Light.ambient * u_Material.ambient;

	//diffuse light
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_Light.direction); 
    float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);
	
	//specular
	vec3 viewDir = normalize(u_CameraPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * (spec * u_Material.specular);  
	
	vec3 result = (ambient + diffuse + specular);
	color = vec4(result, 1.0) * texture(u_Texture, v_TexCoord);
}
 