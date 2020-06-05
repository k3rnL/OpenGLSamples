#version 330

#define MAX_DEPTH 1000
#define EPSILON 0.0001f

uniform mat4 Rotation;
uniform mat4 View;
uniform float Time;
uniform sampler2D Tex;
uniform vec3 Resolution;

in vec3 pos;

out vec3 fragColor;

float sphereSDF(vec3 p, float r) {
    return length(p) - r;
}

float sdBox(vec3 p, vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float planxSDF(vec3 p) {
    return p.z;
}

float sceneSDF(vec3 p) {
    // sphere
    float dist = sphereSDF(p+vec3(0, -3, 0), 1.5);
    float displacement = smoothstep(-3.14*2, 3.14*2, sin(p.y*10+Time*3.f)) / 4;
    displacement += smoothstep(-3.14*2, 3.14*2, cos(p.x*10+Time*3.f)) / 40;
    displacement += smoothstep(-3.14*2, 3.14*2, sin(p.z*10+Time*3.f)) / 40;
    dist = dist + displacement/3;
//     plan
    //  float plan = min(dist, sdBox(p-vec3(0, 1., 0), vec3(1, 1, 2)));
    //  displacement = (sin(p.x*4+Time*3.f) + tan(p.y*4+Time*3.f))/4;
    //  dist = min(plan, dist);
    return dist;
}

vec3 estimateNormal(vec3 p) {
    return normalize(vec3(
    sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),
    sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),
    sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

float rayMarching(vec3 eye, vec3 dir) {
    float depth = 0;

    for (int i = 0; i < 200; i++) {
        float dist = sceneSDF(eye + dir * depth);
        if (dist < EPSILON)
        return depth;
        depth += dist;
        if (depth > MAX_DEPTH)
        return MAX_DEPTH;
    }

    return 100;
}

vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy*vec2(1, 1), -z));
}

float floorRay(vec3 eye, vec3 dir) {
    return -eye.y / dir.y;
}

vec3 floorColor(vec3 p, float distance) {
    float f = clamp(cos(p.x*20)/3.14*10, 0, 1);
    f = clamp(cos(p.z*2)*100, -1, 1) * clamp(cos(p.x*2)*100, -1, 1);
    return vec3(f) / 8 + 1;
//    return clamp(pow(6.5/distance, 0.9), 0, 1) * (vec3(f) / 8 + 1);
}

vec3 lighting(vec3 color, vec3 N, vec3 rayDir, vec3 sun) {
    vec3 L = -normalize(sun);

    float diffuse = clamp(dot(N, L), 0, 1);
    float specular = pow(dot(reflect(-L, N), rayDir), 1.75);
    return color * (1-diffuse) + specular * color;
}

void main()
{
    vec3 eye = -View[3].xyz * mat3(View);
    vec3 dir = (Rotation * vec4(rayDirection(45.0, Resolution.xy, gl_FragCoord.xy), 0)).xyz;

    vec3 light = vec3(cos(Time*1)*2000, 10, sin(Time)*0);
    float d = rayMarching(eye, dir);
    vec3 p = eye + dir * d;
    vec3 N = estimateNormal(p);

    fragColor = vec3(0);
    float plan = -eye.y / dir.y;
    if (d < MAX_DEPTH && d < plan) {
        eye = p;
        dir = refract(dir, N, 1./2.5);
        d = rayMarching(eye, dir);
        p = eye + dir * d;
        N = estimateNormal(p);

        d = floorRay(p, refract(dir, N, 2.5/1.));
        if (d > 0.f) {
            fragColor = floorColor(eye + dir * d, d);
        }

    }
    else {
        d = floorRay(eye, dir);
        if (d > 0.f)
            fragColor = floorColor(eye + dir * d, d);
        fragColor = lighting(fragColor, vec3(0,1,0),dir,light);
    }
}