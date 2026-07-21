# Raytracer
This raytracer simulates the physical behavior of light bouncing around a 3D scene. By tracing rays from the camera through the pixels and into the scene, it computes accurate lighting, shadows, and material interactions.


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
