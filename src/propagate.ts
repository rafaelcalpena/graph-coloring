import { Constraint, Domain } from "./classes";
import { check } from "./operations";
import { CBidirectionalTupleSet } from "./utils/CBidirectionalTupleSet";

export const propagate = <T>(constraint: Constraint, domain: Domain<T>) : Domain<T> => {
    /* Check and remove literals */
    const {a, b, op} = constraint;
    const domainA = domain.get(a);
    const domainB = domain.get(b);
    let deletions: Domain<T> = new CBidirectionalTupleSet();
    domainA.forEach(vA => {
        let satisfiesOne = false;
        domainB.forEach(vB => {
            if (check[op](vA, vB)) {
                satisfiesOne = true;
            }
        })
        if (satisfiesOne === false) {
            deletions.add(a, vA);
        }
    })

    domainB.forEach(vB => {
        let satisfiesOne = false;
        domainA.forEach(vA => {
            if (check[op](vA, vB)) {
                satisfiesOne = true;
            }
        })
        if (satisfiesOne === false) {
            deletions.add(b, vB);
        }
    })    

    return deletions;
}