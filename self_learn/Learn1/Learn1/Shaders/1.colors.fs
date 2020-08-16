#version 330 core

struct Material{//各个反射方式的反射颜色，和材料相关
	sampler2D diffuse;//二维采样器
	sampler2D specular;
	float shininess;
};

uniform Material material;

struct Light{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//衰减三系数
	float constant;//常数
	float linear;
	float quadratic;

};
uniform Light light;

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  


uniform vec3 viewPos;

void main()
{

   
    // ambient
    
    vec3 ambient =light.ambient * vec3(texture(material.diffuse,TexCoords));
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));

	//specular
	//float specularStrength=0.5;//镜面强度

	vec3 viewDir=normalize(viewPos-FragPos);//视线方向
	vec3 reflectDir=reflect(-lightDir,norm);//注意reflect要求第一个是从光源指向片段的向量
	float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoords));
             
    //spotlight
	float theta=dot(lightDir,normalize(-light.direction));
	float epsilon=light.cutOff-light.outerCutOff;
	float intensity=clamp((theta-light.outerCutOff)/epsilon,0.0,1.0);
	diffuse*=intensity;
	specular*=intensity;

    //attenuation
	float distance=length(light.position-FragPos);
	float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
	
    vec3 result = ambient + diffuse+specular;
    FragColor = vec4(result, 1.0);
} 