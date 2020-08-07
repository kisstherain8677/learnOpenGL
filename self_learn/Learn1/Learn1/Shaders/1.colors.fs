#version 330 core
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
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	//specular
	float specularStrength=0.5;//镜面强度
	vec3 viewDir=normalize(viewPos-FragPos);//视线方向
	vec3 reflectDir=reflect(-lightDir,norm);//注意reflect要求第一个是从光源指向片段的向量
	float spec=pow(max(dot(viewDir,reflectDir),0.0),32);
	vec3 specular=specularStrength*spec*lightColor;
            
    vec3 result = (ambient + diffuse+specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 