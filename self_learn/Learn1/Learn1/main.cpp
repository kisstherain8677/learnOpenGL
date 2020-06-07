#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Shader.h"
#include"stb_image.h"
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>





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

void transform_operate(int shaderid,float time) {

	glm::mat4 trans = glm::mat4(1.0f);
	
	trans = glm::rotate(trans, time, glm::vec3(0.0, 0.0, 1.0));
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	unsigned int transloc = glGetUniformLocation(shaderid, "transform");
	glUniformMatrix4fv(transloc, 1, GL_FALSE, glm::value_ptr(trans));
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
	//glViewport(0, 0, 800, 600);
	//viewport���Ŵ��ڸı�Ļص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//������ɫ������
	Shader ourShader("Shaders/shader.vs", "Shaders/shader.fs");

	//��������
	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};

	//�����б�
	unsigned int indices[] = {
		0,1,3,//��һ��������
		1,2,3//�ڶ���������
	};


	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//��VAO
	glBindVertexArray(VAO);
	//�Ѷ������鸴�Ƶ������й�openglʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//�����������鵽һ������������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���Ͷ���������Ӧ�õ�������ɫ��������������ԣ����ö�������ָ�룩
	//λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//������������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//��������
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//���ز���������
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("pic/container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//�ڶ�������
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//���ز���������
	data = stbi_load("pic/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ourShader.use();//set uniform֮ǰҪ
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);
	//ourShader.setInt("texture2", 1);
	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {

		//��������
		processInput(window);

		//��Ⱦָ��
		//�����ɫ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//������ջ������ɫ��״̬���ú�����
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ���壨״̬ʹ�ú�����OpenGL��һ�����״̬����

		//������Ԫ
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);//��󶨵����������Ԫ
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);


		ourShader.use();
		float timeValue = glfwGetTime();
		float mixa = (sin(timeValue*2.0)+1)*0.5f;
		ourShader.setFloat("mixa", mixa);
		//�ƶ�
		transform_operate(ourShader.ID,timeValue);
		//����
		glBindVertexArray(VAO);//ʹ�ð󶨵�VAO�������ж���������Ϣ��
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//�Ǵ�������������Ⱦ ��ͬ��glDrawArray

		//��鲢�����¼����������塣
		glfwSwapBuffers(window);//������ɫ���塣��һ������ÿ��������ɫֵ�Ĵ󻺳�
		glfwPollEvents();//������޴����¼�
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//��Ⱦ�������ͷ����е�glfw��Դ
	glfwTerminate();
	return 0;
}


