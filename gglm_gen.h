// supporting macros
#define CAT2(a, b) a##b
#define CAT(a, b) CAT2(a, b)
#define SQUARE(a) a * a
// name of current vector being implemented
#define VEC CAT(v, N)
#define NEW(x, y, z, w) (VEC){{x, y IF3(, z) IF4(, w)}}
#define CALL(fnname) CAT(VEC, CAT(_, fnname))

// fancy vector struct
#define VECTOR_UNION()\
    typedef union VEC {\
        struct { float x, y IF3(, z) IF4(, w); };\
        IF3(v2 xy;)\
        IF4(v3 xyz;)\
        float ptr[N];\
    } VEC;

// function definition macros
// expands to: static inline vN vN_opname(vN a, vN b)
#define FUNC(opname)\
    static inline VEC CAT(VEC, CAT(_, opname))

// non-vector-returning function
#define FUNC_NV(rettype, opname)\
    static inline rettype CAT(VEC, CAT(_, opname))

// function declaration for new vec
#define FN_NEW_VEC()\
    FUNC() (float x, float y IF3(, float z) IF4(, float w)) {\
        return NEW(x, y, z, w);\
    }

#define FN_FILL()\
    FUNC(fill) (float x) {\
        return NEW(x, x, x, x);\
    }

#define FN_COPY()\
    FUNC(copy) (float *p) {\
        return NEW(p[0], p[1], p[2], p[3]);\
    }

// elementwise basic operations
#define FN_BASIC(opname, op)\
    FUNC(opname) (VEC a, VEC b) {\
        return NEW(a.x op b.x, a.y op b.y, a.z op b.z, a.w op b.w);\
    }

// operations that take a vector and scalar
#define FN_SCALAR(opname, op)\
    FUNC(opname) (VEC v, float s) {\
        return NEW(v.x op s, v.y op s, v.z op s, v.w op s);\
    }

// specific functions
#define FN_NEGATE()\
    FUNC(negate) (VEC v) {\
        return NEW(-v.x, -v.y, -v.z, -v.w);\
    }

#define FN_DOT()\
    FUNC_NV(float, dot) (VEC a, VEC b) {\
        return a.x * b.x + a.y * b.y IF3(+ a.z * b.z) IF4(+ a.w * b.w);\
    }

#define FN_NORM2()\
    FUNC_NV(float, norm2) (VEC v) {\
        return CALL(dot)(v, v);\
    }

#define FN_NORM()\
    FUNC_NV(float, norm) (VEC v) {\
        return sqrtf(CALL(norm2)(v));\
    }

#define FN_DIST2()\
    FUNC_NV(float, dist2) (VEC a, VEC b) {\
        return CALL(norm2)(CALL(sub)(b, a));\
    }

#define FN_DIST()\
    FUNC_NV(float, dist) (VEC a, VEC b) {\
        return sqrtf(CALL(dist2)(a, b));\
    }

#define FN_NORMALIZE()\
    FUNC(normalize) (VEC v) {\
        return CALL(divs)(v, CALL(norm)(v));\
    }

#define FN_LERP()\
    FUNC(lerp) (VEC a, VEC b, float t) {\
        return CALL(add)(a, CALL(muls)(CALL(sub)(b, a), t));\
    }

#define FN_UPPER_BOUND()\
    FUNC(upper_bound) (VEC a, VEC b) {\
        return NEW(\
            GGLM_MAX(a.x, b.x),\
            GGLM_MAX(a.y, b.y),\
            GGLM_MAX(a.z, b.z),\
            GGLM_MAX(a.w, b.w)\
        );\
    }

#define FN_LOWER_BOUND()\
    FUNC(lower_bound) (VEC a, VEC b) {\
        return NEW(\
            GGLM_MIN(a.x, b.x),\
            GGLM_MIN(a.y, b.y),\
            GGLM_MIN(a.z, b.z),\
            GGLM_MIN(a.w, b.w)\
        );\
    }

#define FN_CLAMP()\
    FUNC(clamp) (VEC v, VEC lower, VEC upper) {\
        return NEW(\
            GGLM_CLAMP(v.x, lower.x, upper.x),\
            GGLM_CLAMP(v.y, lower.y, upper.y),\
            GGLM_CLAMP(v.z, lower.z, upper.z),\
            GGLM_CLAMP(v.w, lower.w, upper.w)\
        );\
    }

#define FN_EQUALS()\
    FUNC_NV(bool, eq) (VEC a, VEC b) {\
        return CALL(all)(CALL(sub)(a, b), GGLM_EPSILON > fabsf);\
    }

// implementation
#define IMPL()\
    FN_NEW_VEC()\
    FN_FILL()\
    FN_COPY()\
    FN_BASIC(add, +)\
    FN_BASIC(sub, -)\
    FN_BASIC(mul, *)\
    FN_BASIC(div, /)\
    FN_SCALAR(adds, +)\
    FN_SCALAR(subs, -)\
    FN_SCALAR(muls, *)\
    FN_SCALAR(divs, /)\
    FN_NEGATE()\
    FN_DOT()\
    FN_NORM2()\
    FN_NORM()\
    FN_DIST2()\
    FN_DIST()\
    FN_NORMALIZE()\
    FN_LERP()\
    FN_UPPER_BOUND()\
    FN_LOWER_BOUND()\
    FN_CLAMP()\
    FN_EQUALS()

// vector unions
#define N 2
#define IF3(...)
#define IF4(...)
VECTOR_UNION()
#define N 3
#define IF3(...) __VA_ARGS__
VECTOR_UNION()
#define N 4
#define IF4(...) __VA_ARGS__
VECTOR_UNION()

// functions
#define N 2
#define IF3(...)
#define IF4(...)
IMPL()

#define N 3
#define IF3(...) __VA_ARGS__
IMPL()

#define N 4
#define IF4(...) __VA_ARGS__
IMPL()
