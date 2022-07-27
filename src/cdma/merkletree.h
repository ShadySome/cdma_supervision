// 监管方将注册公司的公钥以及码片信息取哈希作为叶子结点，组织成一颗Merkle Tree

#include "zcash/IncrementalMerkleTree.hpp";
#include "crypto/sha256.h";
#include "uint256.h";
#include "zcash/Zcash.h";
#include "twisted-edwards/Point.h";
using namespace libzcash;
using namespace NTL;
// template <size_t Depth, typename Hash>
class MerkleTree
{  
private:
    libzcash::IncrementalMerkleTree<SAPLING_INCREMENTAL_MERKLE_TREE_DEPTH,SHA256Compress> tree;
    
public:
    MerkleTree( ){};
    //封装一下，将码片信息和公钥取哈希，然后插入到Merkle Tree中
    void add_leaf(NTL::Point p,uint256 pk);

    SHA256Compress root()const{
        return tree.root();
    }
    template<size_t D, typename H>
    void friend operator== (const MerkleTree& a,
                        const MerkleTree& b);

    ~MerkleTree();
};
