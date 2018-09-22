#ifndef translation_H
#define translation_H
#include "controller.h"
#include <bits/stdc++.h>

using namespace std;
class translation{
    public:
        //controller* C;
        string fp;
        translation(string file);
        ~translation();
        int *Instr;
        int Maxplace;
        ifstream fin;
        void trans();
        void work(){
            trans();
            trans();
        }
        map<string,int> Func;
        void Debug(const char *file);
    private:
        int getInt(string &s,bool &exist);
        int getReg(string &s,bool &exist);
        int getFun(string &s,bool &exist);
        void addlong(int *Instr,int &place,int tmp);

};

#endif
