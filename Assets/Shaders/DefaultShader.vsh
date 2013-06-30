attribute vec3      a_position;
attribute vec2      a_textureCoord;

uniform mat4        u_mvp;
uniform sampler2D   u_sampler;

varying vec2        v_textureCoord;

void main()
{
    v_textureCoord = a_textureCoord;
    gl_Position = u_mvp * vec4(a_position, 1.0);
}