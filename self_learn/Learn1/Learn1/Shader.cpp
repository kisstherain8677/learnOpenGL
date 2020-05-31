#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	//1.从文件路径中获取shader
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//保证ifstream对象可以抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		//转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();//转换为字符串数组
	const char* fShaderCode = fragmentCode.c_str();

	//2.编译着色器
	unsigned int vertex, fragment;
	int succes;
	char infoLog[512];

	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//打印编译错误
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
	if (!succes) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED\ " << infoLog << std::endl;
	}
	//片段着色器:
	//顶点着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//打印编译错误
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
	if (!succes) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILE_FAILED\ " << infoLog << std::endl;
	}
	//着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//打印链接错误
	glGetProgramiv(ID, GL_LINK_STATUS, &succes);
	if (!succes) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//删除着色器
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


void Shader:: use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()),value);
}
void Shader::setFloat(const std::string &name, float value)const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()),value);
}



Shader::~Shader()
{
}
