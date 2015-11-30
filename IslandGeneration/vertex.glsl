#version 330

in vec3 inPoint;
in vec3 inColor;

out vec3 color;

void main()
{
	color = inColor;
	gl_Position = vec4(inPoint, 1.0);
}
