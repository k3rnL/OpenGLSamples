#version 330

in vec4 f_point;
in vec3 f_normal;

out vec3 fragColor;

void main()
{
  fragColor = vec3(f_point.y * 2, gl_FragCoord.z, 0);
  if (distance(fract(f_point.x), 0) < 0.1)
    fragColor = vec3(0, 0, 1);

  vec3 light = vec3(-10, 10, -10);

  vec3 L = normalize(light - f_point.xyz);
  float angle = max(0, dot(L, normalize(f_normal)));

//  fragColor = vec3(1);
  fragColor = vec3(0.2) + fragColor * angle;

}