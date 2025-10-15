#version 330 core

in vec3 fragment_position;
in vec3 normal;

out vec4 fragment_color;

uniform vec3 camera_position;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 object_color;

void main() {

    // ambience calculation
    float ambient_strength = 0.1;
    vec3 ambient = light_color * ambient_strength;

    // diffuse calculation
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - fragment_position);
    vec3 diffuse = light_color * max(dot(norm, light_direction), 0.0);

    // specular calculation
    float specular_strength = 0.5;
    vec3 view_direction = normalize(camera_position - fragment_position);
    vec3 reflect_direction = reflect(-light_direction, norm);
    specular_strength *= pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = light_color * specular_strength;

    vec3 result = (ambient + diffuse + specular) * object_color;
    fragment_color = vec4(result, 1.0);
}
