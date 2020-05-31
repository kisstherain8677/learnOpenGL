#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Shader.h"
#include<iostream>


//顶点属性
float vertices[] = {
	//位置            //颜色
	-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,//右下
	 0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,//左下
	 0.0f,0.5f,0.0f,0.0f,0.0f,1.0f//顶点
};



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
	glViewport(0, 0, 800, 600);
	//viewport随着窗口改变的回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//创建着色器对象
	Shader ourShader("Shaders/shader.vs", "Shaders/shader.fs");

	unsigned int VBO;
	//生成缓冲对象
	glGenBuffers(1, &VBO);
	
	unsigned int VAO;//VBO数据和顶点属性配置
	glGenVertexArrays(1, &VAO);
	//绑定VAO
	glBindVertexArray(VAO);
	//把顶点数组复制到缓冲中供opengl使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//解释顶点数据以应用到顶点着色器的逐个顶点属性（设置顶点属性指针）
	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	while (!glfwWindowShouldClose(window)) {

		//处理输入
		processInput(window);

		//渲染指令
		//清空颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空缓冲的颜色（状态设置函数）
		glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲（状态使用函数。OpenGL是一个大的状态机）

		ourShader.use();
		ourShader.setFloat("xOffset", 0.5f);
		
		//绘制三角形
		glBindVertexArray(VAO);//使用绑定的VAO（里面有顶点属性信息）
		glDrawArrays(GL_TRIANGLES, 0, 3);//绘制三角形命令

		//检查并调用事件，交换缓冲。
		glfwSwapBuffers(window);//交换颜色缓冲。是一个储存每个像素颜色值的大缓冲
		glfwPollEvents();//检查有无触发事件
	}

	//渲染结束后释放所有的glfw资源
	glfwTerminate();
	return 0;
}


