#version 120

#extension GL_ARB_texture_rectangle : require
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect     texBufPos;
uniform sampler2DRect     texBufNorm;
uniform sampler2DRect     texBufDiff;
uniform vec2              vpOffset;
uniform int               channel;

uniform vec3 lightUp;
uniform vec3 lightDir;
uniform vec3 lightPos;

vec3 pos;
vec3 norm;
vec4 color = vec4(0);

vec4 OSG_SSME_FP_calcShadow(in vec4 ecFragPos);

void computeDirLight() {
    vec3  lightDir = normalize( lightDir );
    float NdotL    = max(dot(norm, lightDir), 0.);

    if (NdotL > 0.) {
        vec4  shadow = OSG_SSME_FP_calcShadow(vec4(pos, 1.));
        color = shadow * NdotL * color * gl_LightSource[0].diffuse;
    } else color = vec4(0);
}

void main(void) {
    vec2 lookup = gl_FragCoord.xy - vpOffset;
    norm   = texture2DRect(texBufNorm, lookup).xyz;

    if (dot(norm, norm) < 0.95) discard;
    else {
        vec4 posAmb = texture2DRect(texBufPos,  lookup);
        pos = posAmb.xyz;
        color = texture2DRect(texBufDiff, lookup);
	
	if (channel == 0) computeDirLight();
	if (channel == 1) color = vec4(posAmb.xyz, 1.0);
	if (channel == 2) color = vec4(norm.xyz, 1.0);
	if (channel == 3) color = vec4(color.xyz, 1.0);
        gl_FragColor = color;
    }
}
