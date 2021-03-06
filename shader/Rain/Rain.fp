#version 400 compatibility
#define M_PI 3.1415926535897932384626433832795

// gen
in vec3 norm;
in vec4 pos;
in vec2 tcs;
in mat3 miN;
in mat4 miP;
vec3 fragDir = vec3(0,0,0);
vec3 newfragDir;
vec4 color;
bool debugB = false;

uniform vec2 OSGViewportSize;
uniform float rainOffset;
uniform float rainDensity;
uniform bool depthTexer;

uniform vec3 dropColor;
uniform float dropWidth;
uniform float dropLength;
uniform float dropSpeed;
uniform float dropDensity;

uniform vec3 lastCt2;
uniform vec3 lastCt3;
uniform vec3 lastCt5;
uniform vec3 lastCt8;

uniform sampler2D tex;

uniform float camH;

float theta;
vec3 PCam0;
vec3 PCam;

void computeDirection() {
	newfragDir = normalize( miN * (miP * pos).xyz );
}

float hash(vec2 co){
    	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float gettheta(vec3 d){
	return acos(d.y);
}

vec3 getlastCt(float D) {
	vec3 lastCt;
	if (D == 2) lastCt = vec3(lastCt2.x,PCam.y,lastCt2.z);
	if (D == 3) lastCt = vec3(lastCt3.x,PCam.y,lastCt3.z);
	if (D == 5) lastCt = vec3(lastCt5.x,PCam.y,lastCt5.z);
	if (D == 8) lastCt = vec3(lastCt8.x,PCam.y,lastCt8.z);
	return lastCt;
}

vec3 getlastCt0(float D) {
	vec3 lastCt;
	if (D == 2) lastCt = lastCt2;
	if (D == 3) lastCt = lastCt3;
	if (D == 5) lastCt = lastCt5;
	if (D == 8) lastCt = lastCt8;
	return lastCt;
}

vec3 calcDirFromFragDir(float D) {
	vec3 dir;
	float n;
	vec3 localOffset = PCam0 - getlastCt0(D);
	float pX = localOffset.x;
	float pZ = localOffset.z;
	float fX = newfragDir.x;
	float fZ = newfragDir.z;
	float r = D/2;
	//n = -(sqrt(fZ*fZ* (r*r - pX*pX) + 2*pX*fX*pZ*fZ + fX*fX*(r*r - pZ*pZ)) + pX*fX + pZ*fZ)/(fX*fX + fZ*fZ);
	n = (sqrt(fZ*fZ* (r*r - pX*pX) + 2*pX*fX*pZ*fZ + fX*fX*(r*r - pZ*pZ)) - pX*fX - pZ*fZ)/(fX*fX + fZ*fZ);
	vec3 Point = PCam + newfragDir * n;
	dir = normalize(Point - getlastCt(D));	
	//fragDir
	//PCam0

	return dir;
}

/** OBSTRUCTION RETURNS TRUE, IF RAIN IS BLOCKED **/
bool obstruction(float D){
	//vec3 fragDir = calcDirFromFragDir(D);
	float phi = -atan(fragDir.x,fragDir.z);
	vec2 texPos = vec2(0.5+sin(phi)*D/(camH),0.5+cos(phi)*D/(camH));	//texture positions
	vec4 texC = texture2D(tex,texPos); 					//RGB value of pixel
	float disCam = texC.r * (512-0.1);					//distance to cam above	
	float thetaReal = atan(D,camH-disCam-0.1);
	
	//if (texC.x==0) return false;
	//else return true;
	//if (thetaReal < 3.1) return true;
	//if (thetaReal > 3) return true; //seems like for certain viewing angles, thetaReal suddenly is PI (180deg)
	//if (disCam > 80) return true;	
	if (thetaReal < gettheta(fragDir)) return true;
	return false;
}

vec3 debugObstruction(){
	vec3 color = vec3(1,0,0);
	//vec3 fragDir = calcDirFromFragDir(D);
	if (atan( fragDir.x, fragDir.z)*180/M_PI<1 && atan( fragDir.x, fragDir.z)*180/M_PI>-1 && gettheta(fragDir)>M_PI/2) return vec3(1,1,1);	
	if (atan( fragDir.x, fragDir.z)*180/M_PI<91 && atan( fragDir.x, fragDir.z)*180/M_PI>89 && gettheta(fragDir)>M_PI/2) return vec3(0,0,1);	
	if ((atan( fragDir.x, fragDir.z)*180/M_PI<-179 || atan( fragDir.x, fragDir.z)*180/M_PI>179) && gettheta(fragDir)>M_PI/2) return vec3(0,0,1);	
	if (atan( fragDir.x, fragDir.z)*180/M_PI<-89 && atan( fragDir.x, fragDir.z)*180/M_PI>-91 && gettheta(fragDir)>M_PI/2) return vec3(0,1,1);	
	if (obstruction(1)) color = vec3(0,1,0);
	//if (obstruction(2)) color = vec3(0,1,1);
	//if (obstruction(5)) color = vec3(0,0,1);
	//if (obstruction(8)) color = vec3(0,0,1);	
	return color;
}

/** DROPSIZE CALCULATION HERE */
float getdropWidth(in float theta,float distance){
	if (theta < M_PI/2) return dropWidth*2*theta/M_PI*2/distance;
	else {
		if (theta > M_PI/2) return dropWidth*2*(M_PI-theta)/M_PI*2/distance;
		else return 0;
	}
}
float getdropLength(in float theta,float distance){
	if (theta < M_PI/2) return dropLength*2*theta/M_PI*2/distance;
	else {
		if (theta > M_PI/2) return dropLength*2*(M_PI-theta)/M_PI*2/distance;
		else return 0;
	}
}

float getOffset(in float rOffset, in float dropdis,in float D) {
	return dropSpeed*rOffset*5/sqrt(D);
	//return 0;
}

void computeDepth(vec4 position) {
	float d = position.z / position.w;
	gl_FragDepth = d*0.5 + 0.5;
}

void ccDepth(vec3 position){
	float far=gl_DepthRange.far; float near=gl_DepthRange.near;
	
	vec4 pp = vec4(position,1);
	vec4 eye_space_pos = gl_ModelViewMatrix * pp;
	vec4 clip_space_pos = gl_ProjectionMatrix * eye_space_pos;

	float ndc_depth = clip_space_pos.z / clip_space_pos.w;

	//float depth = (((far-near) * ndc_depth) + near + far) / 2.0;
	gl_FragDepth = ndc_depth;
}

vec3 worldCoords(float D){
	//vec3 fragDir = calcDirFromFragDir(D);
	mat4 m = inverse(gl_ModelViewMatrix);
	vec3 PCam = (m*vec4(0,0,0,1)).xyz;
	float relX = D*sin(atan( fragDir.x, fragDir.z)); //relative to cam pos
	float relY = 1/(D*tan(gettheta(fragDir)));
	float relZ = -D*cos(atan( fragDir.x, fragDir.z));
	vec3 world = vec3(PCam.x-relX,PCam.y-relY,PCam.z-relZ);
	return world;
}

//** computes whether there should be raindrops at certain distance D **/
bool isD(float D) {
	fragDir = calcDirFromFragDir(D);
	float dropdis = 2/(D*D); 	// horizontal distance between drops
	float dropdisy = rainDensity*6/dropDensity; // vertical distance between drops
	float dropwidth = getdropWidth(gettheta(fragDir),D);
	float droplength = getdropLength(gettheta(fragDir),D);
	float toffset = rainOffset;
	float phi = atan( fragDir.x, fragDir.z);

	vec2 noise = vec2(floor(phi*180/M_PI/dropdis),floor((D/tan(gettheta(fragDir))+getOffset(toffset,dropdisy,D))/dropdisy));

	float israindropx = mod(phi*180/M_PI+7*hash(noise),dropdis); //phi horizontal in [degree]
	float israindropy = mod(D/tan(gettheta(fragDir))+getOffset(toffset,dropdisy,D)+dropdisy*hash(noise),dropdisy); //height vertical in [m]
	

	float computeZ=sin(0.5*M_PI-gettheta(fragDir))*D;
	//if (atan( fragDir.x, fragDir.z) < 0) realZ = D*cos(-atan( fragDir.x, fragDir.z));
		
	if (D == 1) gl_FragDepth = 0.8;
	if (D == 2) gl_FragDepth = 0.95;
	if (D == 3) gl_FragDepth = 0.98;	
	if (D == 5) gl_FragDepth = 0.99;
	if (D == 8) gl_FragDepth = 0.99;
	if (D > 8) gl_FragDepth = 0.993;
	//computeDepth(gl_ModelViewProjectionMatrix*vec4(worldCoords(D),1));

	if (gettheta(fragDir)>0.3 && israindropx < dropwidth && israindropy < droplength && !obstruction(D)) return true;
	else return false;

	return true;
}

vec3 checkrad() {
	//vec3 fragDir = calcDirFromFragDir(D);
	//DEBUGGING POINTERS -- RAIN OCCLUSION ONLY WORKS BETWEEN GREEN POINTERS
	if (debugB) {
		if (atan( fragDir.x, fragDir.z)*180/M_PI>-1 && atan( fragDir.x, fragDir.z)*180/M_PI<1 && gettheta(fragDir)>M_PI/2) return vec3(1,1,1);	
		if (atan( fragDir.x, fragDir.z)*180/M_PI>-68 && atan( fragDir.x, fragDir.z)*180/M_PI<-66 && gettheta(fragDir)>M_PI/2) return vec3(0,1,0);	
		if (atan( fragDir.x, fragDir.z)*180/M_PI>-113 && atan( fragDir.x, fragDir.z)*180/M_PI<-110 && gettheta(fragDir)>M_PI/2) return vec3(1,0,0);	
		if (atan( fragDir.x, fragDir.z)*180/M_PI>-158 && atan( fragDir.x, fragDir.z)*180/M_PI<-156 && gettheta(fragDir)>M_PI/2) return vec3(0,1,0);	
		if (atan( fragDir.x, fragDir.z)*180/M_PI>-180 && atan( fragDir.x, fragDir.z)*180/M_PI<-179 && gettheta(fragDir)>M_PI/2) return vec3(0,1,1);	
	}
	//vec3 color = vec3(0.3,0.3,0.7);
	vec3 color = dropColor;
	
	if (debugB) color = vec3(1.,0.,0.);

	if (isD(2) || isD(3) ||isD(5) || isD(8)) return color;
	else discard;
}

void main() {
	computeDirection();

	// \theta is angle of fragDir to zenith
	//vec3 fragDir = calcDirFromFragDir(D);
	theta = acos(fragDir.y);

	mat4 m = inverse(gl_ModelViewMatrix);
	PCam = (m*vec4(0,0,0,1)).xyz;
	vec3 P0 = vec3(0,100,0);
	vec3 T0 = P0-PCam;
	vec3 D0 = normalize( P0-PCam );

	PCam0 = vec3(PCam.x,0.0,PCam.z);
	//if (dot(D0,fragDir) < 0.9999 && dot(D0,fragDir) > 0.999) discard;

	vec3 check = checkrad();
	//gl_FragColor = vec4(debugObstruction(),0.3); //DebugMode
	if (!depthTexer) gl_FragColor = vec4(check,1);
	if (depthTexer) gl_FragColor = vec4(1,0,0,1);
}





