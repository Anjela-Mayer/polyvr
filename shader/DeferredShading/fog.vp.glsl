#version 120

attribute vec4 osg_Vertex;
attribute vec2 osg_MultiTexCoord0;

void main(void) {
    gl_Position = osg_Vertex;
    gl_Position.z = -1.0;
    gl_TexCoord[0] = vec4(osg_MultiTexCoord0,0.0,0.0);
}
