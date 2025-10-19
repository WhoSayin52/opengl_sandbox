#version 330 core

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Spotlight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float inner_cone_angle;
    float outer_cone_angle;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

in vec2 texture_coordinates;

in vec3 fragment_position;
in vec3 normal;

out vec4 fragment_color;

#define POINT_LIGHT_COUNT 4

uniform DirectionalLight directional_light;
uniform PointLight point_lights[POINT_LIGHT_COUNT];
uniform Spotlight flashlight;

uniform Material material;

uniform vec3 camera_position;

vec3 calculate_directional_light(DirectionalLight directional_light, vec3 normal, vec3 view_direction);
vec3 calculate_point_light(PointLight point_light, vec3 normal, vec3 fragment_position, vec3 view_direction);
vec3 calculate_spotlight(Spotlight spotlight, vec3 normal, vec3 fragment_position, vec3 view_direction);

void main() {
    vec3 norm = normalize(normal);
    vec3 view_direction = normalize(camera_position - fragment_position);

    // adding directional light calculations to fragment
    vec3 result = calculate_directional_light(directional_light, norm, view_direction);

    // adding point light calculations to fragment
    for (int i = 0; i < POINT_LIGHT_COUNT; ++i) {
        result += calculate_point_light(point_lights[i], norm, fragment_position, view_direction);
    }
    // adding spotlight calculation to fragment
    result += calculate_spotlight(flashlight, norm, fragment_position, view_direction);

    fragment_color = vec4(result, 1.0);
}

vec3 calculate_directional_light(DirectionalLight directional_light, vec3 normal, vec3 view_direction) {
    // ambient
    vec3 ambient = directional_light.ambient * vec3(texture(material.diffuse, texture_coordinates));

    // diffuse
    vec3 light_direction = normalize(-directional_light.direction);
    float diffuse_strength = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = directional_light.diffuse * diffuse_strength * vec3(texture(material.diffuse, texture_coordinates));

    // specular
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_strength = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = directional_light.specular * specular_strength * vec3(texture(material.specular, texture_coordinates));

    return (ambient + diffuse + specular);
}

vec3 calculate_point_light(PointLight point_light, vec3 normal, vec3 fragment_position, vec3 view_direction) {
    // ambient
    vec3 ambient = point_light.ambient * vec3(texture(material.diffuse, texture_coordinates));

    // diffuse
    vec3 light_direction = normalize(point_light.position - fragment_position);
    float diffuse_strength = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = point_light.diffuse * diffuse_strength * vec3(texture(material.diffuse, texture_coordinates));

    // specular
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_strength = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = point_light.specular * specular_strength * vec3(texture(material.specular, texture_coordinates));

    // attenuation
    float distance = length(point_light.position - fragment_position);
    float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + point_light.quadratic * (distance * distance));

    return (ambient + diffuse + specular) * attenuation;
}

vec3 calculate_spotlight(Spotlight spotlight, vec3 normal, vec3 fragment_position, vec3 view_direction) {
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 light_direction = normalize(spotlight.position - fragment_position);
    float theta = dot(light_direction, normalize(-spotlight.direction));
    float epsilon = spotlight.inner_cone_angle - spotlight.outer_cone_angle;
    float intensity = clamp((theta - spotlight.outer_cone_angle) / epsilon, 0.0, 1.0);

    if (theta > spotlight.outer_cone_angle) {
        ambient = spotlight.ambient * vec3(texture(material.diffuse, texture_coordinates));

        float diffuse_strength = max(dot(normal, light_direction), 0.0);
        diffuse = spotlight.diffuse * diffuse_strength * vec3(texture(material.diffuse, texture_coordinates));

        vec3 reflect_direction = reflect(-light_direction, normal);
        float specular_strength = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
        specular = spotlight.specular * specular_strength * vec3(texture(material.specular, texture_coordinates));
    }
    else {
        ambient = spotlight.ambient * vec3(texture(material.diffuse, texture_coordinates));
    }

    return ambient + (diffuse + specular) * intensity; // for spotlight soft edges
}
