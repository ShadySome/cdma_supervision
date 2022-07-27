#include "merkletree.h";
#include "crypto/sha256.h"
#include "utils.h"
#include "nlohmann/json.hpp"
using namespace NTL;
using namespace libzcash;
using namespace std;
void MerkleTree::add_leaf(NTL::Point p,uint256 pk)
{
    //计算码片椭圆曲线形式 以及 公钥的哈希
    CSHA256 hasher;

    std::vector<unsigned char> x = convertZZToVectorLE(p.X);
    std::vector<unsigned char> y = convertZZToVectorLE(p.Y);

    hasher.Write(&x[0],x.size());
    hasher.Write(&y[0],y.size());
    hasher.Write(pk.begin(),32);

    uint256 result;
    hasher.FinalizeNoPadding(result.begin());

    tree.append(result);
}