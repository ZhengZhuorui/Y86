#include "Decode.h"

Decode::Decode(controller *x){
    C=x;
    clear();
}
void Decode::clear(){
    p_stat=SAOK;
    p_icode=1;
    p_ifun=0;
    p_rA=p_rB=RNONE;
    p_valC=p_valP=0;
}
void Decode::next(){
    //if (C->Writeback_handle->p_stat==SBUB) n_stat=SAOK;
    //else n_stat=p_stat;
    n_stat=p_stat;
    n_icode=p_icode;
    n_ifun=p_ifun;
    n_valC=p_valC;

    //n_dstE
    if (p_icode==IRRMOV) n_dstE=p_rB;
    else if (p_icode==IIRMOV || p_icode==IOP) n_dstE=p_rB;
    else if (p_icode==IPUSH || p_icode==IPOP || p_icode==ICALL || p_icode==IRET) n_dstE=RESP;
    else n_dstE=RNONE;

    //n_dstM
    if (p_icode==IMRMOV || p_icode==IPOP) n_dstM=p_rA;
    else n_dstM=RNONE;

    //n_srcA
    if (p_icode==IRRMOV || p_icode==IRMMOV || p_icode==IOP || p_icode==IPUSH) n_srcA=p_rA;
    else if (p_icode==IPOP || p_icode==IRET) n_srcA=RESP;
    else n_srcA=RNONE;

    //n_srcB
    if (p_icode==IOP || p_icode==IRMMOV || p_icode==IMRMOV) n_srcB=p_rB;
    else if (p_icode==IPUSH || p_icode==IPOP || p_icode==ICALL || p_icode==IRET) n_srcB=RESP;
    else n_srcB=RNONE;

    C->e_dstE_mtx.lock();
    C->e_valE_mtx.lock();

    C->M_valE_mtx.lock();
    C->M_dstE_mtx.lock();
    C->M_dstM_mtx.lock();
    C->m_valM_mtx.lock();

    C->W_dstE_mtx.lock();
    C->W_valE_mtx.lock();
    C->W_dstM_mtx.lock();
    C->W_valM_mtx.lock();

    d_rvalA=C->Reg[n_srcA];
    d_rvalB=C->Reg[n_srcB];

    //n_valA
    if (p_icode==ICALL || p_icode==IJXX) n_valA=p_valP;
    else if (n_srcA==C->e_dstE) n_valA=C->e_valE;
    else if (n_srcA==C->M_dstM) n_valA=C->m_valM;
    else if (n_srcA==C->M_dstE) n_valA=C->M_valE;
    else if (n_srcA==C->W_dstM) n_valA=C->W_valM;
    else if (n_srcA==C->W_dstE) n_valA=C->W_valE;
    else n_valA=d_rvalA;

    //n_valB
    if (n_srcB==C->e_dstE) n_valB=C->e_valE;
    else if (n_srcB==C->M_dstM) n_valB=C->m_valM;
    else if (n_srcB==C->M_dstE) n_valB=C->M_valE;
    else if (n_srcB==C->W_dstM) n_valB=C->W_valM;
    else if (n_srcB==C->W_dstE) n_valB=C->W_valE;
    else n_valB=d_rvalB;

    C->e_dstE_mtx.unlock();
    C->e_valE_mtx.unlock();

    C->M_valE_mtx.unlock();
    C->M_dstE_mtx.unlock();
    C->M_dstM_mtx.unlock();
    C->m_valM_mtx.unlock();

    C->W_dstE_mtx.unlock();
    C->W_valE_mtx.unlock();
    C->W_dstM_mtx.unlock();
    C->W_valM_mtx.unlock();

}
