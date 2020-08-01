#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Shader.h"
#include"stb_image.h"
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"Camera.h"


//���������
glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = 0.0f, pitch = 0.0f;//ƫ���Ǻ͸�����

float deltaTime = 0.0f;//��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f;//��һ֡��ʱ��
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//setting
const float screenWidth = 600.0f;
const float screenHeight = 800.0f;

//��Դλ��
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//���Ļص�����
bool firstMouse = true;
float lastX = screenWidth/2, lastY = screenHeight/2;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	
	camera.ProcessMouseMovement(xoffset, yoffset);
	
}

//��������
float fov = 45.0f;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}
//��������,����Ⱦѭ���е���.�����������������Ⱦ��ʱ��
//�����û�������и��ִ���
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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
	GLFWwindow* window = glfwCreateWindow(screenHeight, screenWidth, "LearnOpenGL", NULL, NULL);
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//������ɫ������
	Shader lightingShader("Shaders/shader.vs", "Shaders/shader.fs");
	Shader lightCubeShader("Shaders/shader.vs", "Shaders/lightFrag.fs");

	//��������
	float  vertices[] = {
	   -0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
	   -0.5f,  0.5f, -0.5f,
	   -0.5f, -0.5f, -0.5f,

	   -0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f,  0.5f,
	   -0.5f, -0.5f,  0.5f,

	   -0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f, -0.5f,
	   -0.5f, -0.5f, -0.5f,
	   -0.5f, -0.5f, -0.5f,
	   -0.5f, -0.5f,  0.5f,
	   -0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

	   -0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
	   -0.5f, -0.5f,  0.5f,
	   -0.5f, -0.5f, -0.5f,

	   -0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f, -0.5f,
	};
	

	//��������cube��VAO
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//Ȼ�����ù�Դ��VAO
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//����deltatime
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		//��������
		processInput(window);

		//��Ⱦָ��
		//�����ɫ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//������ջ������ɫ��״̬���ú�����
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ���壨״̬ʹ�ú�����OpenGL��һ�����״̬����
		//������Ȳ���
		glEnable(GL_DEPTH_TEST);
		//�����Ȼ���
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//����shader
		lightingShader.use();
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		//����view
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);
		//��Ⱦ
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//��Ⱦ��
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));//��Сһ��
		lightCubeShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		//��鲢�����¼����������塣
		glfwSwapBuffers(window);//������ɫ���塣��һ������ÿ��������ɫֵ�Ĵ󻺳�
		glfwPollEvents();//������޴����¼�
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	//��Ⱦ�������ͷ����е�glfw��Դ
	glfwTerminate();
	return 0;
}


