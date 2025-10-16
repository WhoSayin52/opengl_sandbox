#version 330 core

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 fragment_position;
in vec3 normal;

out vec4 fragment_color;

uniform Light light;
uniform Material material;

uniform vec3 camera_position;

void main() {

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse calculation
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light.position - fragment_position);
    float diffuse_strength = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light.diffuse * material.diffuse * diffuse_strength;

    // specular calculation
    vec3 view_direction = normalize(camera_position - fragment_position);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float specular_strength = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = light.specular * specular_strength * material.specular;

    vec3 result = (ambient + diffuse + specular);
    fragment_color = vec4(result, 1.0);
}
