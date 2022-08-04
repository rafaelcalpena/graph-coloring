module.exports = {
    graphs: [
        {name: "./dimacs/myciel3.col", optimal: 4}, // 20 edges, 11 vertices
        {name: "./dimacs/myciel4.col", optimal: 5}, // 71 edges, 23 vertices
        {name: "./dimacs/myciel5.col", optimal: 6}, // 236 edges, 47 vertices
        {name: "./dimacs/queen5_5.col", optimal: 5}, // 320 edges, 25 vertices
        {name: "./dimacs/jean.col", optimal: 10}, // 508 edges, 80 vertices
        {name: "./dimacs/queen6_6.col", optimal: 7}, // 580 edges, 36 vertices
        {name: "./dimacs/huck.col", optimal: 11}, // 602 edges, 74 vertices
        {name: "./dimacs/myciel6.col", optimal: 7}, // 755 edges, 95 vertices
        {name: "./dimacs/miles250.col", optimal: 8}, // 774 edges, 128 vertices
        {name: "./dimacs/david.col", optimal: 11}, // 812 edges, 87 vertices
        {name: "./dimacs/queen7_7.col", optimal: 7}, // 952 edges, 49 vertices
        {name: "./dimacs/anna.col", optimal: 11}, // 986 edges, 138 vertices
        {name: "./dimacs/games120.col", optimal: 9}, // 1276 edges, 120 vertices
        {name: "./dimacs/queen8_8.col", optimal: 9}, // 1456 edges, 64 vertices
        {name: "./dimacs/queen9_9.col", optimal: 10}, // 2112 edges, 81 vertices
        {name: "./dimacs/miles500.col", optimal: 20}, // 2340 edges, 128 vertices
        {name: "./dimacs/myciel7.col", optimal: 8}, // 2360 edges, 191 vertices
        {name: "./dimacs/queen8_12.col", optimal: 12}, // 2736 edges, 96 vertices
        {name: "./dimacs/queen10_10.col", optimal: null}, // 2940 edges, 100 vertices
        {name: "./dimacs/homer.col", optimal: 13}, // 3256 edges, 561 vertices
        {name: "./dimacs/zeroin.i.3.col", optimal: 30}, // 3540 edges, 206 vertices
        {name: "./dimacs/zeroin.i.2.col", optimal: 30}, // 3541 edges, 211 vertices
        {name: "./dimacs/mulsol.i.2.col", optimal: 31}, // 3885 edges, 188 vertices
        {name: "./dimacs/mulsol.i.3.col", optimal: 31}, // 3916 edges, 184 vertices
        {name: "./dimacs/mulsol.i.1.col", optimal: 49}, // 3925 edges, 197 vertices
        {name: "./dimacs/mulsol.i.4.col", optimal: 31}, // 3946 edges, 185 vertices
        {name: "./dimacs/queen11_11.col", optimal: 11}, // 3960 edges, 121 vertices
        {name: "./dimacs/mulsol.i.5.col", optimal: 31}, // 3973 edges, 186 vertices
        {name: "./dimacs/zeroin.i.1.col", optimal: 49}, // 4100 edges, 211 vertices
        {name: "./dimacs/miles750.col", optimal: 31}, // 4226 edges, 128 vertices
        {name: "./dimacs/queen12_12.col", optimal: null}, // 5192 edges, 144 vertices
        {name: "./dimacs/le450_5a.col", optimal: 5}, // 5714 edges, 450 vertices
        {name: "./dimacs/le450_5b.col", optimal: 5}, // 5734 edges, 450 vertices
        {name: "./dimacs/miles1000.col", optimal: 42}, // 6432 edges, 128 vertices
        {name: "./dimacs/queen13_13.col", optimal: 13}, // 6656 edges, 169 vertices
        {name: "./dimacs/le450_15a.col", optimal: 15}, // 8168 edges, 450 vertices
        {name: "./dimacs/le450_15b.col", optimal: 15}, // 8169 edges, 450 vertices
        {name: "./dimacs/le450_25a.col", optimal: 25}, // 8260 edges, 450 vertices
        {name: "./dimacs/le450_25b.col", optimal: 25}, // 8263 edges, 450 vertices
        {name: "./dimacs/queen14_14.col", optimal: null}, // 8372 edges, 196 vertices
        {name: "./dimacs/fpsol2.i.3.col", optimal: 30}, // 8688 edges, 425 vertices
        {name: "./dimacs/fpsol2.i.2.col", optimal: 30}, // 8691 edges, 451 vertices
        {name: "./dimacs/le450_5d.col", optimal: 5}, // 9757 edges, 450 vertices
        {name: "./dimacs/le450_5c.col", optimal: 5}, // 9803 edges, 450 vertices
        {name: "./dimacs/queen15_15.col",  optimal: null},// 10360 edges, 225 vertices
        {name: "./dimacs/miles1500.col",  optimal: 73},// 10396 edges, 128 vertices
        {name: "./dimacs/fpsol2.i.1.col",  optimal: 65},// 11654 edges, 496 vertices
        {name: "./dimacs/queen16_16.col",  optimal: null},// 12640 edges, 256 vertices
        {name: "./dimacs/inithx.i.3.col",  optimal: 31},// 13969 edges, 621 vertices
        {name: "./dimacs/inithx.i.2.col",  optimal: 31},// 13979 edges, 645 vertices
        {name: "./dimacs/school1_nsh.col",  optimal: null},// 14612 edges, 352 vertices
        {name: "./dimacs/le450_15c.col",  optimal: 15},// 16680 edges, 450 vertices
        {name: "./dimacs/le450_15d.col",  optimal: 15},// 16750 edges, 450 vertices
        {name: "./dimacs/le450_25c.col",  optimal: 25},// 17343 edges, 450 vertices
        {name: "./dimacs/le450_25d.col",  optimal: 25},// 17425 edges, 450 vertices
        {name: "./dimacs/inithx.i.1.col",  optimal: 54},// 18707 edges, 864 vertices
        {name: "./dimacs/school1.col",  optimal: null},// 19095 edges, 385 vertices
        {name: "./dimacs/latin_square_10.col", optimal: null} // 307350 edges, 900 vertices
    ],
    algorithms: [
        {name: 'greedy'},
        {name: 'dsatur'},
        {name: 'greedy-backtracking'},
        {name: 'dsatur-backtracking'},
        {name: 'dsatur-sewell'},
        {name: 'dsatur-pass-always'},
        {name: 'dsatur-pass-conditional'},
        {name: 'dsatur-gac-0'},
        {name: 'dsatur-gac-1'},
        {name: 'dsatur-gac-2'}
    ]
}