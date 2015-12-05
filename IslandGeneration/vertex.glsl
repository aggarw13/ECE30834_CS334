#version 130

in vec3 inPoint;
in vec3 inColor;

out vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
	color = inColor;
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPoint, 1.0);
}
