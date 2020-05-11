#version 330

in vec4 pos;

out vec3 fragColor;

void main()
{
  fragColor = vec3(pos.y * 2, gl_FragCoord.z, 0);
  if (distance(fract(pos.x), 0) < 0.1)
    fragColor = vec3(0, 0, 1);
}