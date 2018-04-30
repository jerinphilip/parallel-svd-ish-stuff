#include "ops.h"
#include "cublas_v2.h"

CUDAContext *ctx = CUDAContext::getInstance();

CUDATensor operator+(CUDATensor A, CUDATensor B){
    CUDATensor C(B.rows, B.cols);
    C = B;

    int n, incx, incy;
    double alpha;

    alpha = 1.0;
    incx = 1, incy = 1;
    n = A._size();


    cublasDaxpy(ctx->handle(), n, &alpha, 
            A.storage->data, incx,
            C.storage->data, incy);
    return C;
}

CUDATensor operator-(CUDATensor A, CUDATensor B){
    CUDATensor C(B.rows, B.cols);
    C = A;

    int n, incx, incy;
    double alpha;

    alpha = -1.0;
    incx = 1, incy = 1;
    n = A._size();


    cublasDaxpy(ctx->handle(), n, &alpha, 
            B.storage->data, incx,
            C.storage->data, incy);
    return C;
}

CUDATensor operator*(double alpha, CUDATensor A){
    CUDATensor C(A.rows, A.cols);
    int n, incx, incy;
    incx = 1, incy = 1;
    n = A._size();
    cublasDaxpy(ctx->handle(), n, &alpha, 
            A.storage->data, incx,
            C.storage->data, incy);
    return C;
}

CUDATensor operator*(CUDATensor A, double alpha){
    return alpha*A;
}

CUDATensor operator*(CUDATensor A, CUDATensor B){
    assert (A.cols == B.rows);
    CUDATensor C(A.rows, B.cols);

    double alpha, beta;
    beta = 0.0;
    alpha = 1.0;

    cublasDgemm(ctx->handle(), 
            CUBLAS_OP_N, CUBLAS_OP_N,
            A.rows, B.cols, A.rows,
            &alpha,
            A.storage->data, A.rows,
            B.storage->data, B.rows,
            &beta,
            C.storage->data, C.rows);

    return C;
}
