/* 
*Computer Organization 304, W&M, May the 4th Spring 2023 
*Author: Connor MacKinnon
*Assignment 06: Ray Caster and Ray Tracer
*Description:
*This is a Ray Caster for spheres. Spheres can be input as (X,Y,Z,radius,R,G,B).
*rt.c is a ray tracer supporting lighting functionality.
*Prints output to be read in a .PPM file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rc.h"
#include "vec.h"
#include "light.h"

#define WIDTH  1000
#define HEIGHT 1000

/*
* Reads in inputs as objects containing spheres and colors
* These objects have a next pointer and make a linked list 
*/
void read_objs(OBJ_T **list) {
    *list = NULL; // set list to null initially
    float ctrx, ctry, ctrz;
    float R, G, B;
    float radius;

    while (scanf("%f %f %f %f %f %f %f", &ctrx, &ctry, &ctrz, &radius, &R, &G, &B ) == 7) {
        // creates a new object in the linked list
        OBJ_T *node = (OBJ_T *) malloc(sizeof(OBJ_T));
        node->sphere.center.x = ctrx;
        node->sphere.center.y = ctry;
        node->sphere.center.z = ctrz;
        node->sphere.radius = radius;

        COLOR_T color;
        color.R = R;
        color.G = G;
        color.B = B;

        node->color = color;

        //adds the node to the linked list, adjusts list head
        node->next = *list;
        *list = node;
    }
}

/*
* Takes in a source ray, a sphere, and intersection pointer.
* Uses the ray and the quadratic formula to calculate the intersection points of the ray.
* The smaller intersection will be pointed to by *t to represent the closer side of the sphere.
* There are several edge cases for being inside of the sphere or in front of it.
*/
int intersect_sphere(RAY_T ray, SPHERE_T sphere, double *t) {
    double A = 1.0;
    double B = 2.0 * (ray.direction.x * (ray.origin.x - sphere.center.x) +
                      ray.direction.y * (ray.origin.y - sphere.center.y) +
                      ray.direction.z * (ray.origin.z - sphere.center.z));
    double C = pow(ray.origin.x - sphere.center.x, 2) +
               pow(ray.origin.y - sphere.center.y, 2) +
               pow(ray.origin.z - sphere.center.z, 2) -
               pow(sphere.radius, 2);

    double discriminant = pow(B, 2) - (4 * A * C);
    // no intersection
    if (discriminant <= 0.0) {
        return 0; 
    }

    //both quadratic answers
    double plus = (-B + sqrt(discriminant)) / (2 * A);
    double minus = (-B - sqrt(discriminant)) / (2 * A);

    //Edge Cases:
    // intersection behind camera
    if (plus < 0.0 && minus < 0.0) {
        return 0;
    }
    //If camera is inside the sphere and one intersect is shown
    if (plus < 0.0) {
        *t = minus;
        return 1;
    }
    if (minus < 0.0) {
        *t = plus;
        return 1;
    }
    //returns the closest intersection point in t
    *t = fmin(plus, minus);
    return 1;
}

COLOR_T cast(RAY_T ray, OBJ_T *list) {
    // Initializing background color to black
    COLOR_T bg_color;
    bg_color.R = 0;
    bg_color.G = 0;
    bg_color.B = 0;

    // Track the closest intersection point with min_t, sphere may be no further than 1000 units away
    double t;
    double min_t = 1000;
    OBJ_T *closest_obj = NULL; // Track the closest object
    VEC_T int_pt;
    VEC_T normal;

    LIGHT_T light;
    light.loc.x = 5;
    light.loc.y = 10;
    light.loc.z = 0;

    // Iterate through all objects in the list using pointer for navigation
    for (OBJ_T* pointer = list; pointer != NULL; pointer = pointer->next) {
        // Check for intersection with the object
        if (intersect_sphere(ray, pointer->sphere, &t)) {
            // If closer than previous intersection, update closest_obj, min_t, and intersection point
            if (t < min_t) {
                closest_obj = pointer;
                min_t = t;
                int_pt.x = ray.origin.x + t * ray.direction.x;
                int_pt.y = ray.origin.y + t * ray.direction.y;
                int_pt.z = ray.origin.z + t * ray.direction.z;

                normal.x = int_pt.x - pointer->sphere.center.x;
                normal.y = int_pt.y - pointer->sphere.center.y;
                normal.z = int_pt.z - pointer->sphere.center.z;

                normal = normalize(normal);
            }
        }
    }

    // If no intersection found, return background color
    if (closest_obj == NULL) {
        return bg_color;
    }

    // Cast shadow ray towards the light source
    RAY_T shadow_ray;
    shadow_ray.origin = int_pt;
    shadow_ray.direction.x = light.loc.x - int_pt.x;
    shadow_ray.direction.y = light.loc.y - int_pt.y;
    shadow_ray.direction.z = light.loc.z - int_pt.z;
    shadow_ray.direction = normalize(shadow_ray.direction);


    // Check if intersection point is in shadow
    for (OBJ_T* pointer = list; pointer != NULL; pointer = pointer->next) {
        if (pointer != closest_obj) {
            if (intersect_sphere(shadow_ray, pointer->sphere, &t)) {
                // If there's an intersection with another object, intersection point is in shadow
                return bg_color; // Return background color
            }
        }
    }

    // If not in shadow, calculate illumination
    return illuminate(ray, light, closest_obj->color, int_pt, normal);
}


/*
* Runs all the previous functions to get the color at each pixel of the PPM. 
* Prints the output that is ready for the PPM and then frees the memory that the 
* linked list consumed.
*/
int main(void) {
    OBJ_T *list = NULL;
    read_objs(&list);
    RAY_T ray;

    printf( "P6 %d %d 255\n", WIDTH, HEIGHT);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            //translates the ray so that it is relative to the middle
            ray.direction.x = -0.5 + (x/1000.0);
            ray.direction.y = 0.5 - (y/1000.0);
            ray.direction.z = 1;
            ray.origin.x = 0;
            ray.origin.y = 0;
            ray.origin.z = 0;
            ray.direction = normalize(ray.direction);
            COLOR_T color = cast(ray, list);
            printf("%c%c%c", (unsigned char)(color.R * 255),
                             (unsigned char)(color.G * 255),
                             (unsigned char)(color.B * 255));
    }
}

    // Free up the linked list memory
    OBJ_T *head = list;
    while (head != NULL) {
        OBJ_T *next = head->next;
        free(head);
        head = next;
    }

    return 0;
}