// RUN: %clang_cc1 -triple x86_64-unknown-unknown -w -S -o - -emit-llvm              %s | FileCheck %s -check-prefix=NO__ERRNO
// RUN: %clang_cc1 -triple x86_64-unknown-unknown -w -S -o - -emit-llvm -fmath-errno %s | FileCheck %s -check-prefix=HAS_ERRNO

// Test math, complex, and other builtin attributes to see how errno affects the resulting codegen. 


double *d;
float f;
float *fp;
long double *l;
int *i;
const char *c;

void foo() {
  __builtin_abs(f);       __builtin_labs(f);      __builtin_llabs(f);

// NO__ERRNO-NOT: .abs
// NO__ERRNO-NOT: .labs
// NO__ERRNO-NOT: .llabs
// NO__ERRNO-NOT: @abs
// NO__ERRNO-NOT: @labs
// NO__ERRNO-NOT: @llabs
// HAS_ERRNO-NOT: .abs
// HAS_ERRNO-NOT: .labs
// HAS_ERRNO-NOT: .llabs
// HAS_ERRNO-NOT: @abs
// HAS_ERRNO-NOT: @labs
// HAS_ERRNO-NOT: @llabs

  __builtin_atan2(f,f);    __builtin_atan2f(f,f) ;  __builtin_atan2l(f, f);

// NO__ERRNO: declare double @atan2(double, double) [[READNONE:#[0-9]+]]
// NO__ERRNO: declare float @atan2f(float, float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @atan2l(x86_fp80, x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @atan2(double, double) [[READNONE:#[0-9]+]]
// HAS_ERRNO: declare float @atan2f(float, float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @atan2l(x86_fp80, x86_fp80) [[READNONE]]

  __builtin_copysign(f,f); __builtin_copysignf(f,f);__builtin_copysignl(f,f);

// NO__ERRNO: declare double @llvm.copysign.f64(double, double) [[READNONE_INTRINSIC:#[0-9]+]]
// NO__ERRNO: declare float @llvm.copysign.f32(float, float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.copysign.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.copysign.f64(double, double) [[READNONE_INTRINSIC:#[0-9]+]]
// HAS_ERRNO: declare float @llvm.copysign.f32(float, float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.copysign.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_fabs(f);       __builtin_fabsf(f);      __builtin_fabsl(f);

// NO__ERRNO: declare double @llvm.fabs.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.fabs.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.fabs.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.fabs.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.fabs.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.fabs.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_fmod(f,f);     __builtin_fmodf(f,f);    __builtin_fmodl(f,f);

// NO__ERRNO-NOT: .fmod
// NO__ERRNO-NOT: @fmod
// HAS_ERRNO-NOT: .fmod
// HAS_ERRNO-NOT: @fmod

  __builtin_frexp(f,i);    __builtin_frexpf(f,i);   __builtin_frexpl(f,i);

// NO__ERRNO: declare double @frexp(double, i32*) [[NOT_READNONE:#[0-9]+]]
// NO__ERRNO: declare float @frexpf(float, i32*) [[NOT_READNONE]]
// NO__ERRNO: declare x86_fp80 @frexpl(x86_fp80, i32*) [[NOT_READNONE]]
// HAS_ERRNO: declare double @frexp(double, i32*) [[NOT_READNONE:#[0-9]+]]
// HAS_ERRNO: declare float @frexpf(float, i32*) [[NOT_READNONE]]
// HAS_ERRNO: declare x86_fp80 @frexpl(x86_fp80, i32*) [[NOT_READNONE]]

  __builtin_huge_val();    __builtin_huge_valf();   __builtin_huge_vall();

// NO__ERRNO-NOT: .huge
// NO__ERRNO-NOT: @huge
// HAS_ERRNO-NOT: .huge
// HAS_ERRNO-NOT: @huge

  __builtin_inf();    __builtin_inff();   __builtin_infl();

// NO__ERRNO-NOT: .inf
// NO__ERRNO-NOT: @inf
// HAS_ERRNO-NOT: .inf
// HAS_ERRNO-NOT: @inf

  __builtin_ldexp(f,f);    __builtin_ldexpf(f,f);   __builtin_ldexpl(f,f);  

// NO__ERRNO: declare double @ldexp(double, i32) [[READNONE]]
// NO__ERRNO: declare float @ldexpf(float, i32) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @ldexpl(x86_fp80, i32) [[READNONE]]
// HAS_ERRNO: declare double @ldexp(double, i32) [[READNONE]]
// HAS_ERRNO: declare float @ldexpf(float, i32) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @ldexpl(x86_fp80, i32) [[READNONE]]

  __builtin_modf(f,d);       __builtin_modff(f,fp);      __builtin_modfl(f,l); 

// NO__ERRNO: declare double @modf(double, double*) [[NOT_READNONE]]
// NO__ERRNO: declare float @modff(float, float*) [[NOT_READNONE]]
// NO__ERRNO: declare x86_fp80 @modfl(x86_fp80, x86_fp80*) [[NOT_READNONE]]
// HAS_ERRNO: declare double @modf(double, double*) [[NOT_READNONE]]
// HAS_ERRNO: declare float @modff(float, float*) [[NOT_READNONE]]
// HAS_ERRNO: declare x86_fp80 @modfl(x86_fp80, x86_fp80*) [[NOT_READNONE]]

  __builtin_nan(c);        __builtin_nanf(c);       __builtin_nanl(c);  

// NO__ERRNO: declare double @nan(i8*) [[READNONE]]
// NO__ERRNO: declare float @nanf(i8*) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @nanl(i8*) [[READNONE]]
// HAS_ERRNO: declare double @nan(i8*) [[READNONE:#[0-9]+]]
// HAS_ERRNO: declare float @nanf(i8*) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @nanl(i8*) [[READNONE]]

  __builtin_nans(c);        __builtin_nansf(c);       __builtin_nansl(c);  

// NO__ERRNO: declare double @nans(i8*) [[READNONE]]
// NO__ERRNO: declare float @nansf(i8*) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @nansl(i8*) [[READNONE]]
// HAS_ERRNO: declare double @nans(i8*) [[READNONE]]
// HAS_ERRNO: declare float @nansf(i8*) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @nansl(i8*) [[READNONE]]

  __builtin_pow(f,f);        __builtin_powf(f,f);       __builtin_powl(f,f);

// NO__ERRNO: declare double @llvm.pow.f64(double, double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.pow.f32(float, float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.pow.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.pow.f64(double, double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.pow.f32(float, float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.pow.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_powi(f,f);        __builtin_powif(f,f);       __builtin_powil(f,f);

// NO__ERRNO: declare double @llvm.powi.f64(double, i32) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.powi.f32(float, i32) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.powi.f80(x86_fp80, i32) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.powi.f64(double, i32) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.powi.f32(float, i32) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.powi.f80(x86_fp80, i32) [[READNONE_INTRINSIC]]

  /* math */
  __builtin_acos(f);       __builtin_acosf(f);      __builtin_acosl(f);

// NO__ERRNO: declare double @acos(double) [[READNONE]]
// NO__ERRNO: declare float @acosf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @acosl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @acos(double) [[READNONE]]
// HAS_ERRNO: declare float @acosf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @acosl(x86_fp80) [[READNONE]]

  __builtin_acosh(f);      __builtin_acoshf(f);     __builtin_acoshl(f);  

// NO__ERRNO: declare double @acosh(double) [[READNONE]]
// NO__ERRNO: declare float @acoshf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @acoshl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @acosh(double) [[READNONE]]
// HAS_ERRNO: declare float @acoshf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @acoshl(x86_fp80) [[READNONE]]

  __builtin_asin(f);       __builtin_asinf(f);      __builtin_asinl(f); 

// NO__ERRNO: declare double @asin(double) [[READNONE]]
// NO__ERRNO: declare float @asinf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @asinl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @asin(double) [[READNONE]]
// HAS_ERRNO: declare float @asinf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @asinl(x86_fp80) [[READNONE]]

  __builtin_asinh(f);      __builtin_asinhf(f);     __builtin_asinhl(f);

// NO__ERRNO: declare double @asinh(double) [[READNONE]]
// NO__ERRNO: declare float @asinhf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @asinhl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @asinh(double) [[READNONE]]
// HAS_ERRNO: declare float @asinhf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @asinhl(x86_fp80) [[READNONE]]

  __builtin_atan(f);       __builtin_atanf(f);      __builtin_atanl(f);

// NO__ERRNO: declare double @atan(double) [[READNONE]]
// NO__ERRNO: declare float @atanf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @atanl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @atan(double) [[READNONE]]
// HAS_ERRNO: declare float @atanf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @atanl(x86_fp80) [[READNONE]]

  __builtin_atanh(f);      __builtin_atanhf(f);     __builtin_atanhl(f); 

// NO__ERRNO: declare double @atanh(double) [[READNONE]]
// NO__ERRNO: declare float @atanhf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @atanhl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @atanh(double) [[READNONE]]
// HAS_ERRNO: declare float @atanhf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @atanhl(x86_fp80) [[READNONE]]

  __builtin_cbrt(f);       __builtin_cbrtf(f);      __builtin_cbrtl(f);

// NO__ERRNO: declare double @cbrt(double) [[READNONE]]
// NO__ERRNO: declare float @cbrtf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @cbrtl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @cbrt(double) [[READNONE]]
// HAS_ERRNO: declare float @cbrtf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @cbrtl(x86_fp80) [[READNONE]]

  __builtin_ceil(f);       __builtin_ceilf(f);      __builtin_ceill(f);

// NO__ERRNO: declare double @llvm.ceil.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.ceil.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.ceil.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.ceil.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.ceil.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.ceil.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_cos(f);        __builtin_cosf(f);       __builtin_cosl(f); 

// NO__ERRNO: declare double @cos(double) [[READNONE]]
// NO__ERRNO: declare float @cosf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @cosl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @cos(double) [[READNONE]]
// HAS_ERRNO: declare float @cosf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @cosl(x86_fp80) [[READNONE]]

  __builtin_cosh(f);       __builtin_coshf(f);      __builtin_coshl(f);

// NO__ERRNO: declare double @cosh(double) [[READNONE]]
// NO__ERRNO: declare float @coshf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @coshl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @cosh(double) [[READNONE]]
// HAS_ERRNO: declare float @coshf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @coshl(x86_fp80) [[READNONE]]

  __builtin_erf(f);        __builtin_erff(f);       __builtin_erfl(f);

// NO__ERRNO: declare double @erf(double) [[READNONE]]
// NO__ERRNO: declare float @erff(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @erfl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @erf(double) [[READNONE]]
// HAS_ERRNO: declare float @erff(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @erfl(x86_fp80) [[READNONE]]

  __builtin_erfc(f);       __builtin_erfcf(f);      __builtin_erfcl(f);

// NO__ERRNO: declare double @erfc(double) [[READNONE]]
// NO__ERRNO: declare float @erfcf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @erfcl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @erfc(double) [[READNONE]]
// HAS_ERRNO: declare float @erfcf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @erfcl(x86_fp80) [[READNONE]]

  __builtin_exp(f);        __builtin_expf(f);       __builtin_expl(f);

// NO__ERRNO: declare double @exp(double) [[READNONE]]
// NO__ERRNO: declare float @expf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @expl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @exp(double) [[READNONE]]
// HAS_ERRNO: declare float @expf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @expl(x86_fp80) [[READNONE]]

  __builtin_exp2(f);       __builtin_exp2f(f);      __builtin_exp2l(f); 

// NO__ERRNO: declare double @exp2(double) [[READNONE]]
// NO__ERRNO: declare float @exp2f(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @exp2l(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @exp2(double) [[READNONE]]
// HAS_ERRNO: declare float @exp2f(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @exp2l(x86_fp80) [[READNONE]]

  __builtin_expm1(f);      __builtin_expm1f(f);     __builtin_expm1l(f);

// NO__ERRNO: declare double @expm1(double) [[READNONE]]
// NO__ERRNO: declare float @expm1f(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @expm1l(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @expm1(double) [[READNONE]]
// HAS_ERRNO: declare float @expm1f(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @expm1l(x86_fp80) [[READNONE]]

  __builtin_fdim(f,f);       __builtin_fdimf(f,f);      __builtin_fdiml(f,f);

// NO__ERRNO: declare double @fdim(double, double) [[READNONE]]
// NO__ERRNO: declare float @fdimf(float, float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @fdiml(x86_fp80, x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @fdim(double, double) [[READNONE]]
// HAS_ERRNO: declare float @fdimf(float, float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @fdiml(x86_fp80, x86_fp80) [[READNONE]]

  __builtin_floor(f);      __builtin_floorf(f);     __builtin_floorl(f);

// NO__ERRNO: declare double @llvm.floor.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.floor.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.floor.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.floor.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.floor.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.floor.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_fma(f,f,f);        __builtin_fmaf(f,f,f);       __builtin_fmal(f,f,f);

// NO__ERRNO: declare double @llvm.fma.f64(double, double, double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.fma.f32(float, float, float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.fma.f80(x86_fp80, x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.fma.f64(double, double, double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.fma.f32(float, float, float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.fma.f80(x86_fp80, x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_fmax(f,f);       __builtin_fmaxf(f,f);      __builtin_fmaxl(f,f);

// NO__ERRNO: declare double @llvm.maxnum.f64(double, double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.maxnum.f32(float, float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.maxnum.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.maxnum.f64(double, double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.maxnum.f32(float, float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.maxnum.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_fmin(f,f);       __builtin_fminf(f,f);      __builtin_fminl(f,f);

// NO__ERRNO: declare double @llvm.minnum.f64(double, double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.minnum.f32(float, float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.minnum.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.minnum.f64(double, double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.minnum.f32(float, float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.minnum.f80(x86_fp80, x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_hypot(f,f);      __builtin_hypotf(f,f);     __builtin_hypotl(f,f);

// NO__ERRNO: declare double @hypot(double, double) [[READNONE]]
// NO__ERRNO: declare float @hypotf(float, float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @hypotl(x86_fp80, x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @hypot(double, double) [[READNONE]]
// HAS_ERRNO: declare float @hypotf(float, float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @hypotl(x86_fp80, x86_fp80) [[READNONE]]

  __builtin_ilogb(f);      __builtin_ilogbf(f);     __builtin_ilogbl(f); 

// NO__ERRNO: declare i32 @ilogb(double) [[READNONE]]
// NO__ERRNO: declare i32 @ilogbf(float) [[READNONE]]
// NO__ERRNO: declare i32 @ilogbl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare i32 @ilogb(double) [[READNONE]]
// HAS_ERRNO: declare i32 @ilogbf(float) [[READNONE]]
// HAS_ERRNO: declare i32 @ilogbl(x86_fp80) [[READNONE]]

  __builtin_lgamma(f);     __builtin_lgammaf(f);    __builtin_lgammal(f);

// NO__ERRNO: declare double @lgamma(double) [[READNONE]]
// NO__ERRNO: declare float @lgammaf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @lgammal(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @lgamma(double) [[READNONE]]
// HAS_ERRNO: declare float @lgammaf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @lgammal(x86_fp80) [[READNONE]]

  __builtin_llrint(f);     __builtin_llrintf(f);    __builtin_llrintl(f);

// NO__ERRNO: declare i64 @llrint(double) [[READNONE]]
// NO__ERRNO: declare i64 @llrintf(float) [[READNONE]]
// NO__ERRNO: declare i64 @llrintl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare i64 @llrint(double) [[READNONE]]
// HAS_ERRNO: declare i64 @llrintf(float) [[READNONE]]
// HAS_ERRNO: declare i64 @llrintl(x86_fp80) [[READNONE]]

  __builtin_llround(f);    __builtin_llroundf(f);   __builtin_llroundl(f);

// NO__ERRNO: declare i64 @llround(double) [[READNONE]]
// NO__ERRNO: declare i64 @llroundf(float) [[READNONE]]
// NO__ERRNO: declare i64 @llroundl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare i64 @llround(double) [[READNONE]]
// HAS_ERRNO: declare i64 @llroundf(float) [[READNONE]]
// HAS_ERRNO: declare i64 @llroundl(x86_fp80) [[READNONE]]

  __builtin_log(f);        __builtin_logf(f);       __builtin_logl(f);

// NO__ERRNO: declare double @log(double) [[READNONE]]
// NO__ERRNO: declare float @logf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @logl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @log(double) [[READNONE]]
// HAS_ERRNO: declare float @logf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @logl(x86_fp80) [[READNONE]]

  __builtin_log10(f);      __builtin_log10f(f);     __builtin_log10l(f);

// NO__ERRNO: declare double @log10(double) [[READNONE]]
// NO__ERRNO: declare float @log10f(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @log10l(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @log10(double) [[READNONE]]
// HAS_ERRNO: declare float @log10f(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @log10l(x86_fp80) [[READNONE]]

  __builtin_log1p(f);      __builtin_log1pf(f);     __builtin_log1pl(f);

// NO__ERRNO: declare double @log1p(double) [[READNONE]]
// NO__ERRNO: declare float @log1pf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @log1pl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @log1p(double) [[READNONE]]
// HAS_ERRNO: declare float @log1pf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @log1pl(x86_fp80) [[READNONE]]

  __builtin_log2(f);       __builtin_log2f(f);      __builtin_log2l(f);

// NO__ERRNO: declare double @log2(double) [[READNONE]]
// NO__ERRNO: declare float @log2f(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @log2l(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @log2(double) [[READNONE]]
// HAS_ERRNO: declare float @log2f(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @log2l(x86_fp80) [[READNONE]]

  __builtin_logb(f);       __builtin_logbf(f);      __builtin_logbl(f);

// NO__ERRNO: declare double @logb(double) [[READNONE]]
// NO__ERRNO: declare float @logbf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @logbl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @logb(double) [[READNONE]]
// HAS_ERRNO: declare float @logbf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @logbl(x86_fp80) [[READNONE]]

  __builtin_lrint(f);      __builtin_lrintf(f);     __builtin_lrintl(f);

// NO__ERRNO: declare i64 @lrint(double) [[READNONE]]
// NO__ERRNO: declare i64 @lrintf(float) [[READNONE]]
// NO__ERRNO: declare i64 @lrintl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare i64 @lrint(double) [[READNONE]]
// HAS_ERRNO: declare i64 @lrintf(float) [[READNONE]]
// HAS_ERRNO: declare i64 @lrintl(x86_fp80) [[READNONE]]

  __builtin_lround(f);     __builtin_lroundf(f);    __builtin_lroundl(f);

// NO__ERRNO: declare i64 @lround(double) [[READNONE]]
// NO__ERRNO: declare i64 @lroundf(float) [[READNONE]]
// NO__ERRNO: declare i64 @lroundl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare i64 @lround(double) [[READNONE]]
// HAS_ERRNO: declare i64 @lroundf(float) [[READNONE]]
// HAS_ERRNO: declare i64 @lroundl(x86_fp80) [[READNONE]]

  __builtin_nearbyint(f);  __builtin_nearbyintf(f); __builtin_nearbyintl(f);

// NO__ERRNO: declare double @llvm.nearbyint.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.nearbyint.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.nearbyint.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.nearbyint.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.nearbyint.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.nearbyint.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_nextafter(f,f);  __builtin_nextafterf(f,f); __builtin_nextafterl(f,f);

// NO__ERRNO: declare double @nextafter(double, double) [[READNONE]]
// NO__ERRNO: declare float @nextafterf(float, float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @nextafterl(x86_fp80, x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @nextafter(double, double) [[READNONE]]
// HAS_ERRNO: declare float @nextafterf(float, float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @nextafterl(x86_fp80, x86_fp80) [[READNONE]]

  __builtin_nexttoward(f,f); __builtin_nexttowardf(f,f);__builtin_nexttowardl(f,f);

// NO__ERRNO: declare double @nexttoward(double, x86_fp80) [[READNONE]]
// NO__ERRNO: declare float @nexttowardf(float, x86_fp80) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @nexttowardl(x86_fp80, x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @nexttoward(double, x86_fp80) [[READNONE]]
// HAS_ERRNO: declare float @nexttowardf(float, x86_fp80) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @nexttowardl(x86_fp80, x86_fp80) [[READNONE]]

  __builtin_remainder(f,f);  __builtin_remainderf(f,f); __builtin_remainderl(f,f);

// NO__ERRNO: declare double @remainder(double, double) [[READNONE]]
// NO__ERRNO: declare float @remainderf(float, float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @remainderl(x86_fp80, x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @remainder(double, double) [[READNONE]]
// HAS_ERRNO: declare float @remainderf(float, float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @remainderl(x86_fp80, x86_fp80) [[READNONE]]

  __builtin_remquo(f,f,i);  __builtin_remquof(f,f,i); __builtin_remquol(f,f,i);

// NO__ERRNO: declare double @remquo(double, double, i32*) [[NOT_READNONE]]
// NO__ERRNO: declare float @remquof(float, float, i32*) [[NOT_READNONE]]
// NO__ERRNO: declare x86_fp80 @remquol(x86_fp80, x86_fp80, i32*) [[NOT_READNONE]]
// HAS_ERRNO: declare double @remquo(double, double, i32*) [[NOT_READNONE]]
// HAS_ERRNO: declare float @remquof(float, float, i32*) [[NOT_READNONE]]
// HAS_ERRNO: declare x86_fp80 @remquol(x86_fp80, x86_fp80, i32*) [[NOT_READNONE]]

  __builtin_rint(f);       __builtin_rintf(f);      __builtin_rintl(f);

// NO__ERRNO: declare double @llvm.rint.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.rint.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.rint.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.rint.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.rint.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.rint.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_round(f);      __builtin_roundf(f);     __builtin_roundl(f);

// NO__ERRNO: declare double @llvm.round.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.round.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.round.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.round.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.round.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.round.f80(x86_fp80) [[READNONE_INTRINSIC]]

  __builtin_scalbln(f,f);    __builtin_scalblnf(f,f);   __builtin_scalblnl(f,f);

// NO__ERRNO: declare double @scalbln(double, i64) [[READNONE]]
// NO__ERRNO: declare float @scalblnf(float, i64) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @scalblnl(x86_fp80, i64) [[READNONE]]
// HAS_ERRNO: declare double @scalbln(double, i64) [[READNONE]]
// HAS_ERRNO: declare float @scalblnf(float, i64) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @scalblnl(x86_fp80, i64) [[READNONE]]

  __builtin_scalbn(f,f);     __builtin_scalbnf(f,f);    __builtin_scalbnl(f,f);

// NO__ERRNO: declare double @scalbn(double, i32) [[READNONE]]
// NO__ERRNO: declare float @scalbnf(float, i32) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @scalbnl(x86_fp80, i32) [[READNONE]]
// HAS_ERRNO: declare double @scalbn(double, i32) [[READNONE]]
// HAS_ERRNO: declare float @scalbnf(float, i32) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @scalbnl(x86_fp80, i32) [[READNONE]]

  __builtin_sin(f);        __builtin_sinf(f);       __builtin_sinl(f);

// NO__ERRNO: declare double @sin(double) [[READNONE]]
// NO__ERRNO: declare float @sinf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @sinl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @sin(double) [[READNONE]]
// HAS_ERRNO: declare float @sinf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @sinl(x86_fp80) [[READNONE]]

  __builtin_sinh(f);       __builtin_sinhf(f);      __builtin_sinhl(f);

// NO__ERRNO: declare double @sinh(double) [[READNONE]]
// NO__ERRNO: declare float @sinhf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @sinhl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @sinh(double) [[READNONE]]
// HAS_ERRNO: declare float @sinhf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @sinhl(x86_fp80) [[READNONE]]

  __builtin_sqrt(f);       __builtin_sqrtf(f);      __builtin_sqrtl(f); 

// NO__ERRNO: declare double @sqrt(double) [[READNONE]]
// NO__ERRNO: declare float @sqrtf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @sqrtl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @sqrt(double) [[READNONE]]
// HAS_ERRNO: declare float @sqrtf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @sqrtl(x86_fp80) [[READNONE]]

  __builtin_tan(f);        __builtin_tanf(f);       __builtin_tanl(f);

// NO__ERRNO: declare double @tan(double) [[READNONE]]
// NO__ERRNO: declare float @tanf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @tanl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @tan(double) [[READNONE]]
// HAS_ERRNO: declare float @tanf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @tanl(x86_fp80) [[READNONE]]

  __builtin_tanh(f);       __builtin_tanhf(f);      __builtin_tanhl(f);

// NO__ERRNO: declare double @tanh(double) [[READNONE]]
// NO__ERRNO: declare float @tanhf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @tanhl(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @tanh(double) [[READNONE]]
// HAS_ERRNO: declare float @tanhf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @tanhl(x86_fp80) [[READNONE]]

  __builtin_tgamma(f);     __builtin_tgammaf(f);    __builtin_tgammal(f);

// NO__ERRNO: declare double @tgamma(double) [[READNONE]]
// NO__ERRNO: declare float @tgammaf(float) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @tgammal(x86_fp80) [[READNONE]]
// HAS_ERRNO: declare double @tgamma(double) [[READNONE]]
// HAS_ERRNO: declare float @tgammaf(float) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @tgammal(x86_fp80) [[READNONE]]

  __builtin_trunc(f);      __builtin_truncf(f);     __builtin_truncl(f);

// NO__ERRNO: declare double @llvm.trunc.f64(double) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare float @llvm.trunc.f32(float) [[READNONE_INTRINSIC]]
// NO__ERRNO: declare x86_fp80 @llvm.trunc.f80(x86_fp80) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare double @llvm.trunc.f64(double) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare float @llvm.trunc.f32(float) [[READNONE_INTRINSIC]]
// HAS_ERRNO: declare x86_fp80 @llvm.trunc.f80(x86_fp80) [[READNONE_INTRINSIC]]

  /* complex */

  __builtin_cabs(f);       __builtin_cabsf(f);      __builtin_cabsl(f);

// NO__ERRNO: declare double @cabs(double, double) [[READNONE]]
// NO__ERRNO: declare float @cabsf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @cabsl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare double @cabs(double, double) [[READNONE]]
// HAS_ERRNO: declare float @cabsf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @cabsl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_cacos(f);      __builtin_cacosf(f);     __builtin_cacosl(f);

// NO__ERRNO: declare { double, double } @cacos(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @cacosf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @cacosl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @cacos(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @cacosf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @cacosl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_cacosh(f);     __builtin_cacoshf(f);    __builtin_cacoshl(f);

// NO__ERRNO: declare { double, double } @cacosh(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @cacoshf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @cacoshl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @cacosh(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @cacoshf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @cacoshl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_carg(f);       __builtin_cargf(f);      __builtin_cargl(f);

// NO__ERRNO: declare double @carg(double, double) [[READNONE]]
// NO__ERRNO: declare float @cargf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare x86_fp80 @cargl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare double @carg(double, double) [[READNONE]]
// HAS_ERRNO: declare float @cargf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare x86_fp80 @cargl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_casin(f);      __builtin_casinf(f);     __builtin_casinl(f);

// NO__ERRNO: declare { double, double } @casin(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @casinf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @casinl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @casin(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @casinf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @casinl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_casinh(f);     __builtin_casinhf(f);    __builtin_casinhl(f); 

// NO__ERRNO: declare { double, double } @casinh(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @casinhf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @casinhl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @casinh(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @casinhf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @casinhl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_catan(f);      __builtin_catanf(f);     __builtin_catanl(f); 

// NO__ERRNO: declare { double, double } @catan(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @catanf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @catanl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @catan(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @catanf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @catanl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_catanh(f);     __builtin_catanhf(f);    __builtin_catanhl(f);

// NO__ERRNO: declare { double, double } @catanh(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @catanhf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @catanhl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @catanh(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @catanhf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @catanhl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_ccos(f);       __builtin_ccosf(f);      __builtin_ccosl(f);

// NO__ERRNO: declare { double, double } @ccos(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @ccosf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @ccosl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @ccos(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @ccosf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @ccosl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_ccosh(f);      __builtin_ccoshf(f);     __builtin_ccoshl(f);

// NO__ERRNO: declare { double, double } @ccosh(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @ccoshf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @ccoshl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @ccosh(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @ccoshf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @ccoshl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_cexp(f);       __builtin_cexpf(f);      __builtin_cexpl(f);

// NO__ERRNO: declare { double, double } @cexp(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @cexpf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @cexpl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @cexp(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @cexpf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @cexpl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_cimag(f);      __builtin_cimagf(f);     __builtin_cimagl(f);

// NO__ERRNO-NOT: .cimag
// NO__ERRNO-NOT: @cimag
// HAS_ERRNO-NOT: .cimag
// HAS_ERRNO-NOT: @cimag

  __builtin_conj(f);       __builtin_conjf(f);      __builtin_conjl(f);

// NO__ERRNO-NOT: .conj
// NO__ERRNO-NOT: @conj
// HAS_ERRNO-NOT: .conj
// HAS_ERRNO-NOT: @conj

  __builtin_clog(f);       __builtin_clogf(f);      __builtin_clogl(f);

// NO__ERRNO: declare { double, double } @clog(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @clogf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @clogl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @clog(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @clogf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @clogl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_cproj(f);      __builtin_cprojf(f);     __builtin_cprojl(f); 

// NO__ERRNO: declare { double, double } @cproj(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @cprojf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @cprojl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @cproj(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @cprojf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @cprojl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_cpow(f,f);       __builtin_cpowf(f,f);      __builtin_cpowl(f,f);

// NO__ERRNO: declare { double, double } @cpow(double, double, double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @cpowf(<2 x float>, <2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @cpowl({ x86_fp80, x86_fp80 }* byval align 16, { x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @cpow(double, double, double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @cpowf(<2 x float>, <2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @cpowl({ x86_fp80, x86_fp80 }* byval align 16, { x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_creal(f);      __builtin_crealf(f);     __builtin_creall(f);

// NO__ERRNO-NOT: .creal
// NO__ERRNO-NOT: @creal
// HAS_ERRNO-NOT: .creal
// HAS_ERRNO-NOT: @creal

  __builtin_csin(f);       __builtin_csinf(f);      __builtin_csinl(f);

// NO__ERRNO: declare { double, double } @csin(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @csinf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @csinl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @csin(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @csinf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @csinl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_csinh(f);      __builtin_csinhf(f);     __builtin_csinhl(f);

// NO__ERRNO: declare { double, double } @csinh(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @csinhf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @csinhl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @csinh(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @csinhf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @csinhl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_csqrt(f);      __builtin_csqrtf(f);     __builtin_csqrtl(f);  

// NO__ERRNO: declare { double, double } @csqrt(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @csqrtf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @csqrtl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @csqrt(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @csqrtf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @csqrtl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_ctan(f);       __builtin_ctanf(f);      __builtin_ctanl(f);

// NO__ERRNO: declare { double, double } @ctan(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @ctanf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @ctanl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @ctan(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @ctanf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @ctanl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]

  __builtin_ctanh(f);      __builtin_ctanhf(f);     __builtin_ctanhl(f); 

// NO__ERRNO: declare { double, double } @ctanh(double, double) [[READNONE]]
// NO__ERRNO: declare <2 x float> @ctanhf(<2 x float>) [[READNONE]]
// NO__ERRNO: declare { x86_fp80, x86_fp80 } @ctanhl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
// HAS_ERRNO: declare { double, double } @ctanh(double, double) [[READNONE]]
// HAS_ERRNO: declare <2 x float> @ctanhf(<2 x float>) [[READNONE]]
// HAS_ERRNO: declare { x86_fp80, x86_fp80 } @ctanhl({ x86_fp80, x86_fp80 }* byval align 16) [[NOT_READNONE]]
};


// NO__ERRNO: attributes [[READNONE]] = { {{.*}}readnone{{.*}} }
// NO__ERRNO: attributes [[READNONE_INTRINSIC]] = { {{.*}}readnone{{.*}} }
// NO__ERRNO: attributes [[NOT_READNONE]] = { nounwind "correctly{{.*}} }

// HAS_ERRNO: attributes [[READNONE]] = { {{.*}}readnone{{.*}} }
// HAS_ERRNO: attributes [[READNONE_INTRINSIC]] = { {{.*}}readnone{{.*}} }
// HAS_ERRNO: attributes [[NOT_READNONE]] = { nounwind "correctly{{.*}} }
