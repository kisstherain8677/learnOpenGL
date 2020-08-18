#version 330 core

struct Material{//各个反射方式的反射颜色，和材料相关
	sampler2D diffuse;//二维采样器
	sampler2D specular;
	float shininess;
};

uniform Material material;

//定向光结构体
struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

uniform DirLight dirLight;
//点光源结构体
struct PointLight{
	vec3 position;

	//衰减系数
	float constant;
	float linear;
	float quadratic;

	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
//用预处理指令定义场景中点光源的数量
#define NR_POINT_LIGHT 4
uniform PointLight pointLights[NR_POINT_LIGHT];


struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform SpotLight spotLight;

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  


uniform vec3 viewPos;

//计算定向光
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
//计算点光源
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
//计算聚光
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

  vec3 norm=normalize(Normal);
  vec3 viewDir=normalize(viewPos-FragPos);

  //计算定向光照
  vec3 result=CalcDirLight(dirLight,norm,viewDir);
  //计算点光照
  for(int i=0;i<NR_POINT_LIGHT;i++){
		result+=CalcPointLight(pointLights[i],norm,FragPos,viewDir);
  }
  //计算聚光
  result += CalcSpotLight(spotLight, norm, FragPos, viewDir); 

  FragColor=vec4(result,1.0);
} 

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir){
	vec3 lightDir=normalize(-light.direction);
	//漫反射着色
	float diff=max(dot(lightDir,normal),0.0);
	//镜面光着色
	vec3 reflectDir=reflect(-lightDir,normal);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	//合并结果
	vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));
	vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoords));

	return (ambient+diffuse+specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
	
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}