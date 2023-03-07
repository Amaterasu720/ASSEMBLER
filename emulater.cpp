#include<bits/stdc++.h>      
using namespace std;
ofstream output;
ifstream input;
long int A=0;
long int B=0;
long int SP=0;
long int PC=0;
long int Memory[1<<24];
vector<string> in;
map<int,pair<int,int>> mp_op;
bool exit1=false;
vector<array<int,4>>trace;

int bin_to_dec(string s){
    int value=0;
    int p=1;
    int n=s.size();
    for(int i=n-1;i>0;i--){
        if(s[i]=='1'){
        value=value + p;
        }

        p=p*2;
    }

    if(s[0]=='0')
    return value;
    else
    return value-p;
}


void get_op(){
    string op;
    int line=0;
   for(auto it=in.begin();it!=in.end();it++){
    op=*it;
    string s1(op,0,24);
    string s2(op,24,8);
     mp_op[line]={bin_to_dec(s1),bin_to_dec(s2)};
    s1.clear();
    s2.clear();
    // s3.clear();
    line++;
    op.clear();
    }
    
}

void ldc(int value){                        
    B = A;
    A = value;
}
void adc(int value){
    A += value;
}
void ldl(int offset){
    B = A;
    A = Memory[SP + offset];
}
void stl(int offset){
    Memory[SP + offset] = A;
    A = B;
}
void ldnl(int offset){
    A = Memory[A + offset];

}
void stnl(int offset){
    Memory[A + offset] = B;

}
void add(){
    A += B;
}
void sub(){
    A = B - A;
}
void shl(){
    A = B << A;
}
void shr(){ 
    A = B >> A;
}
void adj(int value){
    SP = SP + value;
}
void a2sp(){
    SP = A;
    A = B;
}
void sp2a(){
    B = A;
    A = SP;
}
void call(int offset){
    B = A;
    A = PC;
    PC += offset;
}
void ret(){
    PC = A;
    A = B;
}
void brz(int offset){
    if(A == 0)
        PC = PC + offset;
}
void brlz(int offset){
    if(A < 0)
        PC = PC + offset;
}
void br(int offset){ 
    PC = PC + offset;
}
void HALT(){
    exit1=true;
}
void run_code(){
        while(PC<=in.size()&&!exit1){
            int op=mp_op[PC].second;
            int operand=mp_op[PC].first;
            switch (op)
            {
            case 0 :
                ldc(operand);
                break;
            
            case 1 :
                adc(operand);
                break;
            
            case 2 :
                ldl(operand);
                break;

            case 3 :
                stl(operand);
                break;
            case 4 :
                ldnl(operand);
                break;        
            
            case 5 :
                stnl(operand);
                break;

            case 6 :
                add();
                break;

            case 7 :
                sub();
                break;

            case 8 :
                shl();
                break;

            case 9 :
                shr();
                break;

            case 10 :
                adj(operand);
                break;

            case 11 :
                a2sp();
                break;

            case 12 :
                sp2a();
                break;

            case 13 :
                call(operand);
                break;

            case 14:
                ret();
                break;

            case 15 :
                brz(operand);
                break;

            case 16 :
                brlz(operand);
                break;

            case 17 :
                br(operand);
                break;

            case 18 :
               HALT();
                break;                                        
            default:
                break;
            }
            PC++;
            cout<<PC<<endl;
            trace.push_back({A,B,PC,SP});
        }
}

int main(){
string in_code,s;
char x;
cout<<"Enter machine code file name to be complied: ";
cin>>s;
input.open(s,ios::binary);
int count=0;
while(input.read((char*)&x,sizeof(x))){
    count++;
    in_code.push_back(x);
    if(count==32){
        count=0;
        in.push_back(in_code);
        in_code.clear();
    }
}
get_op();
for(int i=0;i<in.size();i++){
    cout<<mp_op[i].first<<"  "<<mp_op[i].second<<endl;
}
run_code();
cout<<in.size()<<endl;
cout<<trace.size()<<endl;
int z=0;
while(z!=3){
    
    if(z==1){
    int line=0;
    for(auto it=trace.begin();it!=trace.end();it++){
        cout<<"Line = "<<line<<" A = "<<trace[line][0]<<" B = "<<trace[line][1]<<" PC = "<<trace[line][2]<<" SP = "<<trace[line][3]<<endl;
        line++;
        }
    }else if(z==2){
        cout<<"MEMORY DUMP IS:\n";
        int y=trace.size();
        cout<<"A = "<<trace[y-1][0]<<"B = "<<trace[y-1][1]<<"PC = "<<trace[y-1][2]<<"SP = "<<trace[y-1][3]<<endl;
        int upto=0;
        cout<<"Enter value upto which index MEMORY storage is to be shown\n";
        cin>>upto;
        cout<<"MEMORY STORAGE UPTO "<<upto<<" index is as follows\n";
        for(int i=0;i<upto;i++){
            cout<<Memory[i]<<endl;
        }
    }
    cout<<"If you want to see the trace line by line :  3type 1" << endl;
    cout<<"If you want to see current Memory Dump :     type 2" << endl;
    cout<<"If you want to exit loop:                    type 3"<<endl;
    cin>>z;
}
}