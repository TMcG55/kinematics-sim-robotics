#ifndef CAMERA_H
#define CAMERA_H

// Initialize camera projection (fov in degrees)
void camera_init(float fov, float aspect, float near, float far);

// Set the camera view using a look-at (eye, target, up)
void camera_look_at(float eyeX, float eyeY, float eyeZ,
                    float targetX, float targetY, float targetZ,
                    float upX, float upY, float upZ);

// Get the current projection matrix (16 floats, column-major)
void camera_get_projection(float out[16]);

// Get the current view matrix (16 floats, column-major)
void camera_get_view(float out[16]);

#endif
