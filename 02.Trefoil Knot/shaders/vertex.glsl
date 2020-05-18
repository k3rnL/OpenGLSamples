#version 330

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;
uniform vec3 point_on_plane;

in vec3 point;
in vec3 normal;

out vec4 f_point;
out vec3 f_normal;

void main()
{
  gl_Position = Projection * View * Model * vec4(point.xyz, 1);
  f_point = vec4(point.xyz, 1);
  f_normal = normal;
}