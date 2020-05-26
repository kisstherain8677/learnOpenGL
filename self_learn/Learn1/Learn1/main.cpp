#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>


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

	while (!glfwWindowShouldClose(window)) {

		//处理输入
		processInput(window);

		//渲染指令
		//清空颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空缓冲的颜色（状态设置函数）
		glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲（状态使用函数。OpenGL是一个大的状态机）


		//检查并调用事件，交换缓冲。
		glfwSwapBuffers(window);//交换颜色缓冲。是一个储存每个像素颜色值的大缓冲
		glfwPollEvents();//检查有无触发事件
	}

	//渲染结束后释放所有的glfw资源
	glfwTerminate();
	return 0;
}


