/*!
 * Based on http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html
 */

#version 400

in vec2 fs_uv;
uniform sampler2D u_RT1_tex;

// PROBLEM: the godrays that we have are looking stunning.
//	there is one issue with it though. 
//	when you move in the scene without moving the mouse aroung, the light scatters perfectly.
//	but when you move the mouse around, the rays also scatter in different directions.
//	this thing does not happen in real life when you look at lights.
//	A temp solution to that would be to have the position of the light fixed.
//	but I believe what needs to happen is, we PROPERLY need to calculate the position of the light on the screen.

/* temp solution */ //	vec2  u_lightPos = vec2( .2, 1.2 )	;

uniform vec2  u_lightPos;

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