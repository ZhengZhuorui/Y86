#include "controller.h"
#include <thread>
int str2reg(string s){
    if (s.find("eax")!=string::npos) return REAX;
    if (s.find("ecx")!=string::npos) return RECX;
    if (s.find("edx")!=string::npos) return REDX;
    if (s.find("ebx")!=string::npos) return REBX;
    if (s.find("esp")!=string::npos) return RESP;
    if (s.find("ebp")!=string::npos) return REBP;
    if (s.find("esi")!=string::npos) return RESI;
    if (s.find("edi")!=string::npos) return REDI;
    return RNONE;
}
string reg2str(int r){
    if (r==0) return "eax";
    if (r==1) return "ecx";
    if (r==2) return "edx";
    if (r==3) return "ebx";
    if (r==4) return "esp";
    if (r==5) return "ebp";
    if (r==6) return "esi";
    if (r==7) return "edi";
    return "NONE";
}


void controller::Initial(const char *fp){
    stat=SAOK;
    file=fp;
    clock=0;
    fout.open("Debug.txt");
    T=new translation(file);
    T->work();
    memory=new int[MAXMEMORY];
    Reg=new int[20];
    memset(memory,0,MAXMEMORY*sizeof(int));
    memset(Reg,0,20*sizeof(int));
    memcpy(memory,T->Instr,MAXINSTR*sizeof(int));
    delete T;
    F=new Fetch(this);
    D=new Decode(this);
    E=new Execute(this);
    M=new Memory(this);
    W=new Writeback(this);
    control_unit();
}

controller::~controller(){
    delete F;
    delete D;
    delete E;
    delete M;
    delete W;
    delete Reg;
    delete memory;
}

void controller::init(){
    e_dstE_mtx.lock();
    e_valE_mtx.lock();

    M_icode_mtx.lock();
    M_Cnd_mtx.lock();
    M_valA_mtx.lock();
    M_valE_mtx.lock();
    M_dstE_mtx.lock();
    M_dstM_mtx.lock();
    m_valM_mtx.lock();
    m_stat_mtx.lock();

    W_valE_mtx.lock();
    W_valM_mtx.lock();
    W_dstE_mtx.lock();
    W_dstM_mtx.lock();
    W_icode_mtx.lock();
}

void controller::next(){
    //clock();
    init();
    thread Fe(F->next,F);
    thread De(D->next,D);
    thread Ex(E->next,E);
    thread Me(M->next,M);
    thread Wr(W->next,W);
    Fe.join();
    De.join();
    Ex.join();
    Me.join();
    Wr.join();
    control_unit();
    clock++;
    update();
    stat=W->n_stat;
}
void controller::update(){
    if (!F->stall) F->p_predPC=F->n_predPC;

    if(!D->stall){
        D->p_stat=F->n_stat;
        D->p_icode=F->n_icode;
        D->p_ifun=F->n_ifun;
        D->p_rA=F->n_rA;
        D->p_rB=F->n_rB;
        D->p_valC=F->n_valC;
        D->p_valP=F->n_valP;
    }
    if (D->bubble)D->clear();

    E->p_stat=D->n_stat;
    E->p_icode=D->n_icode;
    E->p_ifun=D->n_ifun;
    E->p_valC=D->n_valC;
    E->p_valA=D->n_valA;
    E->p_valB=D->n_valB;
    E->p_dstE=D->n_dstE;
    E->p_dstM=D->n_dstM;
    E->p_srcA=D->n_srcA;
    E->p_srcB=D->n_srcB;
    if (E->bubble) E->clear();

    M->p_stat=E->n_stat;
    M->p_icode=E->n_icode;
    M->p_Cnd=E->n_Cnd;
    M->p_valE=E->n_valE;
    M->p_valA=E->n_valA;
    M->p_dstE=E->n_dstE;
    M->p_dstM=E->n_dstM;
    if (M->bubble) M->clear();

    if (!W->stall){
        W->p_stat=M->n_stat;
        W->p_icode=M->n_icode;
        W->p_valE=M->n_valE;
        W->p_valM=M->n_valM;
        W->p_dstE=M->n_dstE;
        W->p_dstM=M->n_dstM;
    }

}

void controller::control_unit(){
    if ( ((E->p_icode==IMRMOV || E->p_icode==IPOP) && (E->p_dstM==D->n_srcA || E->p_dstM==D->n_srcB)) || D->p_icode==IRET || E->p_icode==IRET || M->p_icode==IRET) F->stall=true;
    else F->stall=false;

    if ( (E->p_icode==IMRMOV || E->p_icode==IPOP) && (E->p_dstM==D->n_srcA || E->p_dstM==D->n_srcB) ) D->stall=true;
    else D->stall=false;

    if ( (E->p_icode==IJXX && !E->n_Cnd) || !( (E->p_icode == IMRMOV || E->p_icode==IPOP) && (E->p_dstM==D->n_srcA && E->p_dstM==D->n_srcB)) && (D->p_icode==IRET || E->p_icode==IRET || M->p_icode==IRET) ) D->bubble=true;
    else D->bubble=false;

    if ( (E->p_icode==IJXX && !E->n_Cnd) || ( (E->p_icode==IMRMOV || E->p_icode==IPOP) && (E->p_dstM==D->n_srcA || E->p_dstM==D->n_srcB))) E->bubble=true;
    else E->bubble=false;

    if ( (M->n_stat==SADR || M->n_stat==SINS || M->n_stat==SHLT) || (W->p_stat==SADR || W->p_stat==SINS || W->p_stat==SHLT) ) M->bubble=true;
    else M->bubble=false;

    if ( (W->p_stat==SADR || W->p_stat==SINS || W->p_stat==SHLT) ) W->stall=true;
    else W->stall=false;
}
void controller::Debug(){
    fout<<"======================="<<endl;
    fout<<"clock : "<<clock<<endl;
    fout<<"Fetch : predPC= "<<F->p_predPC<<endl;

    fout<<"Decode : stat= "<<D->p_stat<<" | icode= "<<D->p_icode<<" | ifun= "<<D->p_ifun<<" | rA= "<<D->p_rA<<" | rB= "<<D->p_rB;
    fout<<" | valC= "<<D->p_valC<<" | valP= "<<D->p_valP<<endl;

    fout<<"Execute : stat= "<<E->p_stat<<" | icode= "<<E->p_icode<<" | ifun= "<<E->p_ifun<<" | valC= "<<E->p_valC<<" | valA= "<<E->p_valA;
    fout<<" | valB= "<<E->p_valB<<" | dstE= "<<E->p_dstE<<" | dstM= "<<E->p_dstM<<" | srcA= "<<E->p_srcA<<" | srcB= "<<E->p_srcB<<endl;

    fout<<"Memory : stat= "<<M->p_stat<<" | icode= "<<M->p_icode<<" | Cnd= "<<M->p_Cnd<<" | valE= "<<M->p_valE<<" | valA= "<<M->p_valA;
    fout<<" | dstE= "<<M->p_dstE<<" | dstM= "<<M->p_dstM<<endl;

    fout<<"Writeback : stat= "<<W->p_stat<<" | icode= "<<W->p_icode<<" | valE= "<<W->p_valE<<" | valM= "<<W->p_valM<<" | dstE= "<<W->p_dstE<<" | dstM= "<<W->p_dstM<<endl;

    for (int i=0;i<=7;++i) fout<<"Reg["<<i<<"]="<<Reg[i]<<endl;
}
