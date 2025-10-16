#version 330

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 fragment_color;

uniform Light light;

void main() {
    fragment_color = vec4(vec3(1.0f), 1.0);
}
