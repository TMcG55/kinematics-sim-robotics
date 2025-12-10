# Robotic Simulation

A simple project demonstrating 3D 6DOF robot animation. The aim of this development is to build a full simulation for a variety of robotics from the ground up using homemade libraries, and visualisation using openGL.

<p align="center">
  <img src="https://github.com/user-attachments/assets/9c4afb7e-7411-41a5-aee6-01f793380330" />
</p>


## Features

- Animates a 2-link planar robot arm.

## Work in progress
- Inverse kinematics
- Path planning
- Inertial considerations

### Build

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
