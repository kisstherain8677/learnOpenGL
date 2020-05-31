
#version 330 core
layout (location=0) in vec3 aPos;//位置变量的属性位置值为0
layout (location=1) in vec3 aColor;//颜色变量的属性位置值为1 
out vec3 ourColor;
uniform float xOffset;
void main(){
//gl_Position=vec4(aPos,1.0);
//上下颠倒
gl_Position=vec4(aPos.x+xOffset,-aPos.y,aPos.z,1.0);
//ourColor=aColor;颜色根据输入
//颜色与顶点相等
ourColor=aPos;
}