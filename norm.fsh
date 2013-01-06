varying vec3 lt;
varying vec3 ht;

uniform sampler2D bumpMap;
uniform sampler2D diffuseMap;
uniform float effect;
void main (void)
{
  const vec4 specColor = vec4 (1.0 ,0.7 ,0.9, 1);
	//get normal pertrubation

	vec3 n = texture2D (bumpMap, gl_TexCoord[0].xy).rgb;
	vec3 nt = normalize (7.0*abs(n)-6.0);
	vec3 l2 = normalize(lt);
	vec3 h2 = normalize (ht);

	float diff = max (dot (nt , l2), 0.1) + 0.1;
	float spec = pow ( max  (   dot(nt , h2)   , 0.2) , 90);

	gl_FragColor  = diff* texture2D (diffuseMap , gl_TexCoord[0].st).rgba+ spec*specColor;

    if(gl_FragCoord.x > 380.0 && gl_FragCoord.x < 500.0 && effect==1.0)
    {
        float fogdeg=0.0;
        fogdeg=(gl_FragCoord.x-380.0)*(500.0-gl_FragCoord.x)/(120.0*120.0);
        gl_FragColor = diff* texture2D (diffuseMap , gl_TexCoord[0].st).rgba+ spec*specColor + vec4(fogdeg,fogdeg,0.0,1.0);

    }



}
