

// Name: AADIT SHARMA    ROLL NO.: 2101AI02//
//Declaration of Authorship
//This txt file, asm.cpp, is part of the assignment of CS210 at the 
//department of Computer Science and Engg, IIT Patna . 

#include<bits/stdc++.h>      
using namespace std;
ofstream output;   
ifstream input;
vector<string> v_input;   // Store the input file
map<int,string> mp_opcode;      // Store the opcodes
map<int,string> mp_operand;     // Store the operands
map<int,string> mp_output;  
map<string,pair<int,int>> mp_address;
map<int,pair<int,int>>mp_inst;  //Store instructions given by the opcodes 
map<string,pair<int,int>>mp_operations;  //Store the operations with their codes
string out;
map<int,int>mp_err;
map<int,string>mp_error;
string opcode;
string operand;
string label;
vector<string> v_output;
vector<string> v_output_bin;
string s_output;
int extra;


void store_input(string file){
    input.open(file);
    string op;
    while(getline(input,op)){
        int flag4=0;
        for(auto op_p=op.begin();op_p!=op.end();op_p++){
            if(*op_p==' '){                     // To remove empty lines
                flag4=1;                    
            }else{
                flag4=0;
                break;
            }
        }

        if(flag4==1) op.clear();
        if(op.empty()){
            continue;
        }else{
            v_input.push_back(op);
        }
    }
    
}

void init()                            
{
    //0 means no value or offset
    //1 means value or label
    //2 means offset
    // Stored as "Mnemonic"-{opcode,value type}
    mp_operations["data"]     = {-1, 1};
    mp_operations["SET"]      = {-2, 1};
    mp_operations["ldc"]      = {0, 1};
    mp_operations["adc"]      = {1, 1};
    mp_operations["ldl"]      = {2, 2};
    mp_operations["stl"]      = {3, 2};
    mp_operations["ldnl"]     = {4, 2};
    mp_operations["stnl"]     = {5, 2};
    mp_operations["add"]      = {6, 0};
    mp_operations["sub"]      = {7, 0};
    mp_operations["shl"]      = {8, 0};
    mp_operations["shr"]      = {9, 0};
    mp_operations["adj"]      = {10, 1};
    mp_operations["a2sp"]     = {11, 0};
    mp_operations["sp2a"]     = {12, 0};
    mp_operations["call"]     = {13, 2};
    mp_operations["return"]   = {14, 0};
    mp_operations["brz"]      = {15, 2};
    mp_operations["brlz"]     = {16, 2};
    mp_operations["br"]       = {17, 2};
    mp_operations["HALT"]     = {18, 0};

}

void error(){
    // Stored as  {error number , actual error}

    mp_error.insert({1,"Duplicate label definiton"});
    mp_error.insert({2,"No such label"});
    mp_error.insert({3,"Not a number"});
    mp_error.insert({4,"Missing operand"});
    mp_error.insert({5,"Unexpected operand"});
    mp_error.insert({6,"Extra text in end of line"});
    mp_error.insert({7,"Bogus label name"});
    mp_error.insert({8,"Bogus mnemonic"});
}

void read_line(string op,int i,int line){
    int flag1=0;
    int flag2=0;
    for(auto p=op.begin();*p!=';'&&*p!='\0';p++){
        if((*p!=' '&&*p!='\t')&&flag1==0){
            flag1=1;
        }
        if(*p==':'){
            label=opcode;
            opcode.clear();     
            flag1=0;
        }

        if((*p!=' '&&*p!='\t')&&flag1==1){
            opcode.push_back(*p);           //store the opcode of the line
        }

        if((*p==' '||*p=='\t')&&flag1==1){
            flag1=2;
        }

        if((*p!=' '&&*p!='\t')&&flag1==2){
            flag1=3;
        }

        if((*p!=' '&&*p!='\t')&&flag1==3){
            operand.push_back(*p);         //store the operand of the line
        }

        if((*p==' '||*p=='\t')&&flag1==3){
            flag1=4;
        }

        if((*p!=' '&&*p!='\t')&&flag1==4){
            extra=1;            // Detect any false or extra input
            flag1=5;
        }

    }

    if(label.empty()==false){
    if(mp_address.find(label)!=mp_address.end()){
        mp_err.insert({i,1});
    }else if(*label.begin()>='0'&&*label.begin()<='9'){
        mp_err.insert({i,7});
    }else{
    mp_address.insert({label,{i,line}});
    }
    }else{
         mp_address.insert({label,{i,line}});
    }
    if(extra==1){
        mp_err.insert({i,6});
    }

    mp_opcode.insert({i,opcode});
    mp_operand.insert({i,operand});

}

int operand_val1(string str,int line,int i ){
    int val;
            char oper[100];
            for(int j=0;j<100;j++) oper[j]='?';
             auto it=str.begin();
            for(int j=0;j<str.size();j++){
                oper[j]=*it;
                it++;
            }
           int x=strtol(oper,NULL,0);
           
            
           if(x==0){
                if(mp_address.find(str)!=mp_address.end()){  // use to get the label addres if valid label
                    val=mp_address[str].second-1-line;
                }else if(str=="0"){
                    val=0;
                }else{
                    if(*str.begin()>='0'&&*str.begin()<='9'){
                    mp_err.insert({i,3});                  // if the operand is not a number
                    }else {
                        mp_err.insert({i,2});              // if operand is not a valid label
                    }
                }
           }else{
                val=x;
           }
        
        return val;
}

int operand_val2(string str,int line,int i){
    int val;
            char oper[100];
            for(int j=0;j<100;j++) oper[j]='?'; // initialise the char array
             auto it=str.begin();
            for(int j=0;j<str.size();j++){
                oper[j]=*it;
                it++;
            }
           int x=strtol(oper,NULL,0);
            
           if(x==0){
                if(mp_address.find(str)!=mp_address.end()){
                    if(mp_opcode[mp_address[str].first]=="SET"){
                       string y= mp_operand[mp_address[str].first];  // for SET to use the given value at label's place
                       stringstream set(y);
                       set>>val;
                    }else
                    val=mp_address[str].second;    // if label is used store its address in the instruction set
                }else if(str=="0"){
                    val=0;
                }else{
                    if(*str.begin()>='0'&&*str.begin()<='9'){
                    mp_err.insert({i,3});
                    }else {
                        mp_err.insert({i,2});
                    }
                }
           }else{
                val=x;
           }
        //    if(mp_opcode[i]=="SET"){

        //    }
        
        return val;
}

void fill_instructions(){
    int line =0;
    for(int i=0;i<v_input.size();i++){
        if(mp_opcode[i].empty()==false){
            if(mp_operations.find(mp_opcode[i])!=mp_operations.end()){   //Enter if the opcode is valid
            int x;          
            int code;
            code=mp_operations[mp_opcode[i]].first;          //To get the opcode number
            x=mp_operations[mp_opcode[i]].second;           // To get the type of value needed to store
            if(x==0){
                if(mp_operand[i].empty()==false){          // IF the value is not needed
                    mp_err.insert({i,5});             // Error if the operand is even even if not needed "UNEXPECTED OPERAND"
                }else
                mp_inst[line] = {code,0};           // Store the correct instruction in the data structures
            
            }else if(x==1){                     //IF the value is needed as a constant or address accordingly
                
                if(mp_operand[i].empty()==true){
                    mp_err.insert({i,4});       // Error if no operand given
                }else{
                    mp_inst[line] = {code,operand_val2(mp_operand[i],line,i)};
                }
            }else{
                if(mp_operand[i].empty()==true){                // IF Offset is stored
                    mp_err.insert({i,4});                       // IF operand not present
                    // cout<<"err2"<<endl;
                }else{
                    mp_inst[line] = {code,operand_val1(mp_operand[i],line,i)};
                }
            }
            line++;
        }
        else
            mp_err.insert({i,8});      // Error if the opcode not defined
        }
        }
}

void print_address(int line){
    char addr[8];
    sprintf(addr, "%X",line);
    string address(addr);
    // output<<"line number is ";
    for(int i=0;i<8-address.size();i++){
        output<<"0";            
    }

    output<<address<<" ";
}

void print_out(int line){
    char addr[8];
    sprintf(addr, "%X",line);
    string address(addr);
    // output<<"line number is ";
    for(int i=0;i<8-address.size();i++){
        output<<"0";
        s_output.push_back('0');        
    }
     for(auto x=address.begin();x!=address.end();x++){    //print address
                s_output.push_back(*x);
            }
            v_output.push_back(s_output);
            s_output.clear();
        
    output<<address<<" ";
}

void int_to_hex(int x){     //operand store and print
    char hex[8];
     sprintf(hex, "%X", x);
    string hex1(hex);
    if(hex1.size()==7){
        hex1.erase(0,1);
    }
    if(hex1.size()==8){
        hex1.erase(0,2);
    }
    // output<<"operand in hex ";
    for(int i=0;i<6-hex1.size();i++){
        s_output.push_back('0');
        output<<"0";
    }
     for(auto x=hex1.begin();x!=hex1.end();x++){
                s_output.push_back(*x);
            }
    output<<hex1;
}

void print_output(string file){
    output.open(file);
    int line=0;         // line stores addresss of operations
    int i=0;
    while(mp_inst.find(line)!=mp_inst.end()&&i<v_input.size()){
        if(mp_opcode[i].empty()==false){
        print_address(line);        // covert line into hexadecimal address


        if(mp_inst[line].first>=0){
            int_to_hex(mp_inst[line].second);           
            char hex[8];
            sprintf(hex, "%X", mp_inst[line].first);   //To convert into hex
            string hex1(hex);
            if(hex1.size()==1){
                output<<0;
                s_output.push_back('0');
            }
            for(auto x=hex1.begin();x!=hex1.end();x++){
                s_output.push_back(*x);
            }
            v_output.push_back(s_output);           //Store operand value
            s_output.clear();
            output<<hex1;
        }else{
            print_out(mp_inst[line].second);            
        }
            output<<"       "<<v_input[i]<<endl; // print the input text in the listing file
            line++;
            i++;
        }else{
            print_address(line);
            output<<"               "<<v_input[i]<<endl;   //print the input text in the listing file
            i++;
        }
    }
    output.close();
}

void print_error(string file){
    output.open(file);
    for(int i=0;i<v_input.size();i++){
       if(mp_err.find(i)!=mp_err.end()){
         output<<"Error in this line is- ";
       }else{
        output<<"No Error in this line";
       }
        output<<mp_error[mp_err[i]];
        output<<endl;
    }
    output.close();
}

void file_name(string in , string &o , string &log ,string &l){
    string name;
    for(auto it=in.begin();it!=in.end();it++){
        name.push_back(*it);
        if(*it=='.'){
            break;
        }
    }
    l=name;
    o=name;
    log=name;
    l.push_back('l');
    log.append("log");
    o.push_back('o');

}

string hex_to_bin(string s){
    string bin;
    for(auto it=s.begin();it!=s.end();it++){
        char x=*it;
        switch(x)
        {
            case '0':
                bin.append("0000");
                break;
            case '1':
                 bin.append("0001");
                break;
            case '2':
                 bin.append("0010");
                break;
            case '3':
                 bin.append("0011");
                break;
            case '4':
                 bin.append("0100");
                break;
            case '5':
                 bin.append("0101");
                break;
            case '6':
                 bin.append("0110");
                break;
            case '7':
                 bin.append("0111");
                break;
            case '8':
                 bin.append("1000");
                break;
            case '9':
                 bin.append("1001");
                break;
            case 'A':
                 bin.append("1010");
                break;
            case 'B':
                 bin.append("1011");
                break;
            case 'C':
                 bin.append("1100");
                break;
            case 'D':
                 bin.append("1101");
                break;
            case 'E':
                 bin.append("1110");
                break;
            case 'F':
                 bin.append("1111");
                break;
            default:
            break;
        }
    }
    return bin;
}

int main(){
    string in,out_o,out_log,out_l;
    cout<<"Enter input file name: ";
    cin>>in;
    store_input(in);
    file_name(in,out_o,out_log,out_l);
    init();
    error(); 
    int line=0;

    for(int i=0;i<v_input.size();i++){
    opcode.clear();
    label.clear();
    operand.clear();
    extra=0;
    read_line(v_input[i],i,line);
    if(opcode.empty()==false) line++;
    } 


    fill_instructions();      // To fill the data structure 


    if(mp_err.empty()==true){
    ofstream a_file(out_l);
    print_output(out_l);    // To print output
    }else{
    ofstream a_file(out_log);   
    print_error(out_log);      // To print the errors
    }
    
    for(auto ito=v_output.begin();ito!=v_output.end();ito++){
        v_output_bin.push_back(hex_to_bin(*ito));
    }

    ofstream c_file(out_o);
    output.open(out_o,ios::binary);
    for(auto ito=v_output_bin.begin();ito!=v_output_bin.end();ito++){
         string str = *ito;
         for(auto i=str.begin();i!=str.end();i++){
            char x=*i;
            output.write((char *)&x , sizeof(x));
         }
    }

}