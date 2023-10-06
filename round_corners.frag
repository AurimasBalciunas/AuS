#version 310 es
precision highp float;
layout(binding = 0) uniform sampler2D src;

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

void main() {
    vec4 sampled = texture(src, qt_TexCoord0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Red
    fragColor = vec4(qt_TexCoord0.x, qt_TexCoord0.y, 0.0, 1.0);
    fragColor = vec4(sampled.rgb, 1.0);
    fragColor = vec4(qt_TexCoord0.x, qt_TexCoord0.y, 0.0, 1.0);


//    vec4 tex = texture(src, qt_TexCoord0);
//    if (tex.r > 0.5) {
//        fragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Output red if sampled red > 0.5
//    } else {
//        fragColor = vec4(0.0, 0.0, 1.0, 1.0);  // Output blue otherwise
//    }
//    vec4 tex = texture(src, qt_TexCoord0);
//    vec2 center = vec2(0.5, 0.5);
//    float radius = 0.5;

//    if (distance(qt_TexCoord0, center) > radius) {
//        discard;
//    }
//    else {
//        if (qt_TexCoord0.x < 0.5) {
//            fragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red
//            vec4 debugColor = texture(src, qt_TexCoord0);
//            if (debugColor.a == 0.0) {
//                fragColor = vec4(1.0, 1.0, 0.0, 1.0);  // Yellow for debug
//            }
//        } else {
//            fragColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue
//        }
//    }
}


// Red blue test
//#version 310 es
//precision highp float;

//layout(binding = 0) uniform sampler2D src;

//layout(location = 0) in vec2 qt_TexCoord0;
//layout(location = 0) out vec4 fragColor;

//void main() {
//    vec4 tex = texture(src, qt_TexCoord0);

//    if (qt_TexCoord0.x < 0.5) {
//        fragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red
//    } else {
//        fragColor = vec4(0.0, 0.0, 1.0, 1.0); // Blue
//    }
//}



// GRAYSCALE TEST
//#version 310 es

//precision highp float;

//layout(binding = 0) uniform sampler2D src;

//layout(location = 0) in vec2 qt_TexCoord0;
//layout(location = 0) out vec4 fragColor;

//void main() {
//vec4 color = texture(src, qt_TexCoord0);
//float gray = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
//fragColor = vec4(vec3(gray), color.a);
//}

//#version 310 es

//precision highp float;

//layout(binding = 0) uniform sampler2D src;

//layout(location = 0) in vec2 qt_TexCoord0;
//layout(location = 0) out vec4 fragColor;

//void main() {
//    vec2 centeredCoord = qt_TexCoord0 - vec2(0.5, 0.5);

//    // Check if the current pixel is outside the square defined by (-0.5, 0.5) for x and y
//    if (abs(centeredCoord.x) > 0.5 || abs(centeredCoord.y) > 0.5) {
//        fragColor = vec4(0.0, 0.0, 0.0, 0.0);  // Transparent pixel
//    } else {
//        fragColor = texture(src, qt_TexCoord0);
//    }
//}



//#version 310 es

//precision highp float;

//layout(binding = 0) uniform sampler2D src;

//layout(location = 0) in vec2 qt_TexCoord0;
//layout(location = 0) out vec4 fragColor;

//void main() {
//    vec2 centeredCoord = qt_TexCoord0 - vec2(0.5, 0.5);
//    float dist = length(centeredCoord);

//    if (dist > 0.5) {
//        fragColor = vec4(0.0, 0.0, 0.0, 0.0); // transparent corners
//    } else {
//        fragColor = texture(src, qt_TexCoord0);
//    }
//}






//INITIAL ROUNDED CORNER ATTEMPT
//#version 310 es

//precision highp float;

//layout(binding = 0) uniform lowp sampler2D src;

//layout(location = 0) in highp vec2 qt_TexCoord0;
//layout(location = 0) out lowp vec4 fragColor;

//const float cornerRadius = 0.2;

//void main() {
//    // Coordinate transformations to center origin
//    vec2 centeredCoord = qt_TexCoord0 - vec2(0.5, 0.5);
//    centeredCoord = abs(centeredCoord);

//    // Check distance to the corner
//    float dist = length(centeredCoord) - 0.5 + cornerRadius;

//    // Sample texture
//    lowp vec4 tex = texture(src, qt_TexCoord0);

//    // Apply corner rounding
//    if (dist > 0.0) {
//        fragColor = vec4(0.0, 0.0, 0.0, 0.0);
//    } else {
//        fragColor = tex;
//    }
//}
