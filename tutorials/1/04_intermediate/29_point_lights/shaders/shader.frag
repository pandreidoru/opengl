#version 330

in vec4 v_col;
in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;

struct Light {
  vec3 colour;
  float ambient_intensity;
  float diffuse_intensity;
};

struct DirectionalLight {
  Light base;
  vec3 direction;
};

struct PointLight {
  Light base;
  vec3 position;
  float constant;
  float linear;
  float exponent;
};

struct Material {
  float specular_intensity;
  float shininess;
};

uniform int point_light_count;

uniform DirectionalLight directional_light;
uniform PointLight point_lights[MAX_POINT_LIGHTS];

uniform sampler2D the_texture;
uniform Material material;
uniform vec3 eye_pos;

vec4 CalcLightByDirection(Light light, vec3 direction) {
  vec4 ambient_colour = vec4(light.colour, 1) * light.ambient_intensity;

  float diffuse_factor = max(dot(normalize(normal), normalize(direction)), 0.0f);
  vec4 diffuse_colour = vec4(light.colour * light.diffuse_intensity * diffuse_factor, 1);

  vec4 specular_colour = vec4(0, 0, 0, 0);

  // Specular lighting exists only diffuse lighting exists (the fragment is not parallel with the source light
  // direction)
  if (diffuse_factor > 0.0f) {
    vec3 frag_to_eye = normalize(eye_pos - frag_pos);
    vec3 reflected_vertex = normalize(reflect(direction, normalize(normal)));

    float specular_factor = dot(frag_to_eye, reflected_vertex);
    if (specular_factor > 0.0f) {
      specular_factor = pow(specular_factor, material.shininess);
      specular_colour = vec4(light.colour * material.specular_intensity * specular_factor, 1);
    }
  }

  return (ambient_colour + diffuse_colour + specular_colour);
}

vec4 CalcDirectionalLight() {
  return CalcLightByDirection(directional_light.base, directional_light.direction);
}

vec4 CalcPointLights() {
  vec4 total_colour = vec4(0, 0, 0, 0);
  for (int i = 0; i < point_light_count; i++) {
    vec3 direction = frag_pos - point_lights[i].position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 colour = CalcLightByDirection(point_lights[i].base, direction);
    float attenuation = point_lights[i].exponent * distance * distance + point_lights[i].linear * distance + point_lights[i].constant;
    total_colour += (colour / attenuation);
  }

  return total_colour;
}

void main() {
  vec4 final_colour = CalcDirectionalLight();
  final_colour += CalcPointLights();

  colour = texture(the_texture, tex_coord) * final_colour;
}