#version 330 core

struct Material{//�������䷽ʽ�ķ�����ɫ���Ͳ������
	sampler2D diffuse;//��ά������
	sampler2D specular;
	float shininess;
};

uniform Material material;

struct Light{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//˥����ϵ��
	float constant;//����
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

    //attenuation
	float distance=length(light.position-FragPos);
	float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));

    // ambient
    
    vec3 ambient =light.ambient * vec3(texture(material.diffuse,TexCoords));
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));

	//specular
	//float specularStrength=0.5;//����ǿ��
	float shineness=32;//����� Խ����ԽС
	vec3 viewDir=normalize(viewPos-FragPos);//���߷���
	vec3 reflectDir=reflect(-lightDir,norm);//ע��reflectҪ���һ���Ǵӹ�Դָ��Ƭ�ε�����
	float spec=pow(max(dot(viewDir,reflectDir),0.0),shineness);
	vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoords));
            
    vec3 result = ambient*attenuation + diffuse*attenuation+specular*attenuation;
    FragColor = vec4(result, 1.0);
} 