#include <iostream>
#include <unistd.h>
#include <sys/types.h>
using namespace std;
const int N = 10000;

void generateCDMAProof()
{
    char line[N];
    FILE *fp1, *fp2;
    // 编译电路并计算witness，生成final zkey
    string cmd1 = "cd circuit && circom cdma.circom --r1cs --wasm --sym && cd cdma_js && node generate_witness.js circuit.wasm ../input.json ../witness.wtns && cd .. && snarkjs plonk setup circuit.r1cs pot12_final.ptau circuit_final.zkey";
    // 把final zkey 导出得到verification_key 然后生成proof.json
    string cmd2 = "snarkjs zkey export verificationkey circuit_final.zkey verification_key.json && snarkjs plonk prove circuit_final.zkey witness.wtns proof.json public.json";
    const char* sysCommand1 = cmd1.data();
    const char* sysCommand2 = cmd2.data();
    //执行上述指令
    if ((fp1 = popen(sysCommand1, "r")) == NULL || (fp2 = popen(sysCommand2,"r")) == NULL) {
        cout << "can not execute the command to generate proof" << endl;
        return;
    }
    while (fgets(line, sizeof(line)-1, fp1) != NULL){
        cout << line ;
    }
    pclose(fp1);
    pclose(fp2);
}
void verifyCDMAProof()
{
    char line[N];
    FILE *fp;
    string cmd = "snarkjs plonk verify verification_key.json public.json proof.json";
    const char* sysCommand = cmd.data();

    if((fp = popen(sysCommand,"r")) == NULL)
    {
        cout <<"can not execute the command to verify proof";
        return ;
    }

    while(fgets(line,sizeof(line)-1,fp) != NULL)
    {
        cout <<line;
    }
    pclose(fp);
}