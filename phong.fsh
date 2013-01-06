uniform sampler2D colorTexture;
varying  vec3 l;
varying	vec3 v;
varying	vec3 n;

void main (void)
{
	 vec4	diffColor = vec4 ( 0.7, 0.3, 0.5, 1.0 );
	const vec4	specColor = vec4 ( 0.9, 0.2, 0.2, 1.0 );
	const float	specPower = 90.0;

	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	v2   = normalize ( v );
	vec3	r    = reflect ( -v2, n2 );
	/*vec4 r1;
	r1.r=r.x;
	r1.g=r.y;
	r1.b=r.z;
	r1.a=0.0;*/

	vec4	diff = diffColor * max ( dot ( n2, l2 ), 0.1 );
	vec4	spec = specColor * pow ( max ( dot ( l2, r ), 0.2 ), specPower );

	gl_FragColor = (diff + spec)* 2.0*texture2D(colorTexture, gl_TexCoord[0].st).rgba;


}
