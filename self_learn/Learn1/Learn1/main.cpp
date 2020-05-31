#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Shader.h"
#include<iostream>


//��������
float vertices[] = {
	//λ��            //��ɫ
	-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,//����
	 0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,//����
	 0.0f,0.5f,0.0f,0.0f,0.0f,1.0f//����
};



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//��������,����Ⱦѭ���е���.�����������������Ⱦ��ʱ��
//�����û�������и��ִ���
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}



int main() {
	glfwInit();//��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//opengl�����汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//opengl�Ĵΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ʹ��opengl�ĺ���ģʽ

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Fail to create GLFW window" << std::endl;
		glfwTerminate();//��������glfw���ָ��
		return -1;
	}
	glfwMakeContextCurrent(window);//����������������Ϊ��ǰ�̵߳���������

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//����viewport
	//viewport�Ǹ���opengl�ģ��������������2άӳ�䴦��
	glViewport(0, 0, 800, 600);
	//viewport���Ŵ��ڸı�Ļص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//������ɫ������
	Shader ourShader("Shaders/shader.vs", "Shaders/shader.fs");

	unsigned int VBO;
	//���ɻ������
	glGenBuffers(1, &VBO);
	
	unsigned int VAO;//VBO���ݺͶ�����������
	glGenVertexArrays(1, &VAO);
	//��VAO
	glBindVertexArray(VAO);
	//�Ѷ������鸴�Ƶ������й�openglʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���Ͷ���������Ӧ�õ�������ɫ��������������ԣ����ö�������ָ�룩
	//λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	while (!glfwWindowShouldClose(window)) {

		//��������
		processInput(window);

		//��Ⱦָ��
		//�����ɫ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//������ջ������ɫ��״̬���ú�����
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ���壨״̬ʹ�ú�����OpenGL��һ�����״̬����

		ourShader.use();
		ourShader.setFloat("xOffset", 0.5f);
		
		//����������
		glBindVertexArray(VAO);//ʹ�ð󶨵�VAO�������ж���������Ϣ��
		glDrawArrays(GL_TRIANGLES, 0, 3);//��������������

		//��鲢�����¼����������塣
		glfwSwapBuffers(window);//������ɫ���塣��һ������ÿ��������ɫֵ�Ĵ󻺳�
		glfwPollEvents();//������޴����¼�
	}

	//��Ⱦ�������ͷ����е�glfw��Դ
	glfwTerminate();
	return 0;
}


