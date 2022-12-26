#ifdef GL_ES
precision mediump float;
#endif

uniform vec2  u_resolution;
uniform vec2  u_rectPos;     // position of the rectangle
uniform vec2  u_rectSize;    // size of the rectangle
uniform vec4  u_color;       // color of the rectangle
uniform float u_borderWidth; // width of the border in screen space
uniform float u_dashLength;  // length of each dash in the border
uniform float u_time;        // time in seconds

float rectShape(vec2 st, vec2 center, vec2 size) {
	vec2 d = abs(st - center) - size;
	return vec2(step(d.x, 0.0), step(d.y, 0.0)).x * vec2(step(d.x, 0.0), step(d.y, 0.0)).y;
}

float border(vec2 st, vec2 center, vec2 size, float borderWidth, float dashLength) {
	vec2  d    = abs(st - center) - size;
	float dist = length(max(d, vec2(0.0)));
	float dash = mod(dist, dashLength * 2.0);
	return step(borderWidth, dist) * step(dashLength, dash);
}

void main() {
	vec2 st    = gl_FragCoord.xy / u_resolution;
	vec4 color = vec4(0.0);

	// draw the rectangle
	float rect = rectShape(st, u_rectPos, u_rectSize);
	color      = mix(color, u_color, rect);

	// draw the border
	float border = border(st, u_rectPos, u_rectSize, u_borderWidth, u_dashLength);
	color        = mix(color, u_color, border);

	gl_FragColor = color;
}
