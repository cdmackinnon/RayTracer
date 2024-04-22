# RayTracer


## Description
This is a ray tracer program that casts rays to render photogenic spheres. The program reads sphere data from a specification file listing dimensions (spheres.txt), calculates the intersection points of the rays with the spheres, handles shading and shadows of spheres on each other, and outputs the rendered image in PPM format.

## How to Compile
To compile the ray tracer use the following command:
```
gcc rt.c vec.c light.o -o raytracer -lm
```

## How to Execute
To execute the ray tracer program with a coordinate file titled "spheres.txt", use the following command:
```
./raytracer < spheres.txt > output.ppm
```


## Example Output
<div align="center">
  <img src="ExampleOutput.jpg" alt="Example Output">
</div>
