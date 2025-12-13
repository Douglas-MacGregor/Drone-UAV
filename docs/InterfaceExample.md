# Interface Example

## Circle

```C
typedef struct {
    double (*area)(void *self);       // function to compute area
    double (*perimeter)(void *self);  // function to compute perimeter
} ShapeInterface;

typedef struct {
    ShapeInterface *vtable;  // pointer to interface
    double radius;
} Circle;

// Implementation of the interface functions
double circle_area(void *self) {
    Circle *c = (Circle *)self;
    return 3.14159 * c->radius * c->radius;
}

double circle_perimeter(void *self) {
    Circle *c = (Circle *)self;
    return 2 * 3.14159 * c->radius;
}

ShapeInterface circle_vtable = {
    .area = circle_area,
    .perimeter = circle_perimeter
};

void init_circle(Circle *c, double radius) {
    c->vtable = &circle_vtable;
    c->radius = radius;
}

#include <stdio.h>

int main() {
    Circle c;
    init_circle(&c, 5.0);
    printf("Area: %f\n", c.vtable->area(&c));
    printf("Perimeter: %f\n", c.vtable->perimeter(&c));

    return 0;
}

```
