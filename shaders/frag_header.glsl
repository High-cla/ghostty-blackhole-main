// ===== Standalone header (no Ghostty-specific uniforms) ======
#version 330 compatibility

uniform vec3 iResolution;
uniform float iTime;
uniform vec4 iDate;

// Dummy uniforms for Ghostty variables referenced in shader body
uniform vec4 iCurrentCursorColor;
uniform vec4 iPreviousCursorColor;

// Use built-in gl_FragColor (compatibility profile)
out vec4 fragColor;