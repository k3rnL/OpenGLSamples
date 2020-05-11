#version 330

uniform mat4 MVP;
uniform vec3 point_on_plane;

in vec3 point;

void main()
{
  gl_Position = vec4(point.xyz, 1);
}