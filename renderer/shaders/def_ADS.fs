#version 450 core

in vec2 fs_uv;

uniform sampler2D u_albedo;
uniform sampler2D u_position;
uniform sampler2D u_normal;

struct Light {
    vec3 Position;
    vec3 Color;
};

const int NR_LIGHTS = 32;
uniform Light u_lights[NR_LIGHTS];
uniform vec3 u_v3ViewPos;

void main()
{             
    // retrieve data from G-buffer
    vec3 FragPos = texture(u_position, fs_uv).rgb;
    vec3 Normal = texture(u_normal, fs_uv).rgb;
    vec3 Albedo = texture(u_albedo, fs_uv).rgb;
    float Specular = texture(u_albedo, fs_uv).a;
    
    // then calculate lighting as usual
    vec3 lighting = Albedo * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(u_v3ViewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(u_lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * u_lights[i].Color;
        lighting += diffuse;
    }
    
    gl_FragColor = vec4(lighting, 1.0);
}  

