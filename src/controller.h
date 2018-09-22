#ifndef controller_H
#define controller_H
#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include "translation.h"
#include "Fetch.h"
#include "Decode.h"
#include "Execute.h"
#include "Memory.h"
#include "Writeback.h"
using namespace std;
typedef long long LL;
typedef unsigned int UInt;
typedef unsigned ULL;
typedef int logic;
const int MAXMEMORY=1000000;
const int MAXINSTR=10000;

const int IHALT=0;
const int INOP=1;
const int IRRMOV=2;
const int IIRMOV=3;
const int IRMMOV=4;
const int IMRMOV=5;
const int IOP=6;
const int IJXX=7;
const int ICMOVXX=2;
const int ICALL=8;
const int IRET=9;
const int IPUSH=0xA;
const int IPOP=0xB;

const int ALUADD=0;

const int REAX=0;
const int RECX=1;
const int REDX=2;
const int REBX=3;
const int RESP=4;
const int REBP=5;
const int RESI=6;
const int REDI=7;
const int RNONE=0xF;

const int SBUB=0;
const int SAOK=1;
const int SADR=2;
const int SINS=3;
const int SHLT=4;

class Fetch;
class Decode;
class Execute;
class Memory;
class Writeback;
class translation;
class controller
{
public:
    controller(){}
    ~controller();
    void Initial(const char *fp);
    string file;
    ofstream fout;
    Fetch *F;
    Decode *D;
    Execute *E;
    Memory *M;
    Writeback *W;
    translation *T;
    void next();
    int* memory;
    int* Reg;
    mutex e_dstE_mtx,e_valE_mtx;
    mutex M_icode_mtx,M_Cnd_mtx,M_valA_mtx,M_valE_mtx,M_dstE_mtx,M_dstM_mtx,m_valM_mtx,m_stat_mtx;
    mutex W_valE_mtx,W_valM_mtx,W_dstE_mtx,W_dstM_mtx,W_icode_mtx;

    int  e_dstE,e_valE,m_stat;
    int M_icode,M_Cnd,M_valA,M_valE,M_dstE,M_dstM,m_valM;
    int W_valE,W_valM,W_dstE,W_dstM,W_icode;
    int stat;
    void Debug();
    int clock;
private:
    void update();
    void control_unit();
    void init();
};

int str2reg(string s);
string reg2str(int r);
#endif // controller_H

