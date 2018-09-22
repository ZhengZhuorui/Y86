#include "Execute.h"
Execute::Execute(controller *x){
    C=x;
    SF=OF=ZF=0;
    clear();
}
void Execute::clear(){
    p_stat=SAOK;
    p_icode=1;
    p_ifun=0;
    p_valC=p_valA=p_valB=0;
    p_dstE=p_dstM=p_srcA=p_srcB=RNONE;
}
void Execute::next(){

    n_stat=p_stat;
    n_icode=p_icode;

    //aluA
    if (p_icode==IRRMOV || p_icode==IOP) aluA=p_valA;
    else if (p_icode==IIRMOV || p_icode==IRMMOV || p_icode==IMRMOV) aluA=p_valC;
    else if (p_icode==ICALL || p_icode==IPUSH) aluA=-4;
    else if (p_icode==IRET || p_icode==IPOP) aluA=4;

    //aluB
    if (p_icode==IRMMOV || p_icode==IMRMOV || p_icode==IOP || p_icode==ICALL || p_icode==IPUSH || p_icode==IRET || p_icode==IPOP) aluB=p_valB;
    else if (p_icode==IRRMOV || p_icode==IIRMOV) aluB=0;

    //alufun
    if (p_icode==IOP) alufun=p_ifun;
    else alufun=ALUADD;

    //setCC
    C->m_stat_mtx.lock();
    if ( (p_icode==IOP) && !(C->m_stat==SADR || C->m_stat==SINS || C->m_stat==SHLT) && !(C->W->p_stat==SADR || C->W->p_stat==SINS || C->W->p_stat==SHLT) ) setCC=true;
    else setCC=false;
    C->m_stat_mtx.unlock();


    n_valE=ALU(aluA,aluB,alufun,setCC);

    C->e_valE=n_valE;
    C->e_valE_mtx.unlock();

    n_Cnd=getCnd(p_ifun);

    if (p_icode==IRRMOV && n_Cnd==false) n_dstE=RNONE;
    else n_dstE=p_dstE;

    C->e_dstE=n_dstE;
    C->e_dstE_mtx.unlock();

    n_dstM=p_dstM;

    n_valA=p_valA;
}
int Execute::ALU(int aluA,int aluB,int alufun,bool setCC){
    volatile int ans;
    if (alufun==0) ans=aluA+aluB;
    else if (alufun==1) ans=aluB-aluA;
    else if (alufun==2) ans=aluA&aluB;
    else if (alufun==3) ans=aluA^aluB;
    if (setCC){
        ZF=(ans==0);
        SF=(ans<0);
        if (alufun==0) OF=((aluA<0)==(aluB<0))&&((ans<0)!=(aluA<0));
        else if (alufun==1) OF=((aluB!=aluA+ans)||(aluA!=aluB-ans));
        else OF=0;
    }
    return ans;
}

bool Execute::getCnd(int ifun){
	if (ifun==0) return 1;
	if (ifun==1){
        return (SF^OF)|ZF;
	}
	if (ifun==2) return (SF^OF);
	if (ifun==3) return ZF;
	if (ifun==4) return !ZF;
	if (ifun==5) return !(SF^OF);
	if (ifun==6) return !(SF^OF)&!ZF;
	return 0;
}
