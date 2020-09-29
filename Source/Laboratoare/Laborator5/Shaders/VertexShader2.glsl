#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normals;
layout(location = 2) in vec2 v_text_attributes;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;
uniform vec3 color;

// TODO: output values to fragment shader
out vec3 frag_color;
out vec3 frag_position;
out vec2 frag_text_attributes;
out vec3 frag_normals;

void main()
{
	// TODO: send output to fragment shader
	frag_color = color;
	frag_position = v_position;
	frag_text_attributes = v_text_attributes;
	frag_normals = v_normals;

	// TODO: compute gl_Position
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
