#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices=256) out;

uniform int sub_divisions;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 point_on_plane;

out vec4 pos;

vec4 move(vec4 point) {
    float d = distance(point.xyz, point_on_plane);
    if (d < 5)
        d = (5 - d) / 4;
    else
        d = 0;
    return point + vec4(0, d, 0, 0);
}

void main()
{
    vec4 v0 = Model * gl_in[0].gl_Position;
    vec4 v1 = Model * gl_in[1].gl_Position;
    vec4 v2 = Model * gl_in[2].gl_Position;
    float dx = abs(v0.x-v1.x)/sub_divisions;
    float dz = abs(v0.z-v2.z)/sub_divisions;
    float x=v0.x;
    float z=v0.z;
    for (int j=0;j<sub_divisions*sub_divisions;j++) {
        gl_Position =  Projection * View * move(vec4(x, 0, z, 1));
        pos = move(vec4(x, 0, z, 1));
        EmitVertex();
        gl_Position =  Projection * View * move(vec4(x, 0, z+dz, 1));
        pos = move(vec4(x, 0, z+dz, 1));
        EmitVertex();
        gl_Position =  Projection * View * move(vec4(x+dx, 0, z, 1));
        pos = move(vec4(x+dx, 0, z, 1));
        EmitVertex();
        gl_Position =  Projection * View * move(vec4(x+dx, 0, z+dz, 1));
        pos = move(vec4(x+dx, 0, z+dz, 1));
        EmitVertex();
        EndPrimitive();
        x+=dx;
        if ((j+1) %sub_divisions == 0) {
            x=v0.x;
            z+=dz;
        }
    }
}