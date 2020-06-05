#version 330

uniform mat4 MVP;
uniform vec3 point_on_plane;

in vec3 point;
out vec3 pos;

void main()
{
  gl_Position = vec4(point.xzy, 1);
  pos = point;
}