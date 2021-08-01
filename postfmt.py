#!/usr/bin/env python
import sys

"""
post formatting because clang-format isn't perfect
removes # lines, spaces out functions, and adds header guard
"""

fname = sys.argv[1]
text = ""

with open(fname, 'r') as f:
    for line in f.readlines():
        if line[0] != '#':
            text += line

            if line[0] == '}' or line.startswith("const"):
                text += "\n"

text = """\
#ifndef GGLM_H
#define GGLM_H

// this header is c11-only, this macro should works GCC (and maybe clang?)
#ifdef __STDC_VERSION__
#if __STDC_VERSION__ < 201112L
#error "gglm is only compatible with c11 onwards."
#endif
#endif

#include <math.h>
#include <stdbool.h>

#define GGLM_EPSILON 0.000001

#define GGLM_MAX(a, b) ((a) > (b) ? (a) : (b))
#define GGLM_MIN(a, b) ((a) < (b) ? (a) : (b))
#define GGLM_CLAMP(x, lo, hi) GGLM_MIN(GGLM_MAX(x, lo), hi)

// map(), any(), all() are super useful for expressive programming
#define v2_map(v, func) (v2){{func(v.x), func(v.y)}}
#define v3_map(v, func) (v3){{func(v.x), func(v.y), func(v.z)}}
#define v4_map(v, func) (v4){{func(v.x), func(v.y), func(v.z), func(v.w)}}

/*
a cool trick with any() and all() is to use conditionals, for example:
> v4_any(v, 0.0 >) // checks if zero is more than any of vector values
you can even combine them with functions (this is the v4_eq implementation!):
> v4_all(v4_sub(a, b), GGLM_EPSILON > fabsf)
*/
#define v2_any(v, func) (func(v.x) || func(v.y))
#define v3_any(v, func) (func(v.x) || func(v.y) || func(v.z))
#define v4_any(v, func) (func(v.x) || func(v.y) || func(v.z) || func(v.w))

#define v2_all(v, func) (func(v.x) && func(v.y))
#define v3_all(v, func) (func(v.x) && func(v.y) && func(v.z))
#define v4_all(v, func) (func(v.x) && func(v.y) && func(v.z) && func(v.w))

/*
these are great for printf's, expanding vectors into float arrays, or using
functions that weren't built for gglm:
> printf("v: %f %f\\n", v2_expand(v));
> float vectors[] = {v2_expand(a), v3_expand(b), v2_expand(c)};
*/
#define v2_expand(v) v.x, v.y
#define v3_expand(v) v.x, v.y, v.z
#define v4_expand(v) v.x, v.y, v.z, v.w

// --- macro-expansion generation below this point ---
""" + text + """\
#endif
"""

with open(fname, 'w') as f:
    f.write(text)
