attribute vec3      a_position;
attribute vec2      a_textureCoord;

uniform sampler2D   u_sampler;

varying vec2        v_textureCoord;

void main()
{
    v_textureCoord = a_textureCoord;
    gl_Position = vec4(a_position, 1.0);
}