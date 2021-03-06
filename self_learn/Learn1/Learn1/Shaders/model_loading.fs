#version 330 core
out vec4 FragColor;

in vec2 TexCoords;//vs传入的纹理坐标
in vec3 Normal;  //vs传入的法线向量
in vec3 FragPos;//vs传入的片段世界坐标

uniform sampler2D texture_diffuse1;
uniform float shininess;

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

uniform PointLight pointLight;//光源信息
uniform vec3 viewPos;//观察者位置


//计算点光源
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);

void main()
{    

	vec3 normal=normalize(Normal);
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 result=CalcPointLight(pointLight,normal,FragPos,viewDir);
    FragColor = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_diffuse1, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
	
}