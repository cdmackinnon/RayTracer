# RayTracer


## Description
This is a simple ray tracer program that casts rays to render spheres. The program reads sphere data from a file (spheres.txt), calculates the intersection points of the rays with the spheres, handles shading and shadows, and outputs the rendered image in PPM format.

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
