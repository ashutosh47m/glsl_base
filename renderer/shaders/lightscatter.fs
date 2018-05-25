/*!
 * Based on http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html
 */

#version 400

in vec2 fs_uv;
uniform sampler2D u_RT1_tex;
uniform vec2  u_lightPos	;

layout (location = 0) out vec4 FragColor;

uniform float	u_exposure;
uniform float	u_decay;
uniform float	u_density;
uniform float	u_weight;
uniform int		u_NUM_SAMPLES;	
//uniform float	u_clampMax;

void main()
{
    vec2 deltaTextCoord = vec2(fs_uv - u_lightPos);
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