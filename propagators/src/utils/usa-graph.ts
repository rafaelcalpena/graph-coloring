/* From: https://people.sc.fsu.edu/~jburkardt/datasets/states/state_neighbors.txt */

import { CBidirectionalTupleSet } from "./CBidirectionalTupleSet";

/* https://blogs.mathworks.com/cleve/2018/09/03/graph-object-of-48-usa-states/ */
const txt = 
`AL FL GA TN MS
AK
AZ NM UT NV CA
AR LA MS TN MO OK TX
CA AZ NV OR
CO NM OK KS NE WY UT
CT RI MA NY
DE NJ PA MD
DC MD VA
FL GA AL
GA SC NC TN AL FL
HI
ID WA OR NV UT WY MT
IL WI IA MO KY IN
IN IL KY OH MI
IA MN SD NE MO IL WI
KS OK MO NE CO
KY TN VA WV OH IN IL MO
LA MS AR TX
ME NH
MD DE PA WV VA DC
MA NH VT NY CT RI
MI WI IN OH
MN ND SD IA WI
MS AL TN AR LA
MO AR TN KY IL IA NE KS OK
MT ID WY SD ND
NE KS MO IA SD WY CO
NV AZ UT ID OR CA
NH VT MA ME
NJ NY PA DE
NM TX OK CO AZ
NY PA NJ CT MA VT
NC VA TN GA SC
ND MT SD MN
OH MI IN KY WV PA
OK TX AR MO KS CO NM
OR CA NV ID WA
PA OH WV MD DE NJ NY
PR
RI MA CT
SC NC GA
SD NE IA MN ND MT WY
TN AL GA NC VA KY MO AR MS
TX LA AR OK NM
US
UT AZ CO WY ID NV 
VT NY MA NH
VA MD DC WV KY TN NC
WA OR ID
WV VA MD PA OH KY
WI MN IA IL MI
WY CO NE SD MT ID UT`

const txtToArrays = (txt: string) => {
    let lines = txt.split('\n').map(i => i.split(' ').filter(i => i !== ''));
    let edgesTuple = new CBidirectionalTupleSet<string, string>();
    let nodes = new Set<string>();
    lines.forEach(line => {
        for(let i = 0; i < line.length; i++) {
            let from = line[i];
            nodes.add(from);
            for (let j = i + 1; j < line.length; j++) {
                let to = line[j];
                if (edgesTuple.has(from, to) || edgesTuple.has(to, from)) {
                    continue;
                }
                edgesTuple.add(from, to);
            }
        }
    })

    return {
        edges: [...edgesTuple].map(pair => ({from: pair[0], to: pair[1]})),
        nodes: [...nodes].map(id => ({id}))
    }
}

export const usaGraph = txtToArrays(txt);