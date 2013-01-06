
uniform  vec4	lightPos;
uniform	vec4	eyePos;
uniform	float	tl;

varying vec3 lt;
varying vec3 ht;
varying vec3 fur;

void main ()
{
	vec4		 pp = gl_Vertex + 0.8 * tl * vec4 ( gl_Normal, 1.0 );

	vec3	p = (gl_ModelViewMatrix * gl_Vertex).xyz;						// transformed point to world space
	vec3	l = normalize ( lightPos.xyz - p );				// vector to light source
	vec3	v = normalize ( eyePos.xyz   - p );				// vector to the eye
	vec3	h = l + v;
	vec3	n = gl_NormalMatrix * gl_Normal;				// transformed n
	vec3 	t = gl_NormalMatrix * gl_MultiTexCoord1.rgb;	// transformed t
	vec3 	b = gl_NormalMatrix * gl_MultiTexCoord2.rgb;	// transformed b

	lt       = vec3 ( dot ( l, t ), dot ( l, b ), dot ( l, n ) );
	ht       = vec3 ( dot ( h, t ), dot ( h, b ), dot ( h, n ) );
	fur   = vec3 ( 0.0, 0.0, 1.0 );

	gl_Position     = gl_ModelViewProjectionMatrix * pp;
	gl_TexCoord [0] = 0.3*gl_MultiTexCoord0;

}
