pragma circom 2.0.3;
include "./mimcsponge.circom";


template HashLeftRight() {
    signal input left;
    signal input right;

    signal output hash;
    component hasher = MiMCSponge(2, 220, 1);
    left ==> hasher.ins[0];
    right ==> hasher.ins[1];
    hasher.k <== 0;

    hash <== hasher.outs[0];
}
// component main = HashLeftRight();
template Selector() {
    signal input input_elem;
    signal input path_elem;
    signal input path_index;

    signal output left;
    signal output right;

    signal left_selector_1;

    signal left_selector_2;

    signal right_selector_1;

    signal right_selector_2;

    path_index * (1-path_index) === 0;

    left_selector_1 <== (1 - path_index)*input_elem;
    left_selector_2 <== (path_index)*path_elem;
    right_selector_1 <== (path_index)*input_elem;
    right_selector_2 <== (1 - path_index)*path_elem;

    left <== left_selector_1 + left_selector_2;
    right <== right_selector_1 + right_selector_2;
}

template GetMerleRoot(levels) {

    signal input leaf;
    signal input path_index[levels];
    signal input path_elements[levels];

    signal output out;

    component selectors[levels];
    component hashers[levels];

    selectors[0] = Selector();
    hashers[0] = HashLeftRight();

    leaf ==> selectors[0].input_elem;
    path_index[0] ==> selectors[0].path_index;
    path_elements[0] ==> selectors[0].path_elem;

    selectors[0].left ==> hashers[0].left;
    selectors[0].right ==> hashers[0].right;

    for (var i = 1; i < levels; i++) {
        selectors[i] = Selector();
        hashers[i] = HashLeftRight();

        path_index[i] ==> selectors[i].path_index;
        path_elements[i] ==> selectors[i].path_elem;
        hashers[i-1].hash ==> selectors[i].input_elem;

        selectors[i].left ==> hashers[i].left;
        selectors[i].right ==> hashers[i].right;
    }

    out <== hashers[levels-1].hash;
}

template MerkleProof(levels)
{
    signal input root;
    signal input leaf;
    signal input path_index[levels];
    signal input path_elements[levels]; 

    // signal output out;
    component get_merkle_root = GetMerleRoot(levels);
    leaf ==> get_merkle_root.leaf;

    for(var i = 0;i < levels;i ++)
    {
        path_elements[i] ==> get_merkle_root.path_elements[i];
        path_index[i] ==> get_merkle_root.path_index[i];
    }

    get_merkle_root.out === root;
    // out <== getmerkle.out;
}