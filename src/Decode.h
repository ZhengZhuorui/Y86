#ifndef Decode_H
#define Decode_H
#include "controller.h"
class controller;
class Decode
{
public:
    Decode(controller *x);
    ~Decode(){}
    void next();
    void clear();
    bool stall,bubble;
    volatile int p_stat,p_icode,p_ifun,p_rA,p_rB,p_valC,p_valP;
    volatile int n_stat,n_icode,n_ifun,n_valC,n_valA,n_valB,n_dstE,n_dstM,n_srcA,n_srcB;
    //void PRINT();
private:
    //UInt stat,icod,ifun,valC,valA,valB,dstE,dstM;
    volatile int d_rvalA,d_rvalB;
    controller *C;
};
#endif

