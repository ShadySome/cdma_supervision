#include "supervisor.h"
#include "cdma/utils.h"
#include "twisted-edwards/calc.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <sodium.h>
#include <map>
#include <vector>
using json = nlohmann::json;

using namespace std;
using namespace NTL;

uint256 random_uint256()
{
    uint256 ret;
    randombytes_buf(ret.begin(), 32);

    return ret;
}
void Supervisor::generateCode()
{
    // int n = this->num_of_comp;
    int codes[N][N];
    //递归构建正交矩阵,先初始化一个2*2的正交矩阵
    codes[0][0] = 1;
    codes[0][1] = 1;
    codes[1][0] = 1;
    codes[1][1] = -1;
    int d = 2;
    while(d <= N)
    {
        for(int i = 0;i < d;i ++)
        {
            for(int j = 0; j < d; j++)
            {
                codes[d+i][j] = codes[i][j];
                codes[i][d+j] = codes[i][j];
                codes[d+i][d+j] = -codes[i][j];
            }
        }
        d *= 2;
    }
    for(int i = 0;i < N; i++)
    {
        vector<int> vec;
        for(int j = 0; j < N; j++)
             vec.push_back(codes[i][j]);
        this->code_matrix.push_back(vec);
    }
}

PublicParameter Supervisor::generatePP()
{
    pp.a = random_uint256();

    uint64_t n = num_of_comp;
    //choose random generator
    NTL::Point base_point;
    NTL::ZZ r = convertUint256ToZZ(random_uint256());
    NTL::Point p;
    p = NTL::binaryMethod(base_point,r);

    //用随机选取的生成元p，再随机生成生成元g,h 
    NTL::Point g = NTL::binaryMethod(p,convertUint256ToZZ(random_uint256()));
    NTL::Point h = NTL::binaryMethod(p,convertUint256ToZZ(random_uint256()));
    pp.g.push_back(g);
    pp.h.push_back(h);
    for(int i = 1;i < n;i ++)
    {
        pp.g.push_back(NTL::binaryMethod(pp.g[i-1],a));
        pp.h.push_back(NTL::binaryMethod(pp.h[i-1],a));
    }
    json j;
    ofstream ppfile;
    j["a"] = pp.a;
    j["g"] = {};
    

    return pp;
}

vector<NTL::Point> Supervisor::calcCodeEccForm()
{
    vector<NTL::Point> code_ecc_form;
    NTL::ZZ x = ZZ(0),y = ZZ(1),z = ZZ(0);
    for(int i = 0;i < num_of_comp;i ++)
    {
        Point p(x,y,z);
        for(int j = 0; j < num_of_comp; j ++)        
        {
            NTL::Point temp;
            //code * (g[i] + h[i])
            temp = NTL::binaryMethod(NTL::addPoints(pp.g[j],pp.h[j]),conv<ZZ>(code_matrix[i][j]));
            p = NTL::addPoints(p,temp);
        }
        code_ecc_form.push_back(p);
    }
    return code_ecc_form;
}
json constructJsonOfPoint(NTL::Point p)
{
    json j;
    j["X"] = conv<string>(p.X);
    j["Y"] = conv<string>(p.Y);
    return j;
}

void Supervisor::generateKCode()
{
    vector<NTL::ZZ> random_ks;
    for(int i = 0;i < num_of_comp; i ++)
    {
        random_ks.push_back(convertUint256ToZZ(random_uint256()));
    }
    vector<NTL::Point> gK; 
    vector<NTL::Point> hK;
    for(int i = 0;i < num_of_comp;i ++)
    {
        gK.push_back(NTL::binaryMethod(pp.g[i],random_ks[i]));
        hK.push_back(NTL::binaryMethod(pp.h[i],random_ks[i]));
    }
    //存到文件中
    ofstream gk_file,hk_file,k_file;
    gk_file.open("gK.json");
    hk_file.open("hK.json");
    k_file.open("k.json");
    for(int i = 0;i < gK.size();i ++)
    {
        gk_file << constructJsonOfPoint(gK[i]);
        hk_file << constructJsonOfPoint(hK[i]);
        k_file << conv<string>(random_ks[i]);
    }
    gk_file.close();
    hk_file.close();
    k_file.close();
}

void 
