/*!
 * Based on http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html
 */

#version 400

in vec2 fs_uv;
uniform vec3 lightPos;
uniform sampler2D u_RT1_tex;

layout (location = 0) out vec4 FragColor;

uniform float u_exposure 	= 0.6;
uniform float u_decay 		= 0.93;
uniform float u_density 	= .96;
uniform float u_weight 		= 0.4;
uniform int   u_NUM_SAMPLES 	= 80;
uniform float u_clampMax 	= 1.0;

void main()
{
    vec2 deltaTextCoord = vec2(fs_uv - lightPos.xy);
    deltaTextCoord *= 1.0 /  float(u_NUM_SAMPLES) * u_density;
    vec2 coord = fs_uv;
    float illuminationDecay = 1.0;

    for(int i=0; i < u_NUM_SAMPLES ; i++)
    {
            coord -= deltaTextCoord;
            vec4 texel = texture2D(u_RT1_tex, coord);
            texel *= illuminationDecay * u_weight;

            FragColor += texel;

            illuminationDecay *= u_decay;
    }
    FragColor *= u_exposure;
  //  FragColor = clamp(FragColor, 0.0, u_clampMax) * vec4(1.0,1.0,0.94, 1.0);
}