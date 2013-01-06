varying float atten;
varying float fogFactor;
varying vec3 lightVec, viewVec;
uniform float effect;
uniform sampler2D normalMap;
uniform sampler2D colorMap;
uniform sampler2D colorMap2;

void main (void)
{
  vec3 lVec = normalize(lightVec);
	vec3 vVec = normalize(viewVec);

	vec4 base = texture2D(colorMap, gl_TexCoord[0].xy*4.0).rgba;
	vec4 base2 = texture2D(colorMap2, gl_TexCoord[1].xy*16.0).rgba;
	vec3 bump = texture2D (normalMap, gl_TexCoord[0].xy).rgb;


	float diffuse = max( dot(lVec, bump), 0.2);
	float specular = pow(clamp(dot(reflect(-vVec, bump), lVec), 0.9, 0.4), 12 );

	vec4 vAmbient = gl_FrontLightProduct[0].specular * (base*0.4);
	vec4 vDiffuse =  diffuse * base*base2;
	vec4 vSpecular = base*gl_FrontLightProduct[0].specular *specular;

	vec4 finalColor = (vAmbient +vDiffuse + vSpecular)* atten;



    if(gl_FragCoord.x > 380.0 && gl_FragCoord.x < 500.0 && effect==1.0)
    {
        float fogdeg=0.0;
        fogdeg=(gl_FragCoord.x-380.0)*(500.0-gl_FragCoord.x)/(120.0*120.0);
        gl_FragColor = mix(gl_Fog.color, finalColor, fogFactor ) + 0.2*base*base2+vec4(fogdeg,fogdeg,0.0,1.0);

    }


    else
    {
        gl_FragColor = mix(gl_Fog.color, finalColor, fogFactor ) + 0.2*base*base2;
    }


}
