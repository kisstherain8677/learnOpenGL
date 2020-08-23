#version 330 core
out vec4 FragColor;

in vec2 TexCoords;//vs�������������
in vec3 Normal;  //vs����ķ�������
in vec3 FragPos;//vs�����Ƭ����������

uniform sampler2D texture_diffuse1;
uniform float shininess;

//���Դ�ṹ��
struct PointLight{
	vec3 position;

	//˥��ϵ��
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform PointLight pointLight;//��Դ��Ϣ
uniform vec3 viewPos;//�۲���λ��


//������Դ
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
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // ˥��
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // �ϲ����
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_diffuse1, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
	
}