#ifndef ESDEF_H
#define ESDEF_H

///
// Types
//
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef ESboolean
#define ESboolean GLboolean
#endif

///
// Constant
//
#define ES_PI 3.141592653589793
#define ES_TWOPI 6.2831853071795862
#define ES_TWOPI_F 6.2831853f
#define ES_TO_RADIANS(x) (x * 0.017453292519943295)
#define ES_TO_DEGREES(x) (x * 57.29577951308232)

///
//  Macros
//
#define ESUTIL_API
#define ESCALLBACK


/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

#endif // ESDEF_H
