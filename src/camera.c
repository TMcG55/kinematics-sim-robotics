#include "camera.h"
#include <math.h>
#include <string.h>

static float projection[16];
static float view[16];

// Helper functions
static void normalize(float v[3]) {
    float len = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if(len == 0.0f) return;
    v[0] /= len; v[1] /= len; v[2] /= len;
}

static void cross(const float a[3], const float b[3], float r[3]) {
    r[0] = a[1]*b[2] - a[2]*b[1];
    r[1] = a[2]*b[0] - a[0]*b[2];
    r[2] = a[0]*b[1] - a[1]*b[0];
}

static void subtract(const float a[3], const float b[3], float r[3]) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
}

// ---------------- Camera functions ----------------

void camera_init(float fov, float aspect, float near, float far) {
    float f = 1.0f / tanf(fov * 0.5f * (float)M_PI / 180.0f);

    memset(projection, 0, sizeof(projection));
    projection[0] = f / aspect;
    projection[5] = f;
    projection[10] = (far + near) / (near - far);
    projection[11] = -1.0f;
    projection[14] = (2.0f * far * near) / (near - far);
}

void camera_look_at(float eyeX, float eyeY, float eyeZ,
                    float targetX, float targetY, float targetZ,
                    float upX, float upY, float upZ) {
    float f[3] = { targetX - eyeX, targetY - eyeY, targetZ - eyeZ };
    normalize(f);

    float up[3] = { upX, upY, upZ };
    normalize(up);

    float s[3];
    cross(f, up, s);
    normalize(s);

    float u[3];
    cross(s, f, u);

    // Column-major view matrix
    memset(view, 0, sizeof(view));
    view[0] = s[0]; view[1] = u[0]; view[2] = -f[0]; view[3] = 0.0f;
    view[4] = s[1]; view[5] = u[1]; view[6] = -f[1]; view[7] = 0.0f;
    view[8] = s[2]; view[9] = u[2]; view[10] = -f[2]; view[11] = 0.0f;
    view[12] = -(s[0]*eyeX + s[1]*eyeY + s[2]*eyeZ);
    view[13] = -(u[0]*eyeX + u[1]*eyeY + u[2]*eyeZ);
    view[14] = f[0]*eyeX + f[1]*eyeY + f[2]*eyeZ;
    view[15] = 1.0f;
}

void camera_get_projection(float out[16]) {
    for(int i=0;i<16;i++) out[i] = projection[i];
}

void camera_get_view(float out[16]) {
    for(int i=0;i<16;i++) out[i] = view[i];
}