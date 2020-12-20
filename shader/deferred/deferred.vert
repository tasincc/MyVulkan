#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;

layout (location = 0) out vec2 outUV;

layout (set = 0, binding = 0) uniform UBO 
{
	mat4 model;
	mat4 projection;
	mat4 view;
} ubo;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPos.xyz, 1.0);
    // gl_Position = ubo.model * vec4(inPos.xyz, 1.0);
    // gl_Position = vec4(inPos.xyz, 1.0);
    outUV = inUV;
}