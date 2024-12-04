
#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include<bitset>
#include<map>
using namespace std;

vector<int> machine_code;
vector<int> instruction_memory;
vector<int> GPR(32,0);
map<int,int> DM;
int pc = 0;
int cycles = 0, instructions = 0;
int string_to_int (string code){
    int ans=0;
    for(int i = 0;i<code.size();i++){
        ans *= 2;
        ans += code[i] - '0';
    }
    return ans;
}
class R_type{
    public:
        string opcode;
        string funct7;
        string funct3;
        int rd, rs1, rs2;
        
        string opcodeFind(string instruction){
            string sameOpcode = "0110011";
            return sameOpcode;
        }
        string funct7Find(string instruction){
            unordered_map<string,string> funct7Map;
            funct7Map["add"] = "0000000";
            funct7Map["sub"] = "0100000";
            funct7Map["sll"] = "0000000";
            funct7Map["slt"] = "0000000";
            funct7Map["sltu"] = "0000000";
            funct7Map["xor"] = "0000000";
            funct7Map["srl"] = "0000000";
            funct7Map["or"] = "0000000";
            funct7Map["and"] = "0000000";
            funct7Map["sra"] = "0100000";
            return funct7Map[instruction];
        }
        string funct3Find(string instruction){
            unordered_map<string,string> funct3Map;
            funct3Map["add"] = "000";
            funct3Map["sub"] ="000";
            funct3Map["sll"] = "001";
            funct3Map["slt"] = "010";
            funct3Map["sltu"] = "011";
            funct3Map["xor"] = "100";
            funct3Map["srl"] = "101";
            funct3Map["sra"] = "101";
            funct3Map["or"] = "110";
            funct3Map["and"] = "111";
            return funct3Map[instruction];
        }
        
        void generate(string code){
            int i=0;
            string instruction ="";
            int dest =0, source1 =0, source2=0;
            while(code[i]==' ')i++;
            while(code[i]!=' '){
                instruction.push_back(code[i]);
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            while(code[i]>='0' && code[i]<='9'){
                dest*=10;
                dest += code[i]-'0';
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            while(code[i]>='0' && code[i]<='9'){
                source1*=10;
                source1 += code[i]-'0';
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            while(code[i]>='0' && code[i]<='9'){
                source2*=10;
                source2 += code[i]-'0';
                i++;
            }
            this->funct7 = funct7Find(instruction);
            this->rs2 = source2;
            this->rs1 = source1;
            this->funct3 = funct3Find(instruction);
            this->opcode = opcodeFind(instruction);
            this->rd = dest;
            combine();
            
        }
        
        void combine(){
            string final = "";
            bitset<5> source2(this->rs2);
            bitset<5> source1(this->rs1);
            bitset<5> dest(this->rd);
            final = final + funct7 + source2.to_string() + source1.to_string() + funct3 + dest.to_string() + opcode;
            cout<<final;
            instruction_memory.push_back(string_to_int(final));
            cout<<endl;
        }
};

class I_type{
    public:
        string opcode;
        string funct3;
        int rd, rs1, imm;
        
        string opcodeFind(string instruction){
            unordered_map<string,string> opcodeMap;
            opcodeMap["addi"] = "0010011";
            opcodeMap["slti"] = "0010011";
            opcodeMap["sltiu"] = "0010011";
            opcodeMap["xori"] = "0010011";
            opcodeMap["ori"] = "0010011";
            opcodeMap["andi"] = "0010011";
            opcodeMap["slli"] = "0010011";
            opcodeMap["srli"] = "0010011";
            opcodeMap["srai"] = "0010011";
            opcodeMap["ld"] = "0000011";
            opcodeMap["lb"] = "0000011";
            opcodeMap["lw"] = "0000011";
            opcodeMap["jalr"] = "1100111";
            return opcodeMap[instruction];
        }

        string funct3Find(string instruction){
            unordered_map<string,string> funct3Map;
            funct3Map["addi"] = "000";
            funct3Map["slti"] = "010";
            funct3Map["sltiu"] = "011";
            funct3Map["xori"] = "100";
            funct3Map["ori"] = "110";
            funct3Map["andi"] = "111";
            funct3Map["slli"] = "001";
            funct3Map["srli"] = "101";
            funct3Map["srai"] = "101";
            funct3Map["lb"] = "000";
            funct3Map["lw"] = "010";
            funct3Map["ld"] = "011";
            funct3Map["jalr"] = "000";
            return funct3Map[instruction];
        }
        
        void generate(string code){
            int i=0;
            string instruction ="";
            int dest =0, source1 =0, source2=0;//source2 is imm
            while(code[i]==' ')i++;
            while(code[i]!=' '){
                instruction.push_back(code[i]);
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            while(code[i]>='0' && code[i]<='9'){
                dest*=10;
                dest += code[i]-'0';
                i++;
            }
            i++;
            while(code[i]==' ') i++;
            bool isNegative = false;
            if(code[i]=='-'){
                isNegative = true;
                i++;
            }
            if(code[i]>='0' && code[i]<='9'){
                while(code[i]>='0' && code[i]<='9'){
                    source2*=10;
                    source2 += code[i]-'0';
                    i++;
                }
                if(isNegative) source2*=-1;

                while(code[i]<'0' || code[i]>'9') i++;
                while(code[i]>='0' && code[i]<='9'){
                    source1*=10;
                    source1 += code[i]-'0';
                    i++;
                }
                
            }
            else{
                while(code[i]<'0' || code[i]>'9') i++;
                while(code[i]>='0' && code[i]<='9'){
                    source1*=10;
                    source1 += code[i]-'0';
                    i++;
                }
                while(code[i]<'0' || code[i]>'9') i++;
                bool isNeg = (code[i-1]=='-');
                while(code[i]>='0' && code[i]<='9'){
                    source2*=10;
                    source2 += code[i]-'0';
                    i++;
                }
                if(isNeg) source2*=-1;
                
            }
       
            this->imm = source2;
            this->rs1 = source1;
            this->funct3 = funct3Find(instruction);
            this->opcode = opcodeFind(instruction);
            this->rd = dest;
            combine();
            
        }
        
        void combine(){
            string final ="";
            bitset<12> source2(this->imm);
            bitset<5> source1(this->rs1);
            bitset<5> dest(this->rd);
            final = final + source2.to_string() + source1.to_string() + funct3 + dest.to_string() + opcode; 
            cout<<final;
            cout<<endl;
            instruction_memory.push_back(string_to_int(final));
        }
};

class S_type{
    public:
        string opcode;
        string funct3;
        int imm, rs1, rs2;
        
        string opcodeFind(string instruction){
            string opcodeSame = "0100011";
            return opcodeSame;
        }
        
        string funct3Find(string instruction){
            unordered_map<string,string> funct3Map;
            funct3Map["sd"] = "011";
            funct3Map["sb"] = "000";
            funct3Map["sw"] = "010";
            return funct3Map[instruction];
        }
        
        void generate(string code){
            int i=0;
            string instruction ="";
            int source=0,dest=0,val=0;
            while(code[i]==' ')i++;
            while(code[i]!=' '){
                instruction.push_back(code[i]);
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            while(code[i]>='0' && code[i]<='9'){
                source*=10;
                source += code[i]-'0';
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            bool isNegative = (code[i-1]=='-');
            while(code[i]>='0' && code[i]<='9'){
                val*=10;
                val += code[i]-'0';
                i++;
            }
            if(isNegative) val*=-1;
            while(code[i]<'0' || code[i]>'9') i++;
            while(code[i]>='0' && code[i]<='9'){
                dest*=10;
                dest += code[i]-'0';
                i++;
            }
            this->rs2 = source;
            this->rs1 = dest;
            this->funct3 = funct3Find(instruction);
            this->opcode = opcodeFind(instruction);
            this->imm = val;
            combine();
            
        }
        
        void combine(){
            bitset<5> source(this->rs2);
            bitset<5> dest(this->rs1);
            bitset<12> val(this->imm);
            string final = "";
            for(int i=11;i>=5;i--) final += to_string(val[i]);
            final = final + source.to_string() + dest.to_string();
            final = final +this->funct3;
            for(int i=4;i>=0;i--) final += to_string(val[i]);
            final += this->opcode;
            cout<<final;
            instruction_memory.push_back(string_to_int(final));
            cout<<endl;
        }
};

class B_type{
    public:
        string opcode;
        string funct3;
        int imm, rs1, rs2;
        
        string opcodeFind(string instruction){
            return "1100011";
        }
        
        string funct3Find(string instruction){
            unordered_map<string,string> funct3Map;
            funct3Map["beq"] = "000";
            funct3Map["bne"] = "001";
            funct3Map["blt"] = "100";
            funct3Map["bge"] = "101";
            funct3Map["bltu"] = "110";
            funct3Map["bgeu"] = "111";
            return funct3Map[instruction];
        }
        
        void generate(string code){
            int i=0;
            string instruction ="";
            int source1=0,source2=0,val=0;
            while(code[i]==' ')i++;
            while(code[i]!=' '){
                instruction.push_back(code[i]);
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            while(code[i]>='0' && code[i]<='9'){
                source1*=10;
                source1 += code[i]-'0';
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            while(code[i]>='0' && code[i]<='9'){
                source2*=10;
                source2 += code[i]-'0';
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            bool isNegative = (code[i-1]=='-');
            while(code[i]>='0' && code[i]<='9'){
                val*=10;
                val += code[i]-'0';
                i++;
            }
            if(isNegative) val*=-1;
            this->rs2 = source2;
            this->rs1 = source1;
            this->funct3 = funct3Find(instruction);
            this->opcode = opcodeFind(instruction);
            this->imm = val;
            combine();
            
        }
        
        void combine(){
            bitset<5> reg2(this->rs2);
            bitset<5> reg1(this->rs1);
            bitset<13> val(this->imm);
            string final = "";
            final += to_string(val[12]);
            for(int i=10;i>=5;i--) final += to_string(val[i]);
            final += reg2.to_string();
            final += reg1.to_string();
            final += funct3;
            for(int i=4;i>=1;i--) final += to_string(val[i]);
            final += to_string(val[10]);
            final += this->opcode;
            cout<<final;
            instruction_memory.push_back(string_to_int(final));
            cout<<endl;
        }
};

class J_type{
    public:
        string opcode;
        int imm, rd;
        
        string opcodeFind(string instruction){
            return "1101111";
        }
        
        
        void generate(string code){
            int i=0;
            string instruction ="";
            int dest=0,val=0;
            while(code[i]==' ')i++;
            while(code[i]!=' '){
                instruction.push_back(code[i]);
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            while(code[i]>='0' && code[i]<='9'){
                dest*=10;
                dest += code[i]-'0';
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            bool isNegative = (code[i-1]=='-');
            while(code[i]>='0' && code[i]<='9'){
                val*=10;
                val += code[i]-'0';
                i++;
            }
            if(isNegative) val*=-1;
            this->rd = dest;
            this->opcode = opcodeFind(instruction);
            this->imm = val;
            combine();
            
        }
        
        void combine(){
            bitset<5> dest(this->rd);
            bitset<21> val(this->imm);
            string final = "";
            final += to_string(val[20]);
            for(int i=10;i>=1;i--) final += to_string(val[i]);
            final += to_string(val[11]);
            for(int i=19;i>=12;i--) final += to_string(val[i]);
            final += dest.to_string();
            final += opcode;
            cout<<final;
            instruction_memory.push_back(string_to_int(final));
            
            cout<<endl;
        }
};

class U_type{
    public:
        string opcode;
        int imm, rd;
        
        string opcodeFind(string instruction){
            return "0110111";
        }
         
        void generate(string code){
            int i=0;
            string instruction ="";
            int dest=0,val=0;
            while(code[i]==' ')i++;
            while(code[i]!=' '){
                instruction.push_back(code[i]);
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            while(code[i]>='0' && code[i]<='9'){
                dest*=10;
                dest += code[i]-'0';
                i++;
            }
            while(code[i]<'0' || code[i]>'9') i++;
            bool isNegative = (code[i-1]=='-');
            while(code[i]>='0' && code[i]<='9'){
                val*=10;
                val += code[i]-'0';
                i++;
            }
            if(isNegative) val*=-1;
            this->rd = dest;
            this->opcode = opcodeFind(instruction);
            this->imm = val;
            combine();
            
        }
        
        void combine(){
            bitset<5> dest(this->rd);
            bitset<20> val(this->imm);
            string final ="";
            for(int i=19;i>=0;i--) final += to_string(val[i]);
            final += dest.to_string();
            final += this->opcode;
            cout<<final;
            instruction_memory.push_back(string_to_int(final));
            cout<<endl;
        }
};



char formatFinder(string instruction){
    unordered_map<string,char> instructionToFormat;
    
    instructionToFormat["add"] ='R';
    instructionToFormat["sub"] ='R';
    instructionToFormat["sll"] ='R';
    instructionToFormat["slt"] ='R';
    instructionToFormat["sltu"] ='R';
    instructionToFormat["xor"] ='R';
    instructionToFormat["srl"] ='R';
    instructionToFormat["sra"] ='R';
    instructionToFormat["or"] ='R';
    instructionToFormat["and"] ='R';
    instructionToFormat["addi"] ='I';
    instructionToFormat["slti"] ='I';
    instructionToFormat["sltiu"] ='I';
    instructionToFormat["xori"] ='I';
    instructionToFormat["ori"] ='I';
    instructionToFormat["andi"] ='I';
    instructionToFormat["slli"] ='I';
    instructionToFormat["srli"] ='I';
    instructionToFormat["srai"] ='I';
    instructionToFormat["lb"] ='I';
    instructionToFormat["lw"] ='I';
    instructionToFormat["ld"] ='I';
    instructionToFormat["jalr"] ='I';
    instructionToFormat["sb"] ='S';
    instructionToFormat["sw"] ='S';
    instructionToFormat["sd"] ='S';
    instructionToFormat["beq"] ='B';
    instructionToFormat["bne"] ='B';
    instructionToFormat["blt"] ='B';
    instructionToFormat["bge"] ='B';
    instructionToFormat["bltu"] ='B';
    instructionToFormat["bgeu"] ='B';
    instructionToFormat["lui"] ='U';
    instructionToFormat["auipc"] ='U';
    instructionToFormat["jal"] ='J';
    return instructionToFormat[instruction];
}

void parsing(string code){
    int i=0;
    string instruction ="";
    while(code[i]==' ')i++;
    while(code[i]!=' '){
        instruction.push_back(code[i]);
        i++;
    }
    char format = formatFinder(instruction);
    switch(format){
        case 'R':
        {
            R_type rtype;
            rtype.generate(code);
            break;
        }
        case 'I':
        {
            I_type itype;
            itype.generate(code);
            break;
        }
        case 'B':
        {
            B_type btype;
            btype.generate(code);
            break;
        }
        case 'J':
        {
            J_type jtype;
            jtype.generate(code);
            break;
        }
        case 'S':
        {
            S_type stype;
            stype.generate(code);
            break;
        }
        case 'U':
        {
            U_type utype;
            utype.generate(code);
            break;
        }
        default:
            cout<<"Invalid"<<endl;
            break;
    }
    return;
}

class fetch{
    public:
    int IR;
    int DPC;
    int NPC;
};
class controls{
    public:
    int ALUOp;
    bool ALUSrc, branch, Jump,MemRead, MemWrite, Mem2Reg, RegRead, RegWrite;
};
class decode{
  public:
  int JPC, DPC, NPC;
  int imm, func,rdl,rs1,rs2,funct7;
  int sd_input;
  controls cw;
};
class execute{
  public:
  controls cw;
  int ALUOUT;
  int rdl;
  int sd_input;
};
class memory_access{
  public:
  controls cw;
  int ALUOUT, LDOUT;
  int rdl, sd_input;
};
class write_back{
    public:
    controls cw;
    int rdl;
};
class pipeline{
    private:
    fetch IF;
    decode ID;
    execute EX;
    memory_access MEM;
    write_back WB;
    bool stall = false;
    bool IF_valid = false, ID_valid = false, EX_valid = false, MEM_valid = false, WB_valid = false;
    public:
    void _fetch(){
        if((pc/4)<instruction_memory.size() && !stall){
            instructions++;
            IF.IR = instruction_memory[pc/4];
            IF.DPC = pc;
            IF.NPC = pc+4;
            pc+=4;
            IF_valid = true;
        }else{
            IF_valid = false;
        }
    }
    
    int signedExtend(int num){
        int imm = 0;
        int imm_20 = (num>>19) &1;
        int imm_10_1 = (num>>9) & 1023;
        int imm_11 = (num>>8) &1;
        int imm_19_12 = num & 255;
        imm = imm | (imm_10_1<<1);
        imm = imm | (imm_11<<11);
        imm = imm | (imm_19_12<<12);
        imm = imm | (imm_20<<20);
        if(imm_20 == 1){
            imm = imm | 0xFFF00000;
        }
        return imm;
    }
    int extractIMM(int IR){
        int imm = 0;
        int imm_11 = IR>>7 &1;
        int imm_4_1 = (IR>>8) & 15;
        int imm_10_5 = (IR>>25) & 63;
        int imm_12 = (IR>>31) & 1;
        imm = imm | (imm_4_1<<1);
        imm = imm | (imm_10_5<<5);
        imm = imm | (imm_11<<11);
        imm = imm | (imm_12<<12);
        if(imm_12 == 1){
            imm = imm | 0xFFFFF000;
        }
        return imm;
    }
    controls controller(int opcode){
        controls ctrl;
        ctrl.ALUSrc = false;
        ctrl.branch = false;
        ctrl.Jump = false;
        ctrl.MemRead = false;
        ctrl.MemWrite = false;
        ctrl.Mem2Reg = false;
        ctrl.RegRead = true;
        ctrl.RegWrite = false;
        ctrl.ALUOp = 0;
        
        switch(opcode){
            case 0b1101111: //JAL
                ctrl.Jump = true;
                ctrl.ALUSrc = false;
                ctrl.ALUOp = 0;
                break;
            case 0b1100011: //Branch
                ctrl.branch = true;
                ctrl.ALUSrc = false;
                ctrl.ALUOp = 1;
                break;
            case 0b0000011: // Load
                ctrl.MemRead = true;
                ctrl.ALUSrc = true;
                ctrl.Mem2Reg = true;
                ctrl.RegWrite = true;
                ctrl.ALUOp = 0;
                break;
            case 0b0100011: // Store
                ctrl.MemWrite = true;
                ctrl.ALUSrc= true;
                ctrl.ALUOp = 0;
                break;
            case 0b0010011: // Immediate
                ctrl.ALUSrc = true;
                ctrl.RegWrite = true;
                ctrl.ALUOp = 3;
                break;
            case 0b0110011: // R-type
                ctrl.RegWrite = true;
                ctrl.ALUOp = 2;
                break;
            default:
                cout<<"Opcode invalid(controller)"<<endl;
                break;
                
        }
        return ctrl;
    }
    int extract_imm_S(int IR){
        int imm = 0;
        int imm_4_0 = (IR>>7)&31;
        int imm_11_5 = (IR>>25) & 127;
        imm = imm | (imm_4_0);
        imm = imm | (imm_11_5<<5);
        if((imm>>11) & 1 == 1){
            imm = imm | 0xFFFFF000;
        }
        return imm;
    }
    int signcorrect(int num){
        int ans = num;
        int signbit = (num>>11)&1;
        if(signbit == 1){
            ans = ans | 0xFFFFF000;
        }
        return ans;
    }
    void helper(){
        ID.NPC = IF.NPC;
            ID.JPC = IF.NPC + signedExtend(IF.IR>>12) - 4;
            ID.DPC = IF.DPC;
            ID.imm = extractIMM(IF.IR);
            ID.func = (IF.IR & 28672)>>12;
            ID.funct7 = (IF.IR>>25);
            ID.rdl = (IF.IR & 3968)>>7;
            ID.cw = controller(IF.IR&127);
            if(ID.cw.RegRead)
                ID.rs1 = GPR[(IF.IR>>15)&31];
            if(ID.cw.ALUSrc){
                if(ID.cw.RegRead){
                    if(ID.cw.MemWrite){
                        ID.rs2 = extract_imm_S(IF.IR);
                        ID.sd_input = GPR[(IF.IR & 32505856)>>20];
                    }else{
                        ID.rs2 = signcorrect((IF.IR >>20));
                    }
                }
            }else{
                if(ID.cw.RegRead){
                    ID.rs2 = GPR[(IF.IR >> 20) & 31];
                }
            }
    }
    void _decode(){
        if(IF_valid && !stall){
            bool hazard = false;
            if (EX_valid && (EX.rdl == ((IF.IR & 1015808) >> 15) || EX.rdl == ((IF.IR & 32505856) >> 20))) {
                hazard = true;
            } else if (MEM_valid && (MEM.rdl == ((IF.IR & 1015808) >> 15) || MEM.rdl == ((IF.IR & 32505856) >> 20))) {
                hazard = true;
            }
            if(hazard){
                stall = true;
                return;
            }else{
                IF_valid = false;
                ID_valid = true;
            }
            helper();
            
        }else if (stall) {
            if ((!EX_valid) &&
                (!MEM_valid)) {
                    stall = false;
                    IF_valid = false;
                    ID_valid = true;
                    helper();
                    
            }
        }
    }
    int ALUControl(int ALUOp, int funct3, int funct7){
        if(ALUOp == 0) return 2;
        if(ALUOp == 1) return 6;
        if(funct3 == 7) return 0;
        if(funct3 == 6) return 1;
        if(funct3 == 0){
            if(ALUOp == 3) return 2;
            if(funct7 == 0) return 2;
            else return 6;
        }
        return -1;
    }
    int ALU(int ALUSelect, int rs1, int rs2){
        int result;
        switch(ALUSelect){
            case 1:
                result = rs1 | rs2;
                break;
            case 0:
                result = rs1 & rs2;
                break;
            case 6:
                result = rs1 - rs2;
                break;
            case 2:
                result = rs1 + rs2;
                break;
            default:
                cout<<"Invalid ALU Operation"<<endl;
                break;
        }
        return result;
    }
    void clear_decode(){
        ID_valid = false;
        ID.JPC = 0;
        ID.DPC = 0;
        ID.NPC = 0;
        ID.imm = 0;
        ID.func = 0;
        ID.rdl = 0;
        ID.rs1 = 0;
        ID.rs2 = 0;
        ID.funct7 = 0;
        ID.sd_input = 0;

        ID.cw.ALUSrc = false;
        ID.cw.branch = false;
        ID.cw.Jump = false;
        ID.cw.MemRead = false;
        ID.cw.MemWrite = false;
        ID.cw.Mem2Reg = false;
        ID.cw.RegRead = false;
        ID.cw.RegWrite = false;
        ID.cw.ALUOp = 0;
    }
    void clear_fetch(){
        IF_valid = false;
        // Reset each attribute of the fetch stage to its default state
        IF.IR = 0;   
        IF.DPC = 0;  
        IF.NPC = 0;  
    }
    void _execute(){
        if(ID_valid){
            EX.cw = ID.cw;
            EX.sd_input = ID.sd_input;
            int ALUSelect = ALUControl(ID.cw.ALUOp, ID.func, ID.funct7);
            EX.ALUOUT = ALU(ALUSelect, ID.rs1, ID.rs2);
            bool ALUZeroFlag = (ID.rs1 == ID.rs2);
            EX.rdl = ID.rdl;
            if(EX.cw.branch && ALUZeroFlag){
                pc = (ID.imm) + ID.NPC -4;
                clear_decode();
                clear_fetch();
            }
            if(EX.cw.Jump) {
                pc = ID.JPC;
                clear_decode();
                clear_fetch();
            }
            ID_valid = false;
            EX_valid = true;
        }
    }
    void _memory_access(){
        if(EX_valid){
            MEM.rdl = EX.rdl;
            MEM.sd_input = EX.sd_input;
            MEM.cw = EX.cw;
            if(MEM.cw.MemWrite){
                DM[EX.ALUOUT] = MEM.sd_input;
            }
            if(MEM.cw.MemRead){
                MEM.LDOUT = DM[EX.ALUOUT];
            }
            MEM.ALUOUT = EX.ALUOUT;
            EX_valid = false;
            MEM_valid = true;
        }
    }
    void _write_back(){
        if(MEM_valid){
            WB.cw = MEM.cw;
            WB.rdl = MEM.rdl;
            if(WB.cw.RegWrite){
                if(WB.cw.Mem2Reg){
                    GPR[WB.rdl] = MEM.LDOUT;
                }else{
                    GPR[WB.rdl] = MEM.ALUOUT;
                }
            }
            WB_valid = true;
            MEM_valid = false;
        }else{
            WB_valid = false;
        }
    }
    void run_pipeline(){
        while(true){
            cycles++;
            _write_back();
            _memory_access();
            _execute();
            _decode();
            _fetch();
            if(!IF_valid && !ID_valid && !EX_valid && !MEM_valid && !WB_valid){
                break;
            }
        }
    }
    
};
void printGPR(){
    cout<<"Printing the GPR"<<endl;
    cout<<"----------------------------------------"<<endl;
    for(int i = 0;i<32;i++){
        cout<<"x"<<i<<" -> "<<GPR[i]<<endl;
    }
    cout<<endl;
}
void printDM(){
    cout<<" Printing the Data Memory"<<endl;
    cout<<"----------------------------------------"<<endl;
    for(auto i:DM){
        cout<<i.first<<" -> "<<i.second<<endl;
    }
    cout<<endl;
}

int main(){
    cout<<"Enter the code\n";
    cout<<"Input a blank line to indicate the end of code"<<endl;
    vector<string> code;
    int i=0;
    while(true){
        string line ="";
        getline(cin,line);
        if(line.empty()) break;
        code.push_back(line);
    }
    cout<<"The machine code of the instructions is:"<<endl;
    for(int i=0;i<code.size();i++){
        parsing(code[i]);
    }
    
    pipeline temp;
    temp.run_pipeline();
    printGPR();
    //Only the memory addresses have been used would be printed
    printDM();
    cycles--;
    cout<<"No. of instructions executed = "<<instructions<<endl;
    cout<<"No. of cycles = "<<cycles<<endl;
    cout<<"CPI = "<<float(cycles)/instructions;
    return 0;
}