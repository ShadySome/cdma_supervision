const snarkjs = require("snarkjs");
const fs = require("fs");

async function run() {
    var start_proof = new Date().getTime();
    const { proof, publicSignals } = await snarkjs.groth16.fullProve({
        "gali_x":"17777552123799933955779906779655732241715742912184938656739573121738514868268",
        "gali_y":"2626589144620713026669568689430873010625803728049924121243784502389097019475",
        "v":[1,0,0,0,0,0,0,0,0,0],
        "sk":"123456789",
        "pk":"9787255622629022721748691510369968153577112798182035597878947789353381255294",
        "root":"13406544191306087207323095557859289619627565089723425906658054511413831379151",
        "path_index":[0,0,0,0],
        "path_elements":[
            "987654321",
            "3089049976446759208209687144383339131770406080938200585344893880883658883072",
            "15030227873835483741808756503983800618199432934547562834198190754996431093760",
            "17150355945451186653579834436695516799650433845386473893668425316223617597440"
        ],
        "galiv_x":"17777552123799933955779906779655732241715742912184938656739573121738514868268",
        "galiv_y":"2626589144620713026669568689430873010625803728049924121243784502389097019475"
    },
         "./cdma_js/cdma.wasm", "cdma_final.zkey");
    var end_proof = new Date().getTime();
    console.log("Proof: ");
    console.log(JSON.stringify(proof, null, 1));
    console.log("证明生成时间:",end_proof - start_proof);

    
    const vKey = JSON.parse(fs.readFileSync("verification_key.json"));
    var start_veri = new Date().getTime();
    const res = await snarkjs.groth16.verify(vKey, publicSignals, proof);
    var end_veri = new Date().getTime();
    console.log("验证时间：(ms)");
    console.log(end_veri-start_veri);
    if (res === true) {
        console.log("Verification OK");
    } else { 
        console.log("Invalid proof");
    }
}
run().then(() => {
    process.exit(0);
});
