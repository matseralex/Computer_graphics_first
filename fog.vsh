
varying float atten;
varying float fogFactor;
varying vec3 lightVec, viewVec;
attribute vec3 tangent;

void main(void)
{

  const float LOG2 = 1.442695;
	gl_Position = ftransform();
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;
	gl_TexCoord[1].xy = gl_MultiTexCoord1.xy;
	gl_TexCoord[2].xy = gl_MultiTexCoord2.xy;

	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec3 t = normalize(gl_NormalMatrix * tangent);
	vec3 b = cross(n, t);

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 vLVec = gl_LightSource[0].position.xyz - vVertex;

	atten = 1.0 / (1.0 +10.0 * dot(vLVec, vLVec));

	vec3 vVec = vVertex;
	viewVec.x = dot(vVec, t);
	viewVec.y = dot(vVec, b);
	viewVec.z = dot(vVec, n);

	lightVec.x = dot(vLVec, t);
	lightVec.y = dot(vLVec, b);
    lightVec.z = dot(vLVec, n);
/*
    gl_FogFragCoord = length(vVertex);
    fogFactor = gl_Fog.density * gl_FogFragCoord*20.0;
    fogFactor = clamp(fogFactor, 0.9, 0.2);
    fogFactor = gl_Fog.density * gl_FogFragCoord*20.0;
    fogFactor = clamp(fogFactor, 0.9, 0.2);
*/

    if (vVertex.x>0.4) {
        gl_FogFragCoord = length(vVertex);
        fogFactor = gl_Fog.density * gl_FogFragCoord*20.0;
        fogFactor = clamp(fogFactor, 0.9, 0.2);
    }
    else {
        gl_FogFragCoord=2.0;
        fogFactor=60.0;
    }
}
