#ifndef Writeback_H
#define Writeback_H
#include "controller.h"
using namespace std;
class Writeback
{
public:
//    void Change_PC();
    Writeback(controller *x);
    ~Writeback(){}
    void clear();
    void next();
    volatile bool stall;
    volatile int p_stat,p_icode,p_Cnd,p_valE,p_valM,p_dstE,p_dstM;
    volatile int n_stat;
private:
    controller *C;
};
#endif
