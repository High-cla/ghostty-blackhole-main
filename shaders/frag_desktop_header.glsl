// frag_desktop_header.glsl  standalone header for full blackhole.glsl
#version 330

uniform vec3 iResolution;
uniform float iTime;
uniform vec4 iDate;
uniform sampler2D iChannel0;

uniform vec4 iCurrentCursorColor = vec4(0.0);
uniform vec4 iPreviousCursorColor = vec4(0.0);
uniform float iTimeCursorChange = 0.0;
uniform vec4 iMouse = vec4(0.0);

// GUI-adjustable uniforms (negative = use shader default)
uniform float uHoleRadius = -1.0;
uniform float uDiskGain   = -1.0;
uniform float uDiskTemp   = -1.0;
uniform float uExposure   = -1.0;
uniform float uSpeed      = -1.0;
uniform float uStarGain   = -1.0;
uniform float uDiskIncl   = -1.0;

#define fragColor gl_FragColor