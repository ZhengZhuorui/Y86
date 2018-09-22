#include "Writeback.h"
Writeback::Writeback(controller *x){
    C=x;
    p_icode=1;
    p_stat=SAOK;
    p_dstE=p_dstM=RNONE;
    p_valE=p_valM=0;
}

void Writeback::next(){
    n_stat=p_stat;

    C->W_valE=p_valE;
    C->W_valE_mtx.unlock();

    C->W_valM=p_valM;
    C->W_valM_mtx.unlock();

    C->W_dstE=p_dstE;
    C->W_dstE_mtx.unlock();

    C->W_dstM=p_dstM;
    C->W_dstM_mtx.unlock();

    C->W_icode=p_icode;
    C->W_icode_mtx.unlock();

    if (p_dstE!=RNONE) C->Reg[p_dstE]=p_valE;
    if (p_dstM!=RNONE) C->Reg[p_dstM]=p_valM;
}
