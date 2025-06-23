/** @file miniblas.h
 * KFCore
 * @author Jan Zwiener (jan@zwiener.org)
 *
 * @brief Minimal generic BLAS implementation
 *
 * Note: all matrices are stored in column-major order.
 *
 * @{ */

/******************************************************************************
 * SYSTEM INCLUDE FILES
 ******************************************************************************/

/******************************************************************************
 * PROJECT INCLUDE FILES
 ******************************************************************************/

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

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

    int lsame_(const char* a, const char* b);

    int strsm_(const char* side, const char* uplo, const char* transa, const char* diag, int* m,
              const int* n, const float* alpha, const float* a, const int* lda, float* b, const int* ldb);

    int sgemm_(const char* transa, const char* transb, int* m, const int* n, const int* k, const float* alpha,
              const float* a, const int* lda, const float* b, const int* ldb, const float* beta, float* c__, const int* ldc);

    int ssyrk_(const char* uplo, const char* trans, int* n, const int* k, const float* alpha, const float* a, const int* lda,
              const float* beta, float* c__, const int* ldc);

    int ssymm_(const char* side, const char* uplo, int* m, const int* n, const float* alpha, const float* a, const int* lda, const float* b,
              const int* ldb, const float* beta, float* c__, const int* ldc);

    int strmm_(const char* side, const char* uplo, const char* transa, const char* diag, int* m,
               const int* n, const float* alpha, float* a, const int* lda, float* b, const int* ldb);

#ifdef __cplusplus
}
#endif

/* @} */
