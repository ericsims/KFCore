/** @file miniblas.c
 * KFCore
 * @author Jan Zwiener (jan@zwiener.org)
 *
 * @brief Minimal generic BLAS implementation
 * @{ */

/******************************************************************************
 * SYSTEM INCLUDE FILES
 ******************************************************************************/

#include <ctype.h> /* tolower() */

/******************************************************************************
 * PROJECT INCLUDE FILES
 ******************************************************************************/

#include "miniblas.h"

/******************************************************************************
 * DEFINES
 ******************************************************************************/

/******************************************************************************
 * TYPEDEFS
 ******************************************************************************/

/******************************************************************************
 * LOCAL DATA DEFINITIONS
 ******************************************************************************/

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 ******************************************************************************/

// inline int min(int a, int b) { return a < b ? a : b; }
static int max(int a, int b)
{
    return a > b ? a : b;
}

/******************************************************************************
 * FUNCTION BODIES
 ******************************************************************************/

int lsame_(const char* a, const char* b)
{
    return (tolower(*a) == tolower(*b));
}

int strsm_(const char* side, const char* uplo, const char* transa, const char* diag, int* m, const int* n,
          const float* alpha, const float* a, const int* lda, float* b, const int* ldb)
{
    int   i__, j, k, info;
    float temp;
    int   lside;
    int   nrowa;
    int   upper;
    int   nounit;
    int   a_dim1, a_offset, b_dim1, b_offset, i__1, i__2, i__3;

    /*  Purpose */
    /*  ======= */

    /*  STRSM  solves one of the matrix equations */

    /*     op( A )*X = alpha*B,   or   X*op( A ) = alpha*B, */

    /*  where alpha is a scalar, X and B are m by n matrices, A is a unit, or */
    /*  non-unit,  upper or lower triangular matrix  and  op( A )  is one  of */

    /*     op( A ) = A   or   op( A ) = A'. */

    /*  The matrix X is overwritten on B. */

    /*  Arguments */
    /*  ========== */

    /*  SIDE   - CHARACTER*1. */
    /*           On entry, SIDE specifies whether op( A ) appears on the left */
    /*           or right of X as follows: */
    /*              SIDE = 'L' or 'l'   op( A )*X = alpha*B. */
    /*              SIDE = 'R' or 'r'   X*op( A ) = alpha*B. */

    /*           Unchanged on exit. */

    /*  UPLO   - CHARACTER*1. */
    /*           On entry, UPLO specifies whether the matrix A is an upper or */
    /*           lower triangular matrix as follows: */
    /*              UPLO = 'U' or 'u'   A is an upper triangular matrix. */
    /*              UPLO = 'L' or 'l'   A is a lower triangular matrix. */
    /*           Unchanged on exit. */

    /*  TRANSA - CHARACTER*1. */
    /*           On entry, TRANSA specifies the form of op( A ) to be used in */
    /*           the matrix multiplication as follows: */
    /*              TRANSA = 'N' or 'n'   op( A ) = A. */
    /*              TRANSA = 'T' or 't'   op( A ) = A'. */
    /*              TRANSA = 'C' or 'c'   op( A ) = A'. */

    /*           Unchanged on exit. */

    /*  DIAG   - CHARACTER*1. */
    /*           On entry, DIAG specifies whether or not A is unit triangular */
    /*           as follows: */
    /*              DIAG = 'U' or 'u'   A is assumed to be unit triangular. */
    /*              DIAG = 'N' or 'n'   A is not assumed to be unit */
    /*                                  triangular. */

    /*           Unchanged on exit. */

    /*  M      - INTEGER. */
    /*           On entry, M specifies the number of rows of B. M must be at */
    /*           least zero. */
    /*           Unchanged on exit. */

    /*  N      - INTEGER. */
    /*           On entry, N specifies the number of columns of B.  N must be */
    /*           at least zero. */
    /*           Unchanged on exit. */

    /*  ALPHA  - REAL            . */
    /*           On entry,  ALPHA specifies the scalar  alpha. When  alpha is */
    /*           zero then  A is not referenced and  B need not be set before */
    /*           entry. */
    /*           Unchanged on exit. */

    /*  A      - REAL             array of DIMENSION ( LDA, k ), where k is m */
    /*           when  SIDE = 'L' or 'l'  and is  n  when  SIDE = 'R' or 'r'. */
    /*           Before entry  with  UPLO = 'U' or 'u',  the  leading  k by k */
    /*           upper triangular part of the array  A must contain the upper */
    /*           triangular matrix  and the strictly lower triangular part of */
    /*           A is not referenced. */
    /*           Before entry  with  UPLO = 'L' or 'l',  the  leading  k by k */
    /*           lower triangular part of the array  A must contain the lower */
    /*           triangular matrix  and the strictly upper triangular part of */
    /*           A is not referenced. */
    /*           Note that when  DIAG = 'U' or 'u',  the diagonal elements of */
    /*           A  are not referenced either,  but are assumed to be  unity. */
    /*           Unchanged on exit. */

    /*  LDA    - INTEGER. */
    /*           On entry, LDA specifies the first dimension of A as declared */
    /*           in the calling (sub) program.  When  SIDE = 'L' or 'l'  then */
    /*           LDA  must be at least  max( 1, m ),  when  SIDE = 'R' or 'r' */
    /*           then LDA must be at least max( 1, n ). */
    /*           Unchanged on exit. */

    /*  B      - REAL             array of DIMENSION ( LDB, n ). */
    /*           Before entry,  the leading  m by n part of the array  B must */
    /*           contain  the  right-hand  side  matrix  B,  and  on exit  is */
    /*           overwritten by the solution matrix  X. */

    /*  LDB    - INTEGER. */
    /*           On entry, LDB specifies the first dimension of B as declared */
    /*           in  the  calling  (sub)  program.   LDB  must  be  at  least */
    /*           max( 1, m ). */
    /*           Unchanged on exit. */

    /*  Level 3 Blas routine. */

    /*  -- Written on 8-February-1989. */
    /*     Jack Dongarra, Argonne National Laboratory. */
    /*     Iain Duff, AERE Harwell. */
    /*     Jeremy Du Croz, Numerical Algorithms Group Ltd. */
    /*     Sven Hammarling, Numerical Algorithms Group Ltd. */

    /* Parameter adjustments */
    a_dim1   = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1   = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;

    /* Function Body */
    lside = lsame_(side, "L");
    if (lside)
    {
        nrowa = *m;
    }
    else
    {
        nrowa = *n;
    }
    nounit = lsame_(diag, "N");
    upper  = lsame_(uplo, "U");

    info = 0;
    if (!lside && !lsame_(side, "R"))
    {
        info = 1;
    }
    else if (!upper && !lsame_(uplo, "L"))
    {
        info = 2;
    }
    else if (!lsame_(transa, "N") && !lsame_(transa, "T") && !lsame_(transa, "C"))
    {
        info = 3;
    }
    else if (!lsame_(diag, "U") && !lsame_(diag, "N"))
    {
        info = 4;
    }
    else if (*m < 0)
    {
        info = 5;
    }
    else if (*n < 0)
    {
        info = 6;
    }
    else if (*lda < max(1, nrowa))
    {
        info = 9;
    }
    else if (*ldb < max(1, *m))
    {
        info = 11;
    }
    if (info != 0)
    {
        return info;
    }
    if (*m == 0 || *n == 0)
    { /*     Quick return if possible. */
        return 0;
    }
    if (*alpha == 0.f)
    { /*     And when  alpha.eq.zero. */
        i__1 = *n;
        for (j = 1; j <= i__1; ++j)
        {
            i__2 = *m;
            for (i__ = 1; i__ <= i__2; ++i__)
            {
                b[i__ + j * b_dim1] = 0.f;
            }
        }
        return 0;
    }

    if (lside)
    {
        if (lsame_(transa, "N"))
        {
            /*           Form  B := alpha*inv( A )*B. */
            if (upper)
            {
                i__1 = *n;
                for (j = 1; j <= i__1; ++j)
                {
                    if (*alpha != 1.f)
                    {
                        i__2 = *m;
                        for (i__ = 1; i__ <= i__2; ++i__)
                        {
                            b[i__ + j * b_dim1] = *alpha * b[i__ + j * b_dim1];
                        }
                    }
                    for (k = *m; k >= 1; --k)
                    {
                        if (b[k + j * b_dim1] != 0.f)
                        {
                            if (nounit)
                            {
                                b[k + j * b_dim1] /= a[k + k * a_dim1];
                            }
                            i__2 = k - 1;
                            for (i__ = 1; i__ <= i__2; ++i__)
                            {
                                b[i__ + j * b_dim1] -= b[k + j * b_dim1] * a[i__ + k * a_dim1];
                            }
                        }
                    }
                }
            }
            else
            {
                i__1 = *n;
                for (j = 1; j <= i__1; ++j)
                {
                    if (*alpha != 1.f)
                    {
                        i__2 = *m;
                        for (i__ = 1; i__ <= i__2; ++i__)
                        {
                            b[i__ + j * b_dim1] = *alpha * b[i__ + j * b_dim1];
                        }
                    }
                    i__2 = *m;
                    for (k = 1; k <= i__2; ++k)
                    {
                        if (b[k + j * b_dim1] != 0.f)
                        {
                            if (nounit)
                            {
                                b[k + j * b_dim1] /= a[k + k * a_dim1];
                            }
                            i__3 = *m;
                            for (i__ = k + 1; i__ <= i__3; ++i__)
                            {
                                b[i__ + j * b_dim1] -= b[k + j * b_dim1] * a[i__ + k * a_dim1];
                            }
                        }
                    }
                }
            }
        }
        else
        {

            /*           Form  B := alpha*inv( A' )*B. */

            if (upper)
            {
                i__1 = *n;
                for (j = 1; j <= i__1; ++j)
                {
                    i__2 = *m;
                    for (i__ = 1; i__ <= i__2; ++i__)
                    {
                        temp = *alpha * b[i__ + j * b_dim1];
                        i__3 = i__ - 1;
                        for (k = 1; k <= i__3; ++k)
                        {
                            temp -= a[k + i__ * a_dim1] * b[k + j * b_dim1];
                        }
                        if (nounit)
                        {
                            temp /= a[i__ + i__ * a_dim1];
                        }
                        b[i__ + j * b_dim1] = temp;
                    }
                }
            }
            else
            {
                i__1 = *n;
                for (j = 1; j <= i__1; ++j)
                {
                    for (i__ = *m; i__ >= 1; --i__)
                    {
                        temp = *alpha * b[i__ + j * b_dim1];
                        i__2 = *m;
                        for (k = i__ + 1; k <= i__2; ++k)
                        {
                            temp -= a[k + i__ * a_dim1] * b[k + j * b_dim1];
                        }
                        if (nounit)
                        {
                            temp /= a[i__ + i__ * a_dim1];
                        }
                        b[i__ + j * b_dim1] = temp;
                    }
                }
            }
        }
    }
    else
    {
        if (lsame_(transa, "N"))
        {
            /*           Form  B := alpha*B*inv( A ). */
            if (upper)
            {
                i__1 = *n;
                for (j = 1; j <= i__1; ++j)
                {
                    if (*alpha != 1.f)
                    {
                        i__2 = *m;
                        for (i__ = 1; i__ <= i__2; ++i__)
                        {
                            b[i__ + j * b_dim1] = *alpha * b[i__ + j * b_dim1];
                        }
                    }
                    i__2 = j - 1;
                    for (k = 1; k <= i__2; ++k)
                    {
                        if (a[k + j * a_dim1] != 0.f)
                        {
                            i__3 = *m;
                            for (i__ = 1; i__ <= i__3; ++i__)
                            {
                                b[i__ + j * b_dim1] -= a[k + j * a_dim1] * b[i__ + k * b_dim1];
                            }
                        }
                    }
                    if (nounit)
                    {
                        temp = 1.f / a[j + j * a_dim1];
                        i__2 = *m;
                        for (i__ = 1; i__ <= i__2; ++i__)
                        {
                            b[i__ + j * b_dim1] = temp * b[i__ + j * b_dim1];
                        }
                    }
                }
            }
            else
            {
                for (j = *n; j >= 1; --j)
                {
                    if (*alpha != 1.f)
                    {
                        i__1 = *m;
                        for (i__ = 1; i__ <= i__1; ++i__)
                        {
                            b[i__ + j * b_dim1] = *alpha * b[i__ + j * b_dim1];
                        }
                    }
                    i__1 = *n;
                    for (k = j + 1; k <= i__1; ++k)
                    {
                        if (a[k + j * a_dim1] != 0.f)
                        {
                            i__2 = *m;
                            for (i__ = 1; i__ <= i__2; ++i__)
                            {
                                b[i__ + j * b_dim1] -= a[k + j * a_dim1] * b[i__ + k * b_dim1];
                            }
                        }
                    }
                    if (nounit)
                    {
                        temp = 1.f / a[j + j * a_dim1];
                        i__1 = *m;
                        for (i__ = 1; i__ <= i__1; ++i__)
                        {
                            b[i__ + j * b_dim1] = temp * b[i__ + j * b_dim1];
                        }
                    }
                }
            }
        }
        else /* Form  B := alpha*B*inv( A' ). */
        {
            if (upper)
            {
                for (k = *n; k >= 1; --k)
                {
                    if (nounit)
                    {
                        temp = 1.f / a[k + k * a_dim1];
                        i__1 = *m;
                        for (i__ = 1; i__ <= i__1; ++i__)
                        {
                            b[i__ + k * b_dim1] = temp * b[i__ + k * b_dim1];
                        }
                    }
                    i__1 = k - 1;
                    for (j = 1; j <= i__1; ++j)
                    {
                        if (a[j + k * a_dim1] != 0.f)
                        {
                            temp = a[j + k * a_dim1];
                            i__2 = *m;
                            for (i__ = 1; i__ <= i__2; ++i__)
                            {
                                b[i__ + j * b_dim1] -= temp * b[i__ + k * b_dim1];
                            }
                        }
                    }
                    if (*alpha != 1.f)
                    {
                        i__1 = *m;
                        for (i__ = 1; i__ <= i__1; ++i__)
                        {
                            b[i__ + k * b_dim1] = *alpha * b[i__ + k * b_dim1];
                        }
                    }
                }
            }
            else
            {
                i__1 = *n;
                for (k = 1; k <= i__1; ++k)
                {
                    if (nounit)
                    {
                        temp = 1.f / a[k + k * a_dim1];
                        i__2 = *m;
                        for (i__ = 1; i__ <= i__2; ++i__)
                        {
                            b[i__ + k * b_dim1] = temp * b[i__ + k * b_dim1];
                        }
                    }
                    i__2 = *n;
                    for (j = k + 1; j <= i__2; ++j)
                    {
                        if (a[j + k * a_dim1] != 0.f)
                        {
                            temp = a[j + k * a_dim1];
                            i__3 = *m;
                            for (i__ = 1; i__ <= i__3; ++i__)
                            {
                                b[i__ + j * b_dim1] -= temp * b[i__ + k * b_dim1];
                            }
                        }
                    }
                    if (*alpha != 1.f)
                    {
                        i__2 = *m;
                        for (i__ = 1; i__ <= i__2; ++i__)
                        {
                            b[i__ + k * b_dim1] = *alpha * b[i__ + k * b_dim1];
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int sgemm_(const char* transa, const char* transb, int* m, const int* n, const int* k, const float* alpha, const float* a,
          const int* lda, const float* b, const int* ldb, const float* beta, float* c__, const int* ldc)
{
    int a_dim1, a_offset, b_dim1, b_offset, c_dim1, c_offset, i__1, i__2, i__3;
    /* Local variables */
    int   i__, j, l, info;
    int   nota, notb;
    float temp;
    int   nrowa, nrowb;

    /*  Purpose */
    /*  ======= */

    /*  SGEMM  performs one of the matrix-matrix operations */

    /*     C := alpha*op( A )*op( B ) + beta*C, */

    /*  where  op( X ) is one of */

    /*     op( X ) = X   or   op( X ) = X', */

    /*  alpha and beta are scalars, and A, B and C are matrices, with op( A ) */
    /*  an m by k matrix,  op( B )  a  k by n matrix and  C an m by n matrix. */

    /*  Arguments */
    /*  ========== */

    /*  TRANSA - CHARACTER*1. */
    /*           On entry, TRANSA specifies the form of op( A ) to be used in */
    /*           the matrix multiplication as follows: */

    /*              TRANSA = 'N' or 'n',  op( A ) = A. */

    /*              TRANSA = 'T' or 't',  op( A ) = A'. */

    /*              TRANSA = 'C' or 'c',  op( A ) = A'. */

    /*           Unchanged on exit. */

    /*  TRANSB - CHARACTER*1. */
    /*           On entry, TRANSB specifies the form of op( B ) to be used in */
    /*           the matrix multiplication as follows: */

    /*              TRANSB = 'N' or 'n',  op( B ) = B. */

    /*              TRANSB = 'T' or 't',  op( B ) = B'. */

    /*              TRANSB = 'C' or 'c',  op( B ) = B'. */

    /*           Unchanged on exit. */

    /*  M      - INTEGER. */
    /*           On entry,  M  specifies  the number  of rows  of the  matrix */
    /*           op( A )  and of the  matrix  C.  M  must  be at least  zero. */
    /*           Unchanged on exit. */

    /*  N      - INTEGER. */
    /*           On entry,  N  specifies the number  of columns of the matrix */
    /*           op( B ) and the number of columns of the matrix C. N must be */
    /*           at least zero. */
    /*           Unchanged on exit. */

    /*  K      - INTEGER. */
    /*           On entry,  K  specifies  the number of columns of the matrix */
    /*           op( A ) and the number of rows of the matrix op( B ). K must */
    /*           be at least  zero. */
    /*           Unchanged on exit. */

    /*  ALPHA  - REAL            . */
    /*           On entry, ALPHA specifies the scalar alpha. */
    /*           Unchanged on exit. */

    /*  A      - REAL             array of DIMENSION ( LDA, ka ), where ka is */
    /*           k  when  TRANSA = 'N' or 'n',  and is  m  otherwise. */
    /*           Before entry with  TRANSA = 'N' or 'n',  the leading  m by k */
    /*           part of the array  A  must contain the matrix  A,  otherwise */
    /*           the leading  k by m  part of the array  A  must contain  the */
    /*           matrix A. */
    /*           Unchanged on exit. */

    /*  LDA    - INTEGER. */
    /*           On entry, LDA specifies the first dimension of A as declared */
    /*           in the calling (sub) program. When  TRANSA = 'N' or 'n' then */
    /*           LDA must be at least  max( 1, m ), otherwise  LDA must be at */
    /*           least  max( 1, k ). */
    /*           Unchanged on exit. */

    /*  B      - REAL             array of DIMENSION ( LDB, kb ), where kb is */
    /*           n  when  TRANSB = 'N' or 'n',  and is  k  otherwise. */
    /*           Before entry with  TRANSB = 'N' or 'n',  the leading  k by n */
    /*           part of the array  B  must contain the matrix  B,  otherwise */
    /*           the leading  n by k  part of the array  B  must contain  the */
    /*           matrix B. */
    /*           Unchanged on exit. */

    /*  LDB    - INTEGER. */
    /*           On entry, LDB specifies the first dimension of B as declared */
    /*           in the calling (sub) program. When  TRANSB = 'N' or 'n' then */
    /*           LDB must be at least  max( 1, k ), otherwise  LDB must be at */
    /*           least  max( 1, n ). */
    /*           Unchanged on exit. */

    /*  BETA   - REAL            . */
    /*           On entry,  BETA  specifies the scalar  beta.  When  BETA  is */
    /*           supplied as zero then C need not be set on input. */
    /*           Unchanged on exit. */

    /*  C      - REAL             array of DIMENSION ( LDC, n ). */
    /*           Before entry, the leading  m by n  part of the array  C must */
    /*           contain the matrix  C,  except when  beta  is zero, in which */
    /*           case C need not be set on entry. */
    /*           On exit, the array  C  is overwritten by the  m by n  matrix */
    /*           ( alpha*op( A )*op( B ) + beta*C ). */

    /*  LDC    - INTEGER. */
    /*           On entry, LDC specifies the first dimension of C as declared */
    /*           in  the  calling  (sub)  program.   LDC  must  be  at  least */
    /*           max( 1, m ). */
    /*           Unchanged on exit. */

    /*  Level 3 Blas routine. */

    /*  -- Written on 8-February-1989. */
    /*     Jack Dongarra, Argonne National Laboratory. */
    /*     Iain Duff, AERE Harwell. */
    /*     Jeremy Du Croz, Numerical Algorithms Group Ltd. */
    /*     Sven Hammarling, Numerical Algorithms Group Ltd. */

    /*     Set  NOTA  and  NOTB  as  true if  A  and  B  respectively are not */
    /*     transposed and set  NROWA, NCOLA and  NROWB  as the number of rows */
    /*     and  columns of  A  and the  number of  rows  of  B  respectively. */

    /* Parameter adjustments */
    a_dim1   = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1   = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    c_dim1   = *ldc;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;

    /* Function Body */
    nota = lsame_(transa, "N");
    notb = lsame_(transb, "N");
    if (nota)
    {
        nrowa = *m;
    }
    else
    {
        nrowa = *k;
    }
    if (notb)
    {
        nrowb = *k;
    }
    else
    {
        nrowb = *n;
    }

    /*     Test the input parameters. */
    info = 0;
    if (!nota && !lsame_(transa, "C") && !lsame_(transa, "T"))
    {
        info = 1;
    }
    else if (!notb && !lsame_(transb, "C") && !lsame_(transb, "T"))
    {
        info = 2;
    }
    else if (*m < 0)
    {
        info = 3;
    }
    else if (*n < 0)
    {
        info = 4;
    }
    else if (*k < 0)
    {
        info = 5;
    }
    else if (*lda < max(1, nrowa))
    {
        info = 8;
    }
    else if (*ldb < max(1, nrowb))
    {
        info = 10;
    }
    else if (*ldc < max(1, *m))
    {
        info = 13;
    }
    if (info != 0)
    {
        return info;
    }

    /*     Quick return if possible. */
    if (*m == 0 || *n == 0 || ((*alpha == 0.f || *k == 0) && *beta == 1.f))
    {
        return 0;
    }

    /*     And if  alpha.eq.zero. */
    if (*alpha == 0.f)
    {
        if (*beta == 0.f)
        {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__)
                {
                    c__[i__ + j * c_dim1] = 0.f;
                }
            }
        }
        else
        {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__)
                {
                    c__[i__ + j * c_dim1] = *beta * c__[i__ + j * c_dim1];
                }
            }
        }
        return 0;
    }

    if (notb)
    {
        if (nota)
        {

            /*           Form  C := alpha*A*B + beta*C. */

            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                if (*beta == 0.f)
                {
                    i__2 = *m;
                    for (i__ = 1; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = 0.f;
                    }
                }
                else if (*beta != 1.f)
                {
                    i__2 = *m;
                    for (i__ = 1; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = *beta * c__[i__ + j * c_dim1];
                    }
                }
                i__2 = *k;
                for (l = 1; l <= i__2; ++l)
                {
                    if (b[l + j * b_dim1] != 0.f)
                    {
                        temp = *alpha * b[l + j * b_dim1];
                        i__3 = *m;
                        for (i__ = 1; i__ <= i__3; ++i__)
                        {
                            c__[i__ + j * c_dim1] += temp * a[i__ + l * a_dim1];
                        }
                    }
                }
            }
        }
        else
        {

            /*           Form  C := alpha*A'*B + beta*C */

            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__)
                {
                    temp = 0.f;
                    i__3 = *k;
                    for (l = 1; l <= i__3; ++l)
                    {
                        temp += a[l + i__ * a_dim1] * b[l + j * b_dim1];
                    }
                    if (*beta == 0.f)
                    {
                        c__[i__ + j * c_dim1] = *alpha * temp;
                    }
                    else
                    {
                        c__[i__ + j * c_dim1] = *alpha * temp + *beta * c__[i__ + j * c_dim1];
                    }
                }
            }
        }
    }
    else
    {
        if (nota)
        {
            /*           Form  C := alpha*A*B' + beta*C */
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                if (*beta == 0.f)
                {
                    i__2 = *m;
                    for (i__ = 1; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = 0.f;
                    }
                }
                else if (*beta != 1.f)
                {
                    i__2 = *m;
                    for (i__ = 1; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = *beta * c__[i__ + j * c_dim1];
                    }
                }
                i__2 = *k;
                for (l = 1; l <= i__2; ++l)
                {
                    if (b[j + l * b_dim1] != 0.f)
                    {
                        temp = *alpha * b[j + l * b_dim1];
                        i__3 = *m;
                        for (i__ = 1; i__ <= i__3; ++i__)
                        {
                            c__[i__ + j * c_dim1] += temp * a[i__ + l * a_dim1];
                        }
                    }
                }
            }
        }
        else
        {

            /*           Form  C := alpha*A'*B' + beta*C */

            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__)
                {
                    temp = 0.f;
                    i__3 = *k;
                    for (l = 1; l <= i__3; ++l)
                    {
                        temp += a[l + i__ * a_dim1] * b[j + l * b_dim1];
                    }
                    if (*beta == 0.f)
                    {
                        c__[i__ + j * c_dim1] = *alpha * temp;
                    }
                    else
                    {
                        c__[i__ + j * c_dim1] = *alpha * temp + *beta * c__[i__ + j * c_dim1];
                    }
                }
            }
        }
    }

    return 0;
}

int ssyrk_(const char* uplo, const char* trans, int* n, const int* k, const float* alpha, const float* a, const int* lda, const float* beta,
          float* c__, const int* ldc)
{
    int a_dim1, a_offset, c_dim1, c_offset, i__1, i__2, i__3;

    /* Local variables */
    int   i__, j, l, info;
    float temp;
    int   nrowa;
    int   upper;

    /*  Purpose */
    /*  ======= */

    /*  SSYRK  performs one of the symmetric rank k operations */

    /*     C := alpha*A*A' + beta*C, */

    /*  or */

    /*     C := alpha*A'*A + beta*C, */

    /*  where  alpha and beta  are scalars, C is an  n by n  symmetric matrix */
    /*  and  A  is an  n by k  matrix in the first case and a  k by n  matrix */
    /*  in the second case. */

    /*  Arguments */
    /*  ========== */

    /*  UPLO   - CHARACTER*1. */
    /*           On  entry,   UPLO  specifies  whether  the  upper  or  lower */
    /*           triangular  part  of the  array  C  is to be  referenced  as */
    /*           follows: */

    /*              UPLO = 'U' or 'u'   Only the  upper triangular part of  C */
    /*                                  is to be referenced. */

    /*              UPLO = 'L' or 'l'   Only the  lower triangular part of  C */
    /*                                  is to be referenced. */

    /*           Unchanged on exit. */

    /*  TRANS  - CHARACTER*1. */
    /*           On entry,  TRANS  specifies the operation to be performed as */
    /*           follows: */

    /*              TRANS = 'N' or 'n'   C := alpha*A*A' + beta*C. */

    /*              TRANS = 'T' or 't'   C := alpha*A'*A + beta*C. */

    /*              TRANS = 'C' or 'c'   C := alpha*A'*A + beta*C. */

    /*           Unchanged on exit. */

    /*  N      - INTEGER. */
    /*           On entry,  N specifies the order of the matrix C.  N must be */
    /*           at least zero. */
    /*           Unchanged on exit. */

    /*  K      - INTEGER. */
    /*           On entry with  TRANS = 'N' or 'n',  K  specifies  the number */
    /*           of  columns   of  the   matrix   A,   and  on   entry   with */
    /*           TRANS = 'T' or 't' or 'C' or 'c',  K  specifies  the  number */
    /*           of rows of the matrix  A.  K must be at least zero. */
    /*           Unchanged on exit. */

    /*  ALPHA  - REAL            . */
    /*           On entry, ALPHA specifies the scalar alpha. */
    /*           Unchanged on exit. */

    /*  A      - REAL             array of DIMENSION ( LDA, ka ), where ka is */
    /*           k  when  TRANS = 'N' or 'n',  and is  n  otherwise. */
    /*           Before entry with  TRANS = 'N' or 'n',  the  leading  n by k */
    /*           part of the array  A  must contain the matrix  A,  otherwise */
    /*           the leading  k by n  part of the array  A  must contain  the */
    /*           matrix A. */
    /*           Unchanged on exit. */

    /*  LDA    - INTEGER. */
    /*           On entry, LDA specifies the first dimension of A as declared */
    /*           in  the  calling  (sub)  program.   When  TRANS = 'N' or 'n' */
    /*           then  LDA must be at least  max( 1, n ), otherwise  LDA must */
    /*           be at least  max( 1, k ). */
    /*           Unchanged on exit. */

    /*  BETA   - REAL            . */
    /*           On entry, BETA specifies the scalar beta. */
    /*           Unchanged on exit. */

    /*  C      - REAL             array of DIMENSION ( LDC, n ). */
    /*           Before entry  with  UPLO = 'U' or 'u',  the leading  n by n */
    /*           upper triangular part of the array C must contain the upper */
    /*           triangular part  of the  symmetric matrix  and the strictly */
    /*           lower triangular part of C is not referenced.  On exit, the */
    /*           upper triangular part of the array  C is overwritten by the */
    /*           upper triangular part of the updated matrix. */
    /*           Before entry  with  UPLO = 'L' or 'l',  the leading  n by n */
    /*           lower triangular part of the array C must contain the lower */
    /*           triangular part  of the  symmetric matrix  and the strictly */
    /*           upper triangular part of C is not referenced.  On exit, the */
    /*           lower triangular part of the array  C is overwritten by the */
    /*           lower triangular part of the updated matrix. */

    /*  LDC    - INTEGER. */
    /*           On entry, LDC specifies the first dimension of C as declared */
    /*           in  the  calling  (sub)  program.   LDC  must  be  at  least */
    /*           max( 1, n ). */
    /*           Unchanged on exit. */

    /*  Level 3 Blas routine. */

    /*  -- Written on 8-February-1989. */
    /*     Jack Dongarra, Argonne National Laboratory. */
    /*     Iain Duff, AERE Harwell. */
    /*     Jeremy Du Croz, Numerical Algorithms Group Ltd. */
    /*     Sven Hammarling, Numerical Algorithms Group Ltd. */

    /*     Test the input parameters. */

    /* Parameter adjustments */
    a_dim1   = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    c_dim1   = *ldc;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;

    /* Function Body */
    if (lsame_(trans, "N"))
    {
        nrowa = *n;
    }
    else
    {
        nrowa = *k;
    }
    upper = lsame_(uplo, "U");

    info = 0;
    if (!upper && !lsame_(uplo, "L"))
    {
        info = 1;
    }
    else if (!lsame_(trans, "N") && !lsame_(trans, "T") && !lsame_(trans, "C"))
    {
        info = 2;
    }
    else if (*n < 0)
    {
        info = 3;
    }
    else if (*k < 0)
    {
        info = 4;
    }
    else if (*lda < max(1, nrowa))
    {
        info = 7;
    }
    else if (*ldc < max(1, *n))
    {
        info = 10;
    }
    if (info != 0)
    {
        return info;
    }

    /*     Quick return if possible. */

    if (*n == 0 || ((*alpha == 0.f || *k == 0) && *beta == 1.f))
    {
        return 0;
    }

    /*     And when  alpha.eq.zero. */

    if (*alpha == 0.f)
    {
        if (upper)
        {
            if (*beta == 0.f)
            {
                i__1 = *n;
                for (j = 1; j <= i__1; ++j)
                {
                    i__2 = j;
                    for (i__ = 1; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = 0.f;
                    }
                }
            }
            else
            {
                i__1 = *n;
                for (j = 1; j <= i__1; ++j)
                {
                    i__2 = j;
                    for (i__ = 1; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = *beta * c__[i__ + j * c_dim1];
                    }
                }
            }
        }
        else
        {
            if (*beta == 0.f)
            {
                i__1 = *n;
                for (j = 1; j <= i__1; ++j)
                {
                    i__2 = *n;
                    for (i__ = j; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = 0.f;
                    }
                }
            }
            else
            {
                i__1 = *n;
                for (j = 1; j <= i__1; ++j)
                {
                    i__2 = *n;
                    for (i__ = j; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = *beta * c__[i__ + j * c_dim1];
                    }
                }
            }
        }
        return 0;
    }

    /*     Start the operations. */

    if (lsame_(trans, "N"))
    {

        /*        Form  C := alpha*A*A' + beta*C. */

        if (upper)
        {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                if (*beta == 0.f)
                {
                    i__2 = j;
                    for (i__ = 1; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = 0.f;
                    }
                }
                else if (*beta != 1.f)
                {
                    i__2 = j;
                    for (i__ = 1; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = *beta * c__[i__ + j * c_dim1];
                    }
                }
                i__2 = *k;
                for (l = 1; l <= i__2; ++l)
                {
                    if (a[j + l * a_dim1] != 0.f)
                    {
                        temp = *alpha * a[j + l * a_dim1];
                        i__3 = j;
                        for (i__ = 1; i__ <= i__3; ++i__)
                        {
                            c__[i__ + j * c_dim1] += temp * a[i__ + l * a_dim1];
                        }
                    }
                }
            }
        }
        else
        {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                if (*beta == 0.f)
                {
                    i__2 = *n;
                    for (i__ = j; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = 0.f;
                    }
                }
                else if (*beta != 1.f)
                {
                    i__2 = *n;
                    for (i__ = j; i__ <= i__2; ++i__)
                    {
                        c__[i__ + j * c_dim1] = *beta * c__[i__ + j * c_dim1];
                    }
                }
                i__2 = *k;
                for (l = 1; l <= i__2; ++l)
                {
                    if (a[j + l * a_dim1] != 0.f)
                    {
                        temp = *alpha * a[j + l * a_dim1];
                        i__3 = *n;
                        for (i__ = j; i__ <= i__3; ++i__)
                        {
                            c__[i__ + j * c_dim1] += temp * a[i__ + l * a_dim1];
                        }
                    }
                }
            }
        }
    }
    else
    {

        /*        Form  C := alpha*A'*A + beta*C. */

        if (upper)
        {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                i__2 = j;
                for (i__ = 1; i__ <= i__2; ++i__)
                {
                    temp = 0.f;
                    i__3 = *k;
                    for (l = 1; l <= i__3; ++l)
                    {
                        temp += a[l + i__ * a_dim1] * a[l + j * a_dim1];
                    }
                    if (*beta == 0.f)
                    {
                        c__[i__ + j * c_dim1] = *alpha * temp;
                    }
                    else
                    {
                        c__[i__ + j * c_dim1] = *alpha * temp + *beta * c__[i__ + j * c_dim1];
                    }
                }
            }
        }
        else
        {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                i__2 = *n;
                for (i__ = j; i__ <= i__2; ++i__)
                {
                    temp = 0.f;
                    i__3 = *k;
                    for (l = 1; l <= i__3; ++l)
                    {
                        temp += a[l + i__ * a_dim1] * a[l + j * a_dim1];
                    }
                    if (*beta == 0.f)
                    {
                        c__[i__ + j * c_dim1] = *alpha * temp;
                    }
                    else
                    {
                        c__[i__ + j * c_dim1] = *alpha * temp + *beta * c__[i__ + j * c_dim1];
                    }
                }
            }
        }
    }

    return 0;
}

int ssymm_(const char* side, const char* uplo, int* m, const int* n, const float* alpha, const float* a, const int* lda, const float* b,
          const int* ldb, const float* beta, float* c__, const int* ldc)
{
    int a_dim1, a_offset, b_dim1, b_offset, c_dim1, c_offset, i__1, i__2, i__3;
    /* Local variables */
    int   i__, j, k, info;
    float temp1, temp2;
    int   nrowa;
    int   upper;

    /*  Purpose */
    /*  ======= */

    /*  SSYMM  performs one of the matrix-matrix operations */

    /*     C := alpha*A*B + beta*C, */

    /*  or */

    /*     C := alpha*B*A + beta*C, */

    /*  where alpha and beta are scalars,  A is a symmetric matrix and  B and */
    /*  C are  m by n matrices. */

    /*  Arguments */
    /*  ========== */

    /*  SIDE   - CHARACTER*1. */
    /*           On entry,  SIDE  specifies whether  the  symmetric matrix  A */
    /*           appears on the  left or right  in the  operation as follows: */

    /*              SIDE = 'L' or 'l'   C := alpha*A*B + beta*C, */

    /*              SIDE = 'R' or 'r'   C := alpha*B*A + beta*C, */

    /*           Unchanged on exit. */

    /*  UPLO   - CHARACTER*1. */
    /*           On  entry,   UPLO  specifies  whether  the  upper  or  lower */
    /*           triangular  part  of  the  symmetric  matrix   A  is  to  be */
    /*           referenced as follows: */

    /*              UPLO = 'U' or 'u'   Only the upper triangular part of the */
    /*                                  symmetric matrix is to be referenced. */

    /*              UPLO = 'L' or 'l'   Only the lower triangular part of the */
    /*                                  symmetric matrix is to be referenced. */

    /*           Unchanged on exit. */

    /*  M      - INTEGER. */
    /*           On entry,  M  specifies the number of rows of the matrix  C. */
    /*           M  must be at least zero. */
    /*           Unchanged on exit. */

    /*  N      - INTEGER. */
    /*           On entry, N specifies the number of columns of the matrix C. */
    /*           N  must be at least zero. */
    /*           Unchanged on exit. */

    /*  ALPHA  - REAL            . */
    /*           On entry, ALPHA specifies the scalar alpha. */
    /*           Unchanged on exit. */

    /*  A      - REAL             array of DIMENSION ( LDA, ka ), where ka is */
    /*           m  when  SIDE = 'L' or 'l'  and is  n otherwise. */
    /*           Before entry  with  SIDE = 'L' or 'l',  the  m by m  part of */
    /*           the array  A  must contain the  symmetric matrix,  such that */
    /*           when  UPLO = 'U' or 'u', the leading m by m upper triangular */
    /*           part of the array  A  must contain the upper triangular part */
    /*           of the  symmetric matrix and the  strictly  lower triangular */
    /*           part of  A  is not referenced,  and when  UPLO = 'L' or 'l', */
    /*           the leading  m by m  lower triangular part  of the  array  A */
    /*           must  contain  the  lower triangular part  of the  symmetric */
    /*           matrix and the  strictly upper triangular part of  A  is not */
    /*           referenced. */
    /*           Before entry  with  SIDE = 'R' or 'r',  the  n by n  part of */
    /*           the array  A  must contain the  symmetric matrix,  such that */
    /*           when  UPLO = 'U' or 'u', the leading n by n upper triangular */
    /*           part of the array  A  must contain the upper triangular part */
    /*           of the  symmetric matrix and the  strictly  lower triangular */
    /*           part of  A  is not referenced,  and when  UPLO = 'L' or 'l', */
    /*           the leading  n by n  lower triangular part  of the  array  A */
    /*           must  contain  the  lower triangular part  of the  symmetric */
    /*           matrix and the  strictly upper triangular part of  A  is not */
    /*           referenced. */
    /*           Unchanged on exit. */

    /*  LDA    - INTEGER. */
    /*           On entry, LDA specifies the first dimension of A as declared */
    /*           in the calling (sub) program.  When  SIDE = 'L' or 'l'  then */
    /*           LDA must be at least  max( 1, m ), otherwise  LDA must be at */
    /*           least  max( 1, n ). */
    /*           Unchanged on exit. */

    /*  B      - REAL             array of DIMENSION ( LDB, n ). */
    /*           Before entry, the leading  m by n part of the array  B  must */
    /*           contain the matrix B. */
    /*           Unchanged on exit. */

    /*  LDB    - INTEGER. */
    /*           On entry, LDB specifies the first dimension of B as declared */
    /*           in  the  calling  (sub)  program.   LDB  must  be  at  least */
    /*           max( 1, m ). */
    /*           Unchanged on exit. */

    /*  BETA   - REAL            . */
    /*           On entry,  BETA  specifies the scalar  beta.  When  BETA  is */
    /*           supplied as zero then C need not be set on input. */
    /*           Unchanged on exit. */

    /*  C      - REAL             array of DIMENSION ( LDC, n ). */
    /*           Before entry, the leading  m by n  part of the array  C must */
    /*           contain the matrix  C,  except when  beta  is zero, in which */
    /*           case C need not be set on entry. */
    /*           On exit, the array  C  is overwritten by the  m by n updated */
    /*           matrix. */

    /*  LDC    - INTEGER. */
    /*           On entry, LDC specifies the first dimension of C as declared */
    /*           in  the  calling  (sub)  program.   LDC  must  be  at  least */
    /*           max( 1, m ). */
    /*           Unchanged on exit. */

    /*  Level 3 Blas routine. */

    /*  -- Written on 8-February-1989. */
    /*     Jack Dongarra, Argonne National Laboratory. */
    /*     Iain Duff, AERE Harwell. */
    /*     Jeremy Du Croz, Numerical Algorithms Group Ltd. */
    /*     Sven Hammarling, Numerical Algorithms Group Ltd. */

    /*     Set NROWA as the number of rows of A. */

    /* Parameter adjustments */
    a_dim1   = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1   = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    c_dim1   = *ldc;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;

    /* Function Body */
    if (lsame_(side, "L"))
    {
        nrowa = *m;
    }
    else
    {
        nrowa = *n;
    }
    upper = lsame_(uplo, "U");

    /*     Test the input parameters. */

    info = 0;
    if (!lsame_(side, "L") && !lsame_(side, "R"))
    {
        info = 1;
    }
    else if (!upper && !lsame_(uplo, "L"))
    {
        info = 2;
    }
    else if (*m < 0)
    {
        info = 3;
    }
    else if (*n < 0)
    {
        info = 4;
    }
    else if (*lda < max(1, nrowa))
    {
        info = 7;
    }
    else if (*ldb < max(1, *m))
    {
        info = 9;
    }
    else if (*ldc < max(1, *m))
    {
        info = 12;
    }
    if (info != 0)
    {
        return info;
    }

    /*     Quick return if possible. */

    if (*m == 0 || *n == 0 || (*alpha == 0.f && *beta == 1.f))
    {
        return 0;
    }

    /*     And when  alpha.eq.zero. */

    if (*alpha == 0.f)
    {
        if (*beta == 0.f)
        {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__)
                {
                    c__[i__ + j * c_dim1] = 0.f;
                }
            }
        }
        else
        {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__)
                {
                    c__[i__ + j * c_dim1] = *beta * c__[i__ + j * c_dim1];
                }
            }
        }
        return 0;
    }

    /*     Start the operations. */

    if (lsame_(side, "L"))
    {

        /*        Form  C := alpha*A*B + beta*C. */

        if (upper)
        {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__)
                {
                    temp1 = *alpha * b[i__ + j * b_dim1];
                    temp2 = 0.f;
                    i__3  = i__ - 1;
                    for (k = 1; k <= i__3; ++k)
                    {
                        c__[k + j * c_dim1] += temp1 * a[k + i__ * a_dim1];
                        temp2 += b[k + j * b_dim1] * a[k + i__ * a_dim1];
                    }
                    if (*beta == 0.f)
                    {
                        c__[i__ + j * c_dim1] = temp1 * a[i__ + i__ * a_dim1] + *alpha * temp2;
                    }
                    else
                    {
                        c__[i__ + j * c_dim1] = *beta * c__[i__ + j * c_dim1] +
                                                temp1 * a[i__ + i__ * a_dim1] + *alpha * temp2;
                    }
                }
            }
        }
        else
        {
            i__1 = *n;
            for (j = 1; j <= i__1; ++j)
            {
                for (i__ = *m; i__ >= 1; --i__)
                {
                    temp1 = *alpha * b[i__ + j * b_dim1];
                    temp2 = 0.f;
                    i__2  = *m;
                    for (k = i__ + 1; k <= i__2; ++k)
                    {
                        c__[k + j * c_dim1] += temp1 * a[k + i__ * a_dim1];
                        temp2 += b[k + j * b_dim1] * a[k + i__ * a_dim1];
                    }
                    if (*beta == 0.f)
                    {
                        c__[i__ + j * c_dim1] = temp1 * a[i__ + i__ * a_dim1] + *alpha * temp2;
                    }
                    else
                    {
                        c__[i__ + j * c_dim1] = *beta * c__[i__ + j * c_dim1] +
                                                temp1 * a[i__ + i__ * a_dim1] + *alpha * temp2;
                    }
                }
            }
        }
    }
    else
    {
        /*        Form  C := alpha*B*A + beta*C. */
        i__1 = *n;
        for (j = 1; j <= i__1; ++j)
        {
            temp1 = *alpha * a[j + j * a_dim1];
            if (*beta == 0.f)
            {
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__)
                {
                    c__[i__ + j * c_dim1] = temp1 * b[i__ + j * b_dim1];
                }
            }
            else
            {
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__)
                {
                    c__[i__ + j * c_dim1] =
                        *beta * c__[i__ + j * c_dim1] + temp1 * b[i__ + j * b_dim1];
                }
            }
            i__2 = j - 1;
            for (k = 1; k <= i__2; ++k)
            {
                if (upper)
                {
                    temp1 = *alpha * a[k + j * a_dim1];
                }
                else
                {
                    temp1 = *alpha * a[j + k * a_dim1];
                }
                i__3 = *m;
                for (i__ = 1; i__ <= i__3; ++i__)
                {
                    c__[i__ + j * c_dim1] += temp1 * b[i__ + k * b_dim1];
                }
            }
            i__2 = *n;
            for (k = j + 1; k <= i__2; ++k)
            {
                if (upper)
                {
                    temp1 = *alpha * a[j + k * a_dim1];
                }
                else
                {
                    temp1 = *alpha * a[k + j * a_dim1];
                }
                i__3 = *m;
                for (i__ = 1; i__ <= i__3; ++i__)
                {
                    c__[i__ + j * c_dim1] += temp1 * b[i__ + k * b_dim1];
                }
            }
        }
    }

    return 0;
}

int strmm_(const char* side, const char* uplo, const char* transa, const char*
           diag, int*m, const int* n, const float* alpha, float* a, const int* lda,
           float* b, const int* ldb)
{
    int a_dim1, a_offset, b_dim1, b_offset, i__1, i__2, i__3;
    int i__, j, k, info;
    float temp;
    int lside;
    int nrowa;
    int upper;
    int nounit;

/*  Purpose */
/*  ======= */

/*  STRMM  performs one of the matrix-matrix operations */

/*     B := alpha*op( A )*B,   or   B := alpha*B*op( A ), */

/*  where  alpha  is a scalar,  B  is an m by n matrix,  A  is a unit, or */
/*  non-unit,  upper or lower triangular matrix  and  op( A )  is one  of */

/*     op( A ) = A   or   op( A ) = A'. */

/*  Arguments */
/*  ========== */

/*  SIDE   - CHARACTER*1. */
/*           On entry,  SIDE specifies whether  op( A ) multiplies B from */
/*           the left or right as follows: */

/*              SIDE = 'L' or 'l'   B := alpha*op( A )*B. */

/*              SIDE = 'R' or 'r'   B := alpha*B*op( A ). */

/*           Unchanged on exit. */

/*  UPLO   - CHARACTER*1. */
/*           On entry, UPLO specifies whether the matrix A is an upper or */
/*           lower triangular matrix as follows: */

/*              UPLO = 'U' or 'u'   A is an upper triangular matrix. */

/*              UPLO = 'L' or 'l'   A is a lower triangular matrix. */

/*           Unchanged on exit. */

/*  TRANSA - CHARACTER*1. */
/*           On entry, TRANSA specifies the form of op( A ) to be used in */
/*           the matrix multiplication as follows: */

/*              TRANSA = 'N' or 'n'   op( A ) = A. */

/*              TRANSA = 'T' or 't'   op( A ) = A'. */

/*              TRANSA = 'C' or 'c'   op( A ) = A'. */

/*           Unchanged on exit. */

/*  DIAG   - CHARACTER*1. */
/*           On entry, DIAG specifies whether or not A is unit triangular */
/*           as follows: */

/*              DIAG = 'U' or 'u'   A is assumed to be unit triangular. */

/*              DIAG = 'N' or 'n'   A is not assumed to be unit */
/*                                  triangular. */

/*           Unchanged on exit. */

/*  M      - INTEGER. */
/*           On entry, M specifies the number of rows of B. M must be at */
/*           least zero. */
/*           Unchanged on exit. */

/*  N      - INTEGER. */
/*           On entry, N specifies the number of columns of B.  N must be */
/*           at least zero. */
/*           Unchanged on exit. */

/*  ALPHA  - REAL            . */
/*           On entry,  ALPHA specifies the scalar  alpha. When  alpha is */
/*           zero then  A is not referenced and  B need not be set before */
/*           entry. */
/*           Unchanged on exit. */

/*  A      - REAL             array of DIMENSION ( LDA, k ), where k is m */
/*           when  SIDE = 'L' or 'l'  and is  n  when  SIDE = 'R' or 'r'. */
/*           Before entry  with  UPLO = 'U' or 'u',  the  leading  k by k */
/*           upper triangular part of the array  A must contain the upper */
/*           triangular matrix  and the strictly lower triangular part of */
/*           A is not referenced. */
/*           Before entry  with  UPLO = 'L' or 'l',  the  leading  k by k */
/*           lower triangular part of the array  A must contain the lower */
/*           triangular matrix  and the strictly upper triangular part of */
/*           A is not referenced. */
/*           Note that when  DIAG = 'U' or 'u',  the diagonal elements of */
/*           A  are not referenced either,  but are assumed to be  unity. */
/*           Unchanged on exit. */

/*  LDA    - INTEGER. */
/*           On entry, LDA specifies the first dimension of A as declared */
/*           in the calling (sub) program.  When  SIDE = 'L' or 'l'  then */
/*           LDA  must be at least  max( 1, m ),  when  SIDE = 'R' or 'r' */
/*           then LDA must be at least max( 1, n ). */
/*           Unchanged on exit. */

/*  B      - REAL             array of DIMENSION ( LDB, n ). */
/*           Before entry,  the leading  m by n part of the array  B must */
/*           contain the matrix  B,  and  on exit  is overwritten  by the */
/*           transformed matrix. */

/*  LDB    - INTEGER. */
/*           On entry, LDB specifies the first dimension of B as declared */
/*           in  the  calling  (sub)  program.   LDB  must  be  at  least */
/*           max( 1, m ). */
/*           Unchanged on exit. */


/*  Level 3 Blas routine. */

/*  -- Written on 8-February-1989. */
/*     Jack Dongarra, Argonne National Laboratory. */
/*     Iain Duff, AERE Harwell. */
/*     Jeremy Du Croz, Numerical Algorithms Group Ltd. */
/*     Sven Hammarling, Numerical Algorithms Group Ltd. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;

    /* Function Body */
    lside = lsame_(side, "L");
    if (lside) {
        nrowa = *m;
    } else {
        nrowa = *n;
    }
    nounit = lsame_(diag, "N");
    upper = lsame_(uplo, "U");

    info = 0;
    if (! lside && ! lsame_(side, "R")) {
    info = 1;
    } else if (! upper && ! lsame_(uplo, "L")) {
    info = 2;
    } else if (! lsame_(transa, "N") && ! lsame_(transa,
         "T") && ! lsame_(transa, "C")) {
    info = 3;
    } else if (! lsame_(diag, "U") && ! lsame_(diag,
        "N")) {
    info = 4;
    } else if (*m < 0) {
    info = 5;
    } else if (*n < 0) {
    info = 6;
    } else if (*lda < max(1,nrowa)) {
    info = 9;
    } else if (*ldb < max(1,*m)) {
    info = 11;
    }
    if (info != 0) {
        return -1;
    }

/*     Quick return if possible. */
    if (*m == 0 || *n == 0) {
        return 0;
    }

/*     And when  alpha.eq.zero. */

    if (*alpha == 0.f) {
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
        i__2 = *m;
        for (i__ = 1; i__ <= i__2; ++i__) {
        b[i__ + j * b_dim1] = 0.f;
/* L10: */
        }
/* L20: */
    }
    return 0;
    }

/*     Start the operations. */
    if (lside) {
    if (lsame_(transa, "N")) {
/*           Form  B := alpha*A*B. */
        if (upper) {
        i__1 = *n;
        for (j = 1; j <= i__1; ++j) {
            i__2 = *m;
            for (k = 1; k <= i__2; ++k) {
            if (b[k + j * b_dim1] != 0.f) {
                temp = *alpha * b[k + j * b_dim1];
                i__3 = k - 1;
                for (i__ = 1; i__ <= i__3; ++i__) {
                b[i__ + j * b_dim1] += temp * a[i__ + k *
                    a_dim1];
                }
                if (nounit) {
                temp *= a[k + k * a_dim1];
                }
                b[k + j * b_dim1] = temp;
            }
            }
        }
        } else {
        i__1 = *n;
        for (j = 1; j <= i__1; ++j) {
            for (k = *m; k >= 1; --k) {
            if (b[k + j * b_dim1] != 0.f) {
                temp = *alpha * b[k + j * b_dim1];
                b[k + j * b_dim1] = temp;
                if (nounit) {
                b[k + j * b_dim1] *= a[k + k * a_dim1];
                }
                i__2 = *m;
                for (i__ = k + 1; i__ <= i__2; ++i__) {
                b[i__ + j * b_dim1] += temp * a[i__ + k *
                    a_dim1];
                }
            }
            }
        }
        }
    } else {
/*           Form  B := alpha*A'*B. */
        if (upper) {
        i__1 = *n;
        for (j = 1; j <= i__1; ++j) {
            for (i__ = *m; i__ >= 1; --i__) {
            temp = b[i__ + j * b_dim1];
            if (nounit) {
                temp *= a[i__ + i__ * a_dim1];
            }
            i__2 = i__ - 1;
            for (k = 1; k <= i__2; ++k) {
                temp += a[k + i__ * a_dim1] * b[k + j * b_dim1];
            }
            b[i__ + j * b_dim1] = *alpha * temp;
            }
        }
        } else {
        i__1 = *n;
        for (j = 1; j <= i__1; ++j) {
            i__2 = *m;
            for (i__ = 1; i__ <= i__2; ++i__) {
            temp = b[i__ + j * b_dim1];
            if (nounit) {
                temp *= a[i__ + i__ * a_dim1];
            }
            i__3 = *m;
            for (k = i__ + 1; k <= i__3; ++k) {
                temp += a[k + i__ * a_dim1] * b[k + j * b_dim1];
            }
            b[i__ + j * b_dim1] = *alpha * temp;
            }
        }
        }
    }
    } else {
    if (lsame_(transa, "N")) {
/*           Form  B := alpha*B*A. */
        if (upper) {
        for (j = *n; j >= 1; --j) {
            temp = *alpha;
            if (nounit) {
            temp *= a[j + j * a_dim1];
            }
            i__1 = *m;
            for (i__ = 1; i__ <= i__1; ++i__) {
                b[i__ + j * b_dim1] = temp * b[i__ + j * b_dim1];
            }
            i__1 = j - 1;
            for (k = 1; k <= i__1; ++k) {
            if (a[k + j * a_dim1] != 0.f) {
                temp = *alpha * a[k + j * a_dim1];
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__) {
                b[i__ + j * b_dim1] += temp * b[i__ + k *
                    b_dim1];
                }
            }
            }
        }
        } else {
        i__1 = *n;
        for (j = 1; j <= i__1; ++j) {
            temp = *alpha;
            if (nounit) {
            temp *= a[j + j * a_dim1];
            }
            i__2 = *m;
            for (i__ = 1; i__ <= i__2; ++i__) {
            b[i__ + j * b_dim1] = temp * b[i__ + j * b_dim1];
            }
            i__2 = *n;
            for (k = j + 1; k <= i__2; ++k) {
            if (a[k + j * a_dim1] != 0.f) {
                temp = *alpha * a[k + j * a_dim1];
                i__3 = *m;
                for (i__ = 1; i__ <= i__3; ++i__) {
                b[i__ + j * b_dim1] += temp * b[i__ + k *
                    b_dim1];
                }
            }
            }
        }
        }
    } else {
/*           Form  B := alpha*B*A'. */
        if (upper) {
        i__1 = *n;
        for (k = 1; k <= i__1; ++k) {
            i__2 = k - 1;
            for (j = 1; j <= i__2; ++j) {
            if (a[j + k * a_dim1] != 0.f) {
                temp = *alpha * a[j + k * a_dim1];
                i__3 = *m;
                for (i__ = 1; i__ <= i__3; ++i__) {
                b[i__ + j * b_dim1] += temp * b[i__ + k *
                    b_dim1];
                }
            }
            }
            temp = *alpha;
            if (nounit) {
            temp *= a[k + k * a_dim1];
            }
            if (temp != 1.f) {
            i__2 = *m;
            for (i__ = 1; i__ <= i__2; ++i__) {
                b[i__ + k * b_dim1] = temp * b[i__ + k * b_dim1];
            }
            }
        }
        } else {
        for (k = *n; k >= 1; --k) {
            i__1 = *n;
            for (j = k + 1; j <= i__1; ++j) {
            if (a[j + k * a_dim1] != 0.f) {
                temp = *alpha * a[j + k * a_dim1];
                i__2 = *m;
                for (i__ = 1; i__ <= i__2; ++i__) {
                b[i__ + j * b_dim1] += temp * b[i__ + k *
                    b_dim1];
                }
            }
            }
            temp = *alpha;
            if (nounit) {
            temp *= a[k + k * a_dim1];
            }
            if (temp != 1.f) {
            i__1 = *m;
            for (i__ = 1; i__ <= i__1; ++i__) {
                b[i__ + k * b_dim1] = temp * b[i__ + k * b_dim1];
            }
            }
        }
        }
    }
    }

    return 0;
} /* strmm_ */

