#include "translation.h"
#include <bits/stdc++.h>
#include <regex>
#include <string>

using namespace std;
translation::translation(string file){
    fp=file;
    Instr=new int[MAXINSTR];
    memset(Instr,0,MAXINSTR*sizeof(int));
}
int translation::getInt(string &s,bool &exist){
    exist=true;
    string tmpp,tmps,tmp;
    regex reg;
    smatch sm;
    int ret;
    int base=10;
    reg="\\d";
    if (regex_search(s,sm,reg)){
        tmpp=sm.prefix().str();
        tmps=sm.suffix().str();
        tmp=sm.str();

        if (tmpp[tmpp.length()-1]=='-')tmp="-"+tmp;
        int p=0;
        if (tmps[0]=='x' || tmps[0]=='X')
            base=16,tmp+=tmps[0],++p;
        while (p<tmps.length() && tmps[p]>='0' && tmps[p]<='9' || (base==16 && ((tmps[p]>='a' && tmps[p]<='f') || (tmps[p]>='A' && tmps[p]<='F')))){
            tmp=tmp+tmps[p++];
        }
        ret=stoul(tmp,nullptr,base);
        tmps.erase(0,p);
        s=tmps;
        return ret;
    }
    exist=false;
    return 0;
}

int translation::getReg(string &s,bool &exist){
    exist=true;
    string tmp;
    regex reg;
    smatch sm;
    reg="%[a-zA-Z]{3}";
    if (regex_search(s,sm,reg)){
        tmp=sm.str();
        s=sm.suffix().str();
        return str2reg(tmp);
    }
    exist=false;
    return 0;
}

int translation::getFun(string &s,bool &exist){
    exist=true;
    string tmp;
    regex reg;
    smatch sm;
    reg="[0-9a-zA-Z]{1,20}";
    int ret;
    if (regex_search(s,sm,reg)){
        tmp=sm.str();
        if (Func.count(tmp)==0) ret=0,exist=false;
        else {
            ret=Func[tmp];
            s=sm.suffix().str();
        }
        return ret;
    }
    exist=false;
    return 0;
}

void translation::addlong(int *Instr,int &place,int tmp){
    place+=4;
    for (int i=1;i<=4;++i){
        Instr[place-i]=tmp&255;
        tmp>>=8;
    }
}

void translation::trans(){
    fin.open(fp);
    string s,ts;
    smatch sm;
    regex reg;
    int rmf=0;
    int rmp=0;
    int align=0;
    int place=0;
    bool exist;
    while (getline(fin,s)){
        int p=s.find("#");
        if (p!=string::npos) s.erase(p);
        do{
            if (rmf==0){
                p=s.find("/*");
                if (p!=string::npos){
                    rmf=1;rmp=p;
                }
                else break;
            }
            if (rmf==1){
                p=s.find("*/");
                if (p!=string::npos) s.erase(rmp,p-rmp+1);
                else s.clear();
                rmf=0;
            }
        }while(rmf==0);

        reg=".pos";
        if (regex_search(s,sm,reg)) place=getInt(s,exist);
        reg=".align";
        if (regex_search(s,sm,reg)) align=getInt(s,exist);
        reg="[0-9a-zA-Z]{1,20}:";
        if (regex_search(s,sm,reg)) {
            string tmp=sm.str();
            s=sm.suffix().str();
            reg="[0-9a-zA-Z]{1,20}";
            regex_search(tmp,sm,reg);
            tmp=sm.str();
            Func[tmp]=place;
        }
        reg=".long";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            int tmp;
            //cout<<s<<endl;
            tmp=getFun(s,exist);
            //cout<<s<<endl;
            if (exist==false) tmp=getInt(s,exist);
            addlong(Instr,place,tmp);
        }
        reg=".byte";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            int tmp;
            tmp=getFun(s,exist);
            if (exist==false) tmp=getInt(s,exist);
            Instr[place++]=tmp;
        }
        reg="halt";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x00;
        }

        reg="nop";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x10;
        }

        reg="rrmov[a-zA-Z]?";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x20;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="irmov[a-zA-Z]?";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x30;
            int tmp=getFun(s,exist);
            if (exist==false) tmp=getInt(s,exist);
            Instr[place++]=(0xF0+getReg(s,exist));
            addlong(Instr,place,tmp);
        }

        reg="rmmov[a-zA-Z]?";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x40;
            Instr[place]=getReg(s,exist)<<4;
            int tmp=getInt(s,exist);
            Instr[place++]+=getReg(s,exist);
            addlong(Instr,place,tmp);
        }

        reg="mrmov[a-zA-Z]?";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x50;
            int tmp=getInt(s,exist);
            Instr[place]=getReg(s,exist);
            Instr[place++]+=getReg(s,exist)<<4;
            addlong(Instr,place,tmp);
        }

        reg="add[a-zA-Z]?";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x60;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="sub[a-zA-Z]?";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x61;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="and[a-zA-Z]?";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x62;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="xor[a-zA-Z]?";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x63;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="jmp ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x70;
            int tmp=getFun(s,exist);
            if (exist==false) tmp=getInt(s,exist);
            addlong(Instr,place,tmp);
        }

        reg="jle ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x71;
            //cout<<s<<endl;
            int tmp=getFun(s,exist);

            if (exist==false) tmp=getInt(s,exist);
            addlong(Instr,place,tmp);
        }

        reg="jl ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x72;
            int tmp=getFun(s,exist);
            if (exist==false) tmp=getInt(s,exist);
            addlong(Instr,place,tmp);
        }

        reg="je ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x73;
            int tmp=getFun(s,exist);
            if (exist==false) tmp=getInt(s,exist);
            addlong(Instr,place,tmp);
        }

        reg="jne ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x74;
            int tmp=getFun(s,exist);
            if (exist==false) tmp=getInt(s,exist);
            addlong(Instr,place,tmp);
        }

        reg="jge ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x75;
            int tmp=getFun(s,exist);
            if (exist==false) tmp=getInt(s,exist);
            addlong(Instr,place,tmp);
        }

        reg="jg ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x71;
            int tmp=getFun(s,exist);
            if (exist==false) tmp=getInt(s,exist);
            addlong(Instr,place,tmp);
        }

        reg="cmovle ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x21;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="cmovl ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x22;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="cmove ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x23;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="cmovne ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x24;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="cmovge ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x25;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="cmovg ";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x26;
            Instr[place++]=(getReg(s,exist)<<4)+getReg(s,exist);
        }

        reg="call";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x80;
            int tmp=getFun(s,exist);
            if (exist==false) tmp=getInt(s,exist);
            addlong(Instr,place,tmp);
        }

        reg="ret";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0x90;
        }

        reg="push[a-zA-Z]?";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0xA0;
            Instr[place++]=(getReg(s,exist)<<4)+0xF;
        }

        reg="pop[a-zA-Z]?";
        if (regex_search(s,sm,reg)){
            s=sm.suffix().str();
            Instr[place++]=0xB0;
            Instr[place++]=(getReg(s,exist)<<4)+0xF;
        }
    }
    Maxplace=place;
    fin.close();
}
translation::~translation(){
    delete Instr;
}
void translation::Debug(const char *file){
    ofstream fout(file);
    for (int i=0;i<Maxplace;++i){
        fout<<hex<<Instr[i]<<" ";
    }
    fout<<endl;
    for (map<string,int>::iterator it=Func.begin();it!=Func.end();++it){
        fout<< it->first <<" "<< it->second <<endl;
        //cout<< it->first <<" "<< it->second <<endl;
    }
}
