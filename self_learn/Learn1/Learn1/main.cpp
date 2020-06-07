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

//处理输入,在渲染循环中调用.用这个方法可以在渲染的时候
//根据用户输入进行各种处理
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

	
	glfwInit();//初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//opengl的主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//opengl的次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用opengl的核心模式



	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Fail to create GLFW window" << std::endl;
		glfwTerminate();//回收所有glfw相关指针
		return -1;
	}
	glfwMakeContextCurrent(window);//讲窗口上下文设置为当前线程的主上下文

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//设置viewport
	//viewport是告诉opengl的，最后根据这个进行2维映射处理
	//glViewport(0, 0, 800, 600);
	//viewport随着窗口改变的回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//创建着色器对象
	Shader ourShader("Shaders/shader.vs", "Shaders/shader.fs");

	//顶点属性
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	//索引列表
	unsigned int indices[] = {
		0,1,3,//第一个三角形
		1,2,3//第二个三角形
	};


	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//绑定VAO
	glBindVertexArray(VAO);
	//把顶点数组复制到缓冲中供opengl使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//复制索引数组到一个索引缓冲中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//解释顶点数据以应用到顶点着色器的逐个顶点属性（设置顶点属性指针）
	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//纹理坐标属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//生成纹理
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载并生成纹理
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
	//第二张纹理
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载并生成纹理
	data = stbi_load("pic/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ourShader.use();//set uniform之前要
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);
	//ourShader.setInt("texture2", 1);
	//渲染循环
	while (!glfwWindowShouldClose(window)) {

		//处理输入
		processInput(window);

		//渲染指令
		//清空颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空缓冲的颜色（状态设置函数）
		glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲（状态使用函数。OpenGL是一个大的状态机）

		//绑定纹理单元
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);//会绑定到激活的纹理单元
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);


		ourShader.use();
		float timeValue = glfwGetTime();
		float mixa = (sin(timeValue*2.0)+1)*0.5f;
		ourShader.setFloat("mixa", mixa);
		//移动
		transform_operate(ourShader.ID,timeValue);
		//绘制
		glBindVertexArray(VAO);//使用绑定的VAO（里面有顶点属性信息）
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//是从索引缓冲中渲染 不同于glDrawArray

		//检查并调用事件，交换缓冲。
		glfwSwapBuffers(window);//交换颜色缓冲。是一个储存每个像素颜色值的大缓冲
		glfwPollEvents();//检查有无触发事件
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//渲染结束后释放所有的glfw资源
	glfwTerminate();
	return 0;
}


