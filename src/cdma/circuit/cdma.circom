pragma circom 2.0.3;

include "./circuits/merkle_proof.circom";
include "./circuits/mimcsponge.circom";
include "./circuits/babyjub.circom";

template CDMAProof(levels,n_bits)
{
    signal input gali_x;
    signal input gali_y;
    signal input v[n_bits];
    signal input sk;
    signal input pk;
    signal input root;
    signal input path_index[levels];
    signal input path_elements[levels];
    signal input galiv_x;
    signal input galiv_y;


    //验证 gali_v 的正确性
    component multiplier = BabyMul(n_bits);
    for(var i = 0;i < n_bits;i ++)
    {
        v[i] ==> multiplier.a[i];
    }
    gali_x ==> multiplier.x;
    gali_y ==> multiplier.y;

    multiplier.xout === galiv_x;
    multiplier.yout === galiv_y;


    //验证pk sk
    component mimcsponge1 = MiMCSponge(1,220,1);
    sk ==> mimcsponge1.ins[0];
    0 ==> mimcsponge1.k;
    pk === mimcsponge1.outs[0];


    //验证在merkle tree上
    component merkle_proof = MerkleProof(levels);
    component mimcsponge2 = MiMCSponge(3,220,1);
    pk ==> mimcsponge2.ins[0];
    gali_x ==> mimcsponge2.ins[1];
    gali_y ==> mimcsponge2.ins[2];
    0 ==> mimcsponge2.k;

    mimcsponge2.outs[0] ==> merkle_proof.leaf;
    root ==> merkle_proof.root;
    for(var i = 0;i < levels;i ++)
    {
        path_index[i] ==> merkle_proof.path_index[i];
        path_elements[i] ==> merkle_proof.path_elements[i];
    }
}

component main {public [root,galiv_x,galiv_y]} = CDMAProof(4,10);