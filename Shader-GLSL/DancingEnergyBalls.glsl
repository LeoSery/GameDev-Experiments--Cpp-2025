/**
 * @brief Creates a triangular wave pattern between -1 and 1
 * @param x Input value to create wave from
 * @return Triangular wave value
 */
float triangularWave(float x)
{
    float a = fract(x) * 2.0 - 1.0;
    return abs(a);
}

/**
 * @brief Sets up the UV coordinate frame with proper aspect ratio
 * @param fragCoord Raw pixel coordinates from Shadertoy
 * @param ratio Output parameter for the screen ratio
 * @return Normalized UV coordinates adjusted for aspect ratio
 */
vec2 ComputeCoordFrame(vec2 fragCoord, out float ratio)
{
    vec2 uv = fragCoord/iResolution.xy;
    uv = uv * 2.0 - vec2(1.0);
    
   ratio = iResolution.x / iResolution.y;
    uv.x *= ratio;
    return uv;
}

/**
 * @brief Generates a DVD-like bouncing motion within screen boundaries
 * @param t Time parameter for animation
 * @param ratio Screen aspect ratio to handle bounds
 * @param radius Radius of the moving object to keep it fully on screen
 * @return 2D position for the bouncing motion
 */
vec2 DvdMotion(float t, float ratio, float radius)
{
    // Calculate boundaries accounting for object radius
    float rangeX = ratio - radius;
    float rangeY = 1.0 - radius;
    
    // Create bouncing motion using triangular waves
    // Note: 0.9 factor creates slightly different periods for X and Y
    float px = (triangularWave(t) * 2.0 - 1.0) * rangeX;
    float py = (triangularWave(t * 0.9) * 2.0 - 1.0) * rangeY;
    
    return vec2(px, py);
}

/**
 * @brief Generates a Lissajous curve motion
 * @param t Time parameter for animation
 * @return 2D position on the Lissajous curve
 * @note Uses ratio of 0.84 to create an interesting non-repeating pattern
 */
vec2 Lissajous(float t)
{
    return vec2(sin(t), cos(t * 0.84));
}

/**
 * @brief Main shader entry point
 * @param fragColor Output color for the current pixel
 * @param fragCoord Input pixel coordinates
 * 
 * This shader combines two animated light sources:
 * 1. A bouncing point light that moves like a DVD screensaver
 * 2. A point light following a Lissajous curve
 * 
 * The final output is a binary (black/white) image where pixels
 * are white when they are close enough to either light source.
 */
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float ratio;
    vec2  uv = ComputeCoordFrame(fragCoord, ratio);
    
    // DVD bouncing motion
    float radius = 0.3;
    vec2  dvdPosition = DvdMotion(iTime * 0.2, ratio, 0.3);
    float dvdIntensity = 1.0 / (1.0 + length(uv - dvdPosition)); 
    
    // Lissajous curve motion
    vec2  lissajousPosition = Lissajous(iTime);
    float curveIntensity = 1.0 / (1.0 + length(uv - lissajousPosition)); 
    
    // Combine both light sources with a threshold
    // step() creates a sharp transition at threshold 1.2
    float finalIntensity = step(1.2, (dvdIntensity + curveIntensity));
 
    // Output white where combined intensity exceeds threshold
    fragColor = vec4(finalIntensity, finalIntensity, finalIntensity, 1.0);
}