
#version 330 core
layout (location=0) in vec3 aPos;//λ�ñ���������λ��ֵΪ0
layout (location=1) in vec3 aColor;//��ɫ����������λ��ֵΪ1 
out vec3 ourColor;
uniform float xOffset;
void main(){
//gl_Position=vec4(aPos,1.0);
//���µߵ�
gl_Position=vec4(aPos.x+xOffset,-aPos.y,aPos.z,1.0);
//ourColor=aColor;��ɫ��������
//��ɫ�붥�����
ourColor=aPos;
}