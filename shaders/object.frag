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

    float half_angle;
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

uniform DirectionalLight directional_light;
uniform PointLight point_light;
uniform Spotlight flashlight;

uniform Material material;

uniform vec3 camera_position;

void main() {

    // attenuation
    //float distance = length(point_light.position - fragment_position);
    //float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + point_light.quadratic * (distance * distance));

    // ambient
    //vec3 ambient = directional_light.ambient * vec3(texture(material.diffuse, texture_coordinates));
    //vec3 ambient = point_light.ambient * vec3(texture(material.diffuse, texture_coordinates));

    // diffuse

    //vec3 norm = normalize(normal);
    //vec3 light_direction = normalize(-directional_light.direction);
    //float diffuse_strength = max(dot(norm, light_direction), 0.0);
    //vec3 diffuse = directional_light.diffuse * diffuse_strength * vec3(texture(material.diffuse, texture_coordinates));

    //vec3 norm = normalize(normal);
    //vec3 light_direction = normalize(point_light.position - fragment_position);
    //float diffuse_strength = max(dot(norm, light_direction), 0.0);
    //vec3 diffuse = point_light.diffuse * diffuse_strength * vec3(texture(material.diffuse, texture_coordinates));

    // specular
    //vec3 view_direction = normalize(camera_position - fragment_position);
    //vec3 reflect_direction = reflect(-light_direction, norm);
    //float specular_strength = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    //vec3 specular = directional_light.specular * specular_strength * vec3(texture(material.specular, texture_coordinates));

    //vec3 view_direction = normalize(camera_position - fragment_position);
    //vec3 reflect_direction = reflect(-light_direction, norm);
    //float specular_strength = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    //vec3 specular = point_light.specular * specular_strength * vec3(texture(material.specular, texture_coordinates));

    // flashlight cutoff angle
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 light_direction = normalize(flashlight.position - fragment_position);
    float cutoff_angle = dot(light_direction, normalize(-flashlight.direction));

    if (cutoff_angle > flashlight.half_angle) {
        ambient = flashlight.ambient * vec3(texture(material.diffuse, texture_coordinates));

        vec3 norm = normalize(normal);
        float diffuse_strength = max(dot(norm, light_direction), 0.0);
        diffuse = flashlight.diffuse * diffuse_strength * vec3(texture(material.diffuse, texture_coordinates));

        vec3 view_direction = normalize(camera_position - fragment_position);
        vec3 reflect_direction = reflect(-light_direction, norm);
        float specular_strength = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
        specular = flashlight.specular * specular_strength * vec3(texture(material.specular, texture_coordinates));
    }
    else {
        ambient = flashlight.ambient * vec3(texture(material.diffuse, texture_coordinates));
    }

    vec3 result = (ambient + diffuse + specular);
    //vec3 result = (ambient + diffuse + specular) * attenuation; // for point light, maybe also spotlight

    fragment_color = vec4(result, 1.0);
}
