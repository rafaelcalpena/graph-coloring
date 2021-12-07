import { Domain } from "./classes";
import COLORS from "./colors";
import { SMF } from "./heuristic";
import { check } from "./operations";
import { Problem } from "./problems";
import { CBidirectionalTupleSet } from "./utils/CBidirectionalTupleSet";
import { convertToTupleSet } from "./utils/conversions";
import { filter, union } from "./utils/domain-utils";

const getFirstValue = <T>(set: Set<T>) => {
    for (let item of set) {
        return item;
    }
}

const step = (problem: Problem, domain: Domain<number>, choices: Domain<number>): Array<[string, number][]> => {
    if (domain.size === 0) {
        let isValid = true;
        for (let [t1, op, t2] of problem.constraints) {
            let v1 = getFirstValue(choices.get(t1))
            let v2 = getFirstValue(choices.get(t2));
            if (!check[op](v1, v2)) {
                isValid = false;
            }
        }
        if (isValid) {
            return [[...choices]];
        }
        return [];
    }
    
    const pickedVar = SMF(domain);
    const possibleValues = filter(domain, (k, v) => k === pickedVar[0]);
    const remaining = filter(domain, (k, v) => k !== pickedVar[0]);
    let solutions: Array<[string, number][]> = [];
    let hasFound = false;
    for (let [key, value] of possibleValues) {
        let newChoices: Domain<number> = new CBidirectionalTupleSet(choices)
        newChoices.add(key, value);
        let r = step(problem, remaining, newChoices);
        solutions = [...solutions, ...r];
        /* Early exit */
        if (r.length > 0 && process.env.EARLY_EXIT === '1') {
            return solutions
        }        
    }
    return solutions;
}

/* Solve problem by using brute force */
export const solveProblem = (problem: Problem) => {
    console.log('begin brute force approach');
    console.time('brute force')    
    let initialDomains = new CBidirectionalTupleSet(convertToTupleSet(problem.domains))
    const choices: Domain<number> = new CBidirectionalTupleSet([]);
    let solutions = step(problem, initialDomains, choices);

    const dataFolder = require('path').join(__filename, '../../data/');    
    require('fs').writeFileSync(
        require('path').join(dataFolder, './solver.js'),
        `const solverResult = ` + JSON.stringify(
            solutions,
            null,
            2
        ),
        'utf8'
    );    
    console.timeEnd('brute force')   
    let maxK = COLORS;
    console.log('finished brute force approach', solutions.length, 'of', Math.pow(maxK, Object.keys(problem.domains).length));
    let chromaticNumber = maxK;
    let chromaticItemIndex = -1;
    solutions.forEach((sol, index) => {
        let vs = new Set<number>();
        sol.forEach(([k, v]) => {
            vs.add(v);
        })
        if (vs.size <= chromaticNumber) {
            chromaticItemIndex = index
            chromaticNumber = vs.size
        }        
    })
    console.log({chromaticNumber, chromaticItemIndex, example: solutions[chromaticItemIndex]});
}