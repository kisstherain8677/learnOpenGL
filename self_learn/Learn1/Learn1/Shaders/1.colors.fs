#version 330 core

struct Material{//�������䷽ʽ�ķ�����ɫ���Ͳ������
	sampler2D diffuse;//��ά������
	sampler2D specular;
	float shininess;
};

uniform Material material;

struct Light{//�������䷽ʽ��ǿ�ȣ��͹�Դ��ء�����ֵ��ͬ����Ϊֻ�Ǳ�ʾǿ��
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  

  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    // ambient
    
    vec3 ambient =light.ambient * vec3(texture(material.diffuse,TexCoords));
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));

	//specular
	//float specularStrength=0.5;//����ǿ��
	float shineness=32;//����� Խ����ԽС
	vec3 viewDir=normalize(viewPos-FragPos);//���߷���
	vec3 reflectDir=reflect(-lightDir,norm);//ע��reflectҪ���һ���Ǵӹ�Դָ��Ƭ�ε�����
	float spec=pow(max(dot(viewDir,reflectDir),0.0),shineness);
	vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoords));
            
    vec3 result = ambient + diffuse+specular;
    FragColor = vec4(result, 1.0);
} 