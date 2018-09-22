#include "Memory.h"
Memory::Memory(controller *x){
    C=x;
    clear();
}
void Memory::clear(){
    p_stat=SAOK;
    p_icode=1;
    p_Cnd=1;
    p_valE=p_valA=0;
    p_dstE=p_dstM=RNONE;
}

void Memory::next(){

    C->M_valA=p_valA;
    C->M_valA_mtx.unlock();

    C->M_valE=p_valE;
    C->M_valE_mtx.unlock();

    C->M_dstE=p_dstE;
    C->M_dstE_mtx.unlock();
    n_dstE=p_dstE;

    C->M_dstM=p_dstM;
    C->M_dstM_mtx.unlock();
    n_dstM=p_dstM;

    C->M_Cnd=p_Cnd;
    C->M_Cnd_mtx.unlock();

    C->M_icode=p_icode;
    C->M_icode_mtx.unlock();

    n_icode=p_icode;

    if (p_icode==IRMMOV || p_icode==IPUSH || p_icode==ICALL || p_icode==IMRMOV) addr=p_valE;
    else if (p_icode==IPOP || p_icode==IRET) addr=p_valA;

    if (p_icode==IMRMOV || p_icode==IPOP || p_icode==IRET) mem_read=true;
    else mem_read=false;

    if (p_icode==IRMMOV || p_icode==IPUSH || p_icode==ICALL) mem_write=true;
    else mem_write=false;

    dmem_error=false;
    if (mem_write) {
        if (addr>=0 && addr+3<=MAXMEMORY) {
            volatile int tmp=p_valA;
            for (volatile int i=addr+3;i>=addr;--i) {
                C->memory[i]=tmp%256;tmp/=256;
            }
        }
        else dmem_error=true;
    }

    n_valM=0;
    if (mem_read){
        if (addr>=0 && addr+3<=MAXMEMORY) {
            for (volatile int i=0;i<4;++i) n_valM=(n_valM<<8)+C->memory[addr+i];
        }
        else dmem_error=true;
    }

    C->m_valM=n_valM;
    C->m_valM_mtx.unlock();

    n_valE=p_valE;

    if (dmem_error) n_stat=SADR;
    else n_stat=p_stat;
    C->m_stat=n_stat;
    C->m_stat_mtx.unlock();

}
