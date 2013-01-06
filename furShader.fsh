
uniform sampler2D furTexture;

uniform  vec4	lightPos;
uniform	vec4	eyePos;
uniform	float	tl;

varying vec3 lt;
varying vec3 ht;
varying vec3 fur;

void main ()
{
	const vec3	specColor = vec3 ( 0.0, 0.0, 0.3 );
	const vec3	luminance = vec3 ( 0.0, 0.8, 0.0 );

	vec3	tang  = normalize ( fur);
	float	dot1  = dot       ( normalize ( lt ), tang );
	float	dot2  = dot       ( normalize ( ht ), tang );

	dot1 = min ( 1.0, 0.1+1.0 - dot1 * dot1 );
	dot2 = pow ( 1.0 - dot2 * dot2, 0.0 );

	vec4	color = texture2D ( furTexture, gl_TexCoord [0].xy * vec2 ( 6.0, 12.0 ) ).rgba;

	gl_FragColor = vec4 ( color.xyz * dot1 + specColor * dot2*1.5, 1.5*color.w*tl );
}
