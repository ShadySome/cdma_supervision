/*
    Copyright 2018 0KIMS association.

    This file is part of circom (Zero Knowledge Circuit Compiler).

    circom is a free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    circom is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
    License for more details.

    You should have received a copy of the GNU General Public License
    along with circom. If not, see <https://www.gnu.org/licenses/>.
*/
pragma circom 2.0.0;

include "bitify.circom";
include "escalarmulfix.circom";

template BabyAdd() {
    signal input x1;
    signal input y1;
    signal input x2;
    signal input y2;
    signal output xout;
    signal output yout;

    signal beta;
    signal gamma;
    signal delta;
    signal tau;

    var a = 168700;
    var d = 168696;

    beta <== x1*y2;
    gamma <== y1*x2;
    delta <== (-a*x1+y1)*(x2 + y2);
    tau <== beta * gamma;

    xout <-- (beta + gamma) / (1+ d*tau);
    (1+ d*tau) * xout === (beta + gamma);

    yout <-- (delta + a*beta - gamma) / (1-d*tau);
    (1-d*tau)*yout === (delta + a*beta - gamma);
}

template BabyDbl() {
    signal input x;
    signal input y;
    signal output xout;
    signal output yout;

    component adder = BabyAdd();
    adder.x1 <== x;
    adder.y1 <== y;
    adder.x2 <== x;
    adder.y2 <== y;

    adder.xout ==> xout;
    adder.yout ==> yout;
}


template BabyCheck() {
    signal input x;
    signal input y;

    signal x2;
    signal y2;

    var a = 168700;
    var d = 168696;

    x2 <== x*x;
    y2 <== y*y;

    a*x2 + y2 === 1 + d*x2*y2;
}

// Extracts the public key from private key
template BabyPbk() {
    signal input  in;
    signal output Ax;
    signal output Ay;

    var BASE8[2] = [
        5299619240641551281634865583518297030282874472190772894086521144482721001553,
        16950150798460657717958625567821834550301663161624707787222815936182638968203
    ];

    component pvkBits = Num2Bits(253);
    pvkBits.in <== in;

    component mulFix = EscalarMulFix(253, BASE8);

    var i;
    for (i=0; i<253; i++) {
        mulFix.e[i] <== pvkBits.out[i];
    }
    Ax  <== mulFix.out[0];
    Ay  <== mulFix.out[1];
}

//点乘, a为n bit的数
template BabyMul(n){
    signal input a[n];
    signal input x;
    signal input y;

    signal output xout;
    signal output yout;

    var i;
    
    component babydbls[n];
    component babyadds[n];
    babydbls[0] = BabyDbl();
    babyadds[0] = BabyAdd();

    babydbls[0].x <== x;
    babydbls[0].y <== y;

    babyadds[0].x1 <== 0;
    babyadds[0].y1 <== 1;

    babyadds[0].x2 <== x * a[0];
    babyadds[0].y2 <== y * a[0];

    for(i = 1;i < n;i ++)
    {
        babydbls[i] = BabyDbl();
        babyadds[i] = BabyAdd();
        //double
        babydbls[i].x <== babydbls[i-1].xout;
        babydbls[i].y <== babydbls[i-1].yout;
        //a_bits[i] == 1则累加double的值，等于0则不累加
        babyadds[i].x1 <== babyadds[i-1].xout;
        babyadds[i].y1 <== babyadds[i-1].yout;
        babyadds[i].x2 <== babydbls[i-1].xout * a[i];
        babyadds[i].y2 <== babydbls[i-1].yout * a[i]+(1-a[i]);
    }
    babyadds[n-1].xout ==> xout;
    babyadds[n-1].yout ==> yout;
}