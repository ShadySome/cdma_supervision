#ifndef PARAMS_H
#define PARAMS_H

#include <NTL/ZZ.h>


namespace NTL {
    // Набор параметров id-tc26-gost-3410-2012-256-paramSetA
    // https://docplayer.ru/46408167-Tehnicheskiy-komitet-026-zadanie-parametrov-skruchennyh-ellipticheskih-krivyh-edvardsa-v-sootvetstvii-s-gost-r.html
    const ZZ a = to_ZZ("1");
    const ZZ d = to_ZZ("9706598848417545097372247223557719406784115219466060233080913168975159366771");
    const ZZ p = to_ZZ("21888242871839275222246405745257275088548364400416034343698204186575808495617");
    //测试中测试点乘时候的标量
    const ZZ q = to_ZZ("28948022309329048855892746252171976963338560298092253442512153408785530358887");
    //随便选的曲线上的一个点
    const ZZ u = to_ZZ("13");
    const ZZ v = to_ZZ("43779144989398987843428779166090436406934195821915183574454224403186176950503");
}

#endif //PARAMS_H
