# gglm

gglm is a knockoff of the vector parts of [cglm](https://github.com/recp/cglm). cglm is a great library, gglm just addresses some of the parts I found annoying to work with, and takes advantage of some really convenient modern C11 features.

## getting started

gglm is single-header, just copy it into your include folder and you're good to go!

## why?

a small example of the cglm code I was writing before, vs the equivalent gglm code:

```c
// --- cglm ---
vec3 a = {10.0, 10.0, 10.0};
vec3 b = {5.0, -5.0, 2.0};

// find the midpoint
vec3 midpoint;

glm_vec2_add(a, b, midpoint);
glm_vec2_divs(midpoint, 2.0, midpoint);

printf("(%f, %f, %f)\n", midpoint[0], midpoint[1], midpoint[2]);

// --- gglm ---
v3 a = v2_fill(10.0);
v3 b = v2_(5.0, -5.0, 2.0);

// find the midpoint
v3 midpoint = v3_divs(v3_add(a, b), 2.0);

printf("(%f, %f, %f)\n", v3_EXPAND(midpoint));
```

## reference

*`N` refers to size of vector in this reference. all functions use syntax `vN_fnname` (to add two `v4`s, call `v4_add`).*

#### vector union

vector unions look like this:

```c
typedef union vN {
    struct {
        float x, y, z, w;
    };
    v2 xy; // if v3 or v4
    v3 xyz; // if v4
    float ptr[N];
} vN;
```

this allows you to access the elements like this:

```c
v4 v = v4_(1.0, 2.0, 4.0, 3.0);

float x = v.x;
float y = v.y;

v2 xy = v.xy;
v3 xyz = v.xyz;

float *ptr = v.ptr; // equivalent to float *ptr = (float *)&v;
```

#### utilities

`gglm_eq(float a, float b)`

whether two floats are equivalent with epsilon

#### functions that apply to all vector types

`vN_(float x, float y ...)`

initializer, equivalent to `(vN){{x, y, ...}}`

`copy(float *)`

copies `N` floats from pointer and returns vector

`fill(float x)`

returns filled vector with single value

`add` `sub` `mul` `div`

element-wise operations.

`adds` `subs` `muls` `divs`

apply an operation between a vector and a scalar for each vector element

`negate`

negate all vector elements

`dot`

dot product

`norm2` `norm`

norm (magnitude) of a vector, and norm squared

`dist2` `dist`

distance between two vectors, and distance squared

`normalize`

normalized vector

`lerp(vN a, vN b, float t)`

returns interpolated value between a and b

`upper_bound` `lower_bound`

given two vectors, finds maximum/minimum of all elements

`clamp(vN v, vN lower, vN upper)`

clamp a vector within bounding box defined by a lower and upper bound

`eq`

whether two vectors are equal, with epsilon

#### macros

`ANY(vN, func)` `ALL(vN, func)`

allows for functional-like boolean stuff.

`EXPAND`

expands vector out (`v2` to `v.x, v.y`) for function calls, printf, etc.
