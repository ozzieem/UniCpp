const char *cubemap_files[6] = {
    "../../common/data/cube-room_010.png",
    "../../common/data/cube-room_100.png",
    "../../common/data/cube-room_110.png",
    "../../common/data/cube-room_101.png",
    "../../common/data/cube-room_011.png",
    "../../common/data/cube-room_001.png"};


// cube-room_010.png" - Left of window
// cube-room_100.png" - Right of window
// cube-room_110.png" - Roof
// cube-room_101.png" - Floor 
// cube-room_001.png" - Window
// cube-room_011.png" - Front of window

// -----------------------------------------------


float t = (1.0f + sqrtf(5.0f)) * 0.25f;
int num_faces = 20;
int num_verts = 60;

float positions[] = {
    // An icosahedron without shared vertices has 20*3 vertices
    -0.5, t, 0,
    -0.5, t, 0,
    -0.5, t, 0,
    -0.5, t, 0,
    -0.5, t, 0,

    0.5, t, 0,
    0.5, t, 0,
    0.5, t, 0,
    0.5, t, 0,
    0.5, t, 0,

    -0.5, -t, 0,
    -0.5, -t, 0,
    -0.5, -t, 0,
    -0.5, -t, 0,
    -0.5, -t, 0,

    0.5, -t, 0,
    0.5, -t, 0,
    0.5, -t, 0,
    0.5, -t, 0,
    0.5, -t, 0,

    0, -0.5, t,
    0, -0.5, t,
    0, -0.5, t,
    0, -0.5, t,
    0, -0.5, t,

    0, 0.5, t,
    0, 0.5, t,
    0, 0.5, t,
    0, 0.5, t,
    0, 0.5, t,

    0, -0.5, -t,
    0, -0.5, -t,
    0, -0.5, -t,
    0, -0.5, -t,
    0, -0.5, -t,

    0, 0.5, -t,
    0, 0.5, -t,
    0, 0.5, -t,
    0, 0.5, -t,
    0, 0.5, -t,

    t, 0, -0.5,
    t, 0, -0.5,
    t, 0, -0.5,
    t, 0, -0.5,
    t, 0, -0.5,

    t, 0, 0.5,
    t, 0, 0.5,
    t, 0, 0.5,
    t, 0, 0.5,
    t, 0, 0.5,

    -t, 0, -0.5,
    -t, 0, -0.5,
    -t, 0, -0.5,
    -t, 0, -0.5,
    -t, 0, -0.5,

    -t, 0, 0.5,
    -t, 0, 0.5,
    -t, 0, 0.5,
    -t, 0, 0.5,
    -t, 0, 0.5};

/* uv-coordinates:
*  Texture coordinates specify the point in the texture image that will
*  correspond to the vertex you are specifying them for. 
*  Think of a rectangular rubber sheet with your texture image printed on it, 
*  where the length of each side is normalized to the range 0-1. 
*  Now let's say you wanted to draw a triangle using that texture. 
*  You'd take 3 pins and place them in the rubber sheet in the positions 
*  of each of your desired texture coordinates. 
*  (Say [0, 0], [1, 0] and [1, 1]) then move those pins 
*  (without taking them out) to your desired vertex coordinates 
*  (Say [0, 0], [0.5, 0] and [1, 1]), so that the rubber sheet is 
*  stretched out and the image is distorted. 
*  That's basically how texture coordinates work. 
*/
float uv_coords[] =
    {
        0.072405, 0.144686,
        0.27133, 0.10033,
        0.481007, 0.129901,
        0.681948, 0.115116,
        0.928587, 0.107723,
        0.29888, 0.70651,
        0.449421, 0.736084,
        0.108695, 0.129901,
        0.847270, 0.699121,
        0.868776, 0.728691,
        0.576437, 0.721299,
        0.319044, 0.750869,
        0.548883, 0.736084,
        0.388265, 0.115116,
        0.397674, 0.713906,
        0.149690, 0.122508,
        0.347942, 0.115116,
        0.583157, 0.107723,
        0.780738, 0.100331,
        0.229663, 0.107723,
        0.33436, 0.68882,
        0.181276, 0.721299,
        0.379528, 0.721299,
        0.190684, 0.122508,
        0.358023, 0.736084,
        0.103319, 0.713906,
        0.24512, 0.70651,
        0.083157, 0.699121,
        0.31006, 0.10251,
        0.222942, 0.721299,
        0.772001, 0.728691,
        0.618776, 0.721299,
        0.745120, 0.721299,
        0.433292, 0.115116,
        0.598614, 0.713906,
        0.520657, 0.721299,
        0.650362, 0.736084,
        0.700765, 0.728691,
        0.878184, 0.107723,
        0.671867, 0.743476,
        0.914475, 0.743476,
        0.819717, 0.736084,
        0.201437, 0.721299,
        0.633561, 0.122508,
        0.796195, 0.743476,
        0.140953, 0.728691,
        0.122136, 0.706514,
        0.262593, 0.721299,
        0.157754, 0.728691,
        0.825765, 0.115116,
        0.723614, 0.728691,
        0.897001, 0.721299,
        0.499152, 0.736084,
        0.731007, 0.115116,
        0.469582, 0.721299,
        0.045523, 0.691729,
        0.967566, 0.721299,
        0.28341, 0.67157,
        0.532754, 0.122508,
        0.418507, 0.706514,
};

int num_indices = num_faces * 3;
unsigned short indices[] = {
    // ... and 20 triangular faces,
    // defined by the following vertex indices:
    0, 55, 25,
    1, 26, 5,
    2, 6, 35,
    3, 36, 50,
    4, 51, 56,
    7, 27, 45,
    28, 57, 20,
    58, 52, 10,
    53, 37, 30,
    38, 8, 40,
    15, 46, 21,
    16, 22, 11,
    17, 12, 31,
    18, 32, 41,
    19, 42, 47,
    23, 48, 29,
    13, 24, 59,
    33, 14, 54,
    43, 34, 39,
    49, 44, 9};
