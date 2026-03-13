# GPU Particle System (Compute Shader)

A GPU particle system implemented using OpenGL compute shaders.  
Particles are updated entirely on the GPU using Shader Storage Buffer Objects (SSBOs) and rendered as textured points with alpha blending.

The system supports replaying the simulation with different particle counts each run.

## Demo

[Watch the video](assets/videos/Preview.mp4)

## Features

- GPU particle updates using compute shaders
- Particle data stored in SSBOs
- Position and alpha stored in separate buffers
- Textured particle rendering
- Alpha blending for transparency
- Replay system with variable particle counts
- Fully GPU-driven particle simulation

## How It Works

Particles are simulated on the GPU using a compute shader.  
Each thread updates a particle's state in parallel.

Particle attributes such as position and alpha are stored in Shader Storage Buffer Objects (SSBOs).  
After the compute shader updates the buffers, the particle data is used directly during rendering.

This avoids CPU-GPU synchronization and allows thousands of particles to be updated efficiently.

## Particle Data Layout

Particle attributes are stored in separate SSBOs.

```glsl
layout(std430, binding = 0) buffer Positions
{
	vec2 positions[];
};

layout(std430, binding = 1) buffer Velocities
{
	vec2 velocities[];
};

layout(std430, binding = 2) buffer Alphas
{
	float alphas[];
};
```

This layout allows efficient GPU memory access and parallel updates in the compute shader.

## Compute Shader Update

The compute shader updates particle state such as position and transparency.

Each invocation handles one particle.

```glsl
uint id = gl_GlobalInvocationID.x;

if (id >= numParticles)
{
	return;
}

positions[id] += velocities[id] * speed * dt;
```

## Rendering

Particles are rendered as textured points.

The rendering pipeline uses:

`glDrawArrays(GL_POINTS)`

Particles are textured and use alpha blending for transparency.

```cpp
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

## Replay System

The particle system supports restarting the simulation with different numbers of particles.

Each replay can spawn a new particle count, allowing experimentation with performance and visual density.

## Performance

Updating particles on the GPU provides several advantages:

- Massive parallel updates using compute shaders
- Reduced CPU workload
- Efficient memory access using SSBOs
- Scales well with large particle counts

## Build

Requirements:

- OpenGL 4.5+
- CMake
- GLFW
- GLEW

Build:

```bash
mkdir build
cd build
cmake ..
make
```