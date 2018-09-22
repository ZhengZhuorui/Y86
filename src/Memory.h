#ifndef zzr_Memory_H
#define zzr_Memory_H
#include "controller.h"
using namespace std;
class Memory
{
public:
//    void Change_PC();
    Memory(controller *x);
    ~Memory(){}
    void clear();
    void next();
    bool bubble;
    volatile int p_stat,p_icode,p_Cnd,p_valE,p_valA,p_dstE,p_dstM;
    volatile int n_stat,n_icode,n_valE,n_valM,n_dstE,n_dstM;
private:
    volatile int addr;
    volatile bool mem_read,mem_write,dmem_error;
    //UInt stat,icode,ifun,valE,valM,dstE,dstM;
    controller *C;
};
#endif
