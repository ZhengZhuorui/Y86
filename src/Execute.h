#ifndef Execute_H
#define Execute_H
#include "controller.h"
class controller;
class Execute
{
public:
//    void Change_PC();
    Execute(controller *x);
    ~Execute(){}
    void next();
    void clear();
    //void PRINT();
    volatile bool bubble,setCC;
    volatile int p_stat,p_icode,p_ifun,p_valC,p_valA,p_valB,p_dstE,p_dstM,p_srcA,p_srcB;
    volatile int n_stat,n_icode,n_Cnd,n_valE,n_valA,n_dstE,n_dstM;
    volatile int SF,OF,ZF;
private:
    int ALU(int aluA,int aluB,int alufun,bool setCC);
    volatile int e_Cnd,CC,cond,aluA,aluB,alufun;
    controller *C;
    bool getCnd(int ifun);
};
#endif
