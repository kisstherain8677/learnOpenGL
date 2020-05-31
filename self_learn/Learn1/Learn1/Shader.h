#pragma once
#include<glad/glad.h>//����glad����ȡ���б����OpenGLͷ�ļ�
#include <string>
#include<fstream>
#include<sstream>
#include<iostream>
class Shader
{
public:
	//����ID
	unsigned int ID;
	//��������ȡ��������ɫ��
	Shader(const GLchar* vertexPath,const GLchar* fragmentPath);
	//ʹ�ã��������
	void use();
	//uniform���ߺ���(��ȡһ��uniform��λ�ò�������ֵ)
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value)const;
	void setFloat(const std::string &name, float value)const;
	~Shader();
};

