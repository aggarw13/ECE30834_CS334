#version 330

in vec3 inPoint;
in vec3 inColor;

out vec3 outColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
	outColor = inColor;
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPoint, 1.0);
}
