#version 330 core

struct Material{//各个反射方式的反射颜色，和材料相关
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light{//各个反射方式的强度，和光源相关。三个值相同，因为只是表示强度
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    // ambient
    
    vec3 ambient =light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse*(diff*material.diffuse);

	//specular
	//float specularStrength=0.5;//镜面强度
	float shineness=32;//反光度 越大光点越小
	vec3 viewDir=normalize(viewPos-FragPos);//视线方向
	vec3 reflectDir=reflect(-lightDir,norm);//注意reflect要求第一个是从光源指向片段的向量
	float spec=pow(max(dot(viewDir,reflectDir),0.0),shineness);
	vec3 specular=light.specular*(spec*material.specular);
            
    vec3 result = ambient + diffuse+specular;
    FragColor = vec4(result, 1.0);
} 