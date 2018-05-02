#include "linalg.h"

std::tuple<CPUTensor, CPUTensor, CPUTensor> diagonalize(CPUTensor B) {
    std::cout << "begin diagonalize:\n";
    CPUTensor X_t(B.rows, B.rows);
    CPUTensor sigma(B.rows, B.cols);
    CPUTensor Y(B.cols, B.cols);
    
    CPUTensor x(0, 0);
    CPUTensor y(0, 0);
    CPUTensor G(0, 0);
    
    CPUTensor subX_t(0, 0);
    CPUTensor subB(0, 0);
    CPUTensor subY(0, 0);
    
    X_t = identity(X_t);
    Y = identity(Y);
    
    /* find length of upper diagonal */
    int udiag;
    if (B.rows < B.cols) {
        udiag = B.rows;
    } else {
        udiag = B.cols-1;
    }

    int iterations = 0;
    
    while(!B.is_diagonal()) {

        /* iterate for each pair of diagonal and upper diagonal elements to 
           eliminate the upper diagonal element                               */
        for(int i = 0; i < udiag; i++) {
            /* slice the row pair to be rotated out of B */
            block pair1 = block(i, i+1)(i, i+2);
            x = slice(B, pair1);
            
            /* do givens rotation based on x */
            G = givens(x.transpose(), i, B.cols);
            
            block rel = block(0, B.rows)(i, i+2);
            subB = slice(B, rel);
            
            subB = subB*G.transpose();
            B = set_slice(B, rel, subB);
            
            block rel2 = block(0, Y.rows)(i, i+2);
            subY = slice(Y, rel2);
            
            subY = subY*G.transpose();
            Y = set_slice(Y, rel, subY);

            /* slice the col pair to be rotated out of B */
            block pair2 = block(i, i+2)(i, i+1);
            y = slice(B, pair2);
            
            /* do givens rotation based on y */
            G = givens(y, i, B.rows);
            
            block rel3 = block(i, i+2)(0, B.cols);
            subB = slice(B, rel3);
            
            subB = G*subB;
            B = set_slice(B, rel3, subB);
            
            block rel4 = block(i, i+2)(0, X_t.cols);
            subX_t = slice(X_t, rel4);
            
            subX_t = G*subX_t;
            
            X_t = set_slice(X_t, rel4, subX_t);
            
            B = check_zeros(B);            
            if(B.is_diagonal()) {
                break;
            }
        }
        iterations += 1;
        std:: cout << iterations << " iterations complete.\n";
    }
    sigma = B;
    
    auto products = std::make_tuple(X_t, sigma, Y);
    
    std::cout << "end diagonalize:\n";
    return products;
}