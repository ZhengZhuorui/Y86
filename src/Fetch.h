#ifndef Fetch_H
#define Fetch_H
#include "controller.h"
class controller;
class Fetch
{
private:
    //UInt PC,stat,icode,ifun,rA,rB,valC,valP;
    volatile int f_pc,pcinc;
    volatile bool NeedvalC,Needregids,Instr_valid,imem_error;
    int selPC();
    controller *C;
public:
    Fetch(controller *x);
    ~Fetch(){}
    void next();
    volatile int p_predPC,n_stat,n_icode,n_ifun,n_rA,n_rB,n_valC,n_valP,n_predPC;
    volatile bool stall;

};
#endif
