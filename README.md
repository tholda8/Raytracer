# Raytracer
This raytracer simulates the physical behavior of light bouncing around a 3D scene. By tracing rays from the camera through the pixels and into the scene, it computes accurate lighting, shadows, and material interactions.
Done with [RayTracingInOneWeekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

<img width="1339" height="1340" alt="Snímek obrazovky 2026-07-21 211650" src="https://github.com/user-attachments/assets/c4b77b10-9684-4dc7-8e83-0ec4235237d2" />
<img width="1641" height="1295" alt="Snímek obrazovky 2026-07-21 225537" src="https://github.com/user-attachments/assets/98599823-e51c-4814-9139-7a0f5ff69c72" />


Here are the key features implemented in this project:

Camera & Rendering
Monte Carlo Path Tracing: Realistic global illumination.

Anti-aliasing: Multisampled pixels for smooth edges.

Depth of Field (Defocus Blur): Simulating a physical camera lens with a configurable aperture and focus distance.

Background Color: Skybox gradients or solid background colors.

Geometries & Volumes
Basic Shapes: Spheres and Rectangles (axis-aligned planes: XY, XZ, YZ).

Complex Shapes: 3D Boxes (constructed from rectangles).

Volumetric Rendering: Constant density mediums to simulate smoke, fog, or clouds.

Instances & Transformations: Translation and Y-axis rotation of objects.

Materials
Lambertian (Diffuse): Matte surfaces that scatter light randomly.

Metal: Reflective surfaces with adjustable "fuzziness".

Dielectric: Transparent materials like glass, water, or diamonds (simulating Snell's Law and the Schlick approximation for reflectivity).

Diffuse Light (Emissive): Materials that emit light, acting as light sources in the scene (e.g., Cornell Box lighting).
