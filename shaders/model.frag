#version 450 core

in vec2 tex_coords;
//in vec3 normals;
//in vec3 frag_pos;

out vec4 frag_color;

uniform sampler2D sampler;
//uniform vec3 light_pos;

void main() {
  //phong

  /*
  float ambient_strength = 0.5;
  //texture is the ambient color
  tex_color = texture(sampler, tex_coords_in);

  float diffuse_strength = 0.5;
  vec3 light_dir = light_pos - frag_pos;
  vec3 diffuse = max(dot(light_dir, normals), 0);

  frag_color = vec4(diffuse * diffuse_strength + vec3(tex_color * ambient_strength));
  */
  frag_color = texture(sampler, tex_coords);
}
