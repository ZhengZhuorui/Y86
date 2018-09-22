#include "Fetch.h"
using namespace std;

/*View Book P246*/
Fetch::Fetch(controller *x){
    C=x;
    p_predPC=0;
}

void Fetch::next(){
    f_pc=selPC();

    if (f_pc>=0 && f_pc+5<=MAXINSTR) imem_error=false;
    else imem_error=true;

    if (imem_error){
    }
    else{
        n_icode=C->memory[f_pc]>>4;
        n_ifun=C->memory[f_pc]&0xF;
        n_rA=C->memory[f_pc+1]>>4;
        n_rB=C->memory[f_pc+1]&0xF;
        n_valC=0;
    }
    if (n_icode>=0 && n_icode<=0xB) Instr_valid=true;
    else Instr_valid=false;

    if (n_icode==IRRMOV || n_icode==IIRMOV || n_icode==IRMMOV || n_icode==IMRMOV || n_icode==IOP || n_icode==IPUSH || n_icode==IPOP) Needregids=true;
    else Needregids=false;
    if (n_icode==IIRMOV || n_icode==IRMMOV || n_icode==IMRMOV || n_icode==IJXX || n_icode==ICALL) NeedvalC=true;
    else NeedvalC=false;
    pcinc=f_pc+1;
    if (Needregids)++pcinc;
    n_valC=0;
    if (NeedvalC){
        for (volatile int i=0;i<4;++i)n_valC=(n_valC<<8)+C->memory[pcinc++];
    }

    n_valP=pcinc;

    if (n_icode==IJXX || n_icode==ICALL) n_predPC=n_valC;
    else n_predPC=n_valP;

    if (imem_error) n_stat=SADR;
    else if (!Instr_valid) n_stat=SINS;
    else if (n_icode==IHALT) n_stat=SHLT;
    else n_stat=SAOK;
}
int Fetch::selPC(){
    volatile int ans;
    bool sel=false;
    C->M_icode_mtx.lock();
    C->M_Cnd_mtx.lock();
    C->M_valA_mtx.lock();
    if (C->M_icode==IJXX && !C->M_Cnd) ans=C->M_valA,sel=true;
    C->M_icode_mtx.unlock();
    C->M_Cnd_mtx.unlock();
    C->M_valA_mtx.unlock();
    if (sel==true) return ans;

    C->W_icode_mtx.lock();
    C->W_valM_mtx.lock();
    if (C->W_icode==IRET) ans=C->W_valM,sel=true;
    C->W_icode_mtx.unlock();
    C->W_valM_mtx.unlock();
    if (sel==true) return ans;
    return p_predPC;
}

/*void Fetch::PRINT(){
    fout<<"Fetch:"<<endl;
    fout<<"predPC="<<predPC<<endl;
    fout<<endl;
}*/
