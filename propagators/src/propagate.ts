import { Constraint, Domain } from "./classes";
import { check } from "./operations";
import { CBidirectionalTupleSet } from "./utils/CBidirectionalTupleSet";
import { difference, union } from "./utils/domain-utils";

const addToAgenda = (c: Constraint, agenda: Set<Constraint>) : Set<Constraint> => {
    /* TODO: Improve performance with tripletSet */
    agenda.forEach(d => {
        if ((d.a === c.a) && (d.op === c.op) && (d.b === c.b)) {
            return agenda;
        }
    })
    agenda.add(c);
    return agenda;
}

const removeFromAgenda = (c: Constraint, agenda: Set<Constraint>) : Set<Constraint> => {
    /* TODO: Improve performance with tripletSet */
    agenda.forEach(d => {
        if ((d.a === c.a) && (d.op === c.op) && (d.b === c.b)) {
            agenda.delete(d);
            return agenda;
        }
    })
    return agenda;
}

const propagateToRightHandSide = (a: string, constraints: Constraint[], agenda: Set<Constraint>) => {
    constraints.forEach(c => {
        if (c.b === a) {
            addToAgenda(c, agenda)
        }
    })
}

export const propagate = <T>(initialConstraint: Constraint | Constraint[], domain: Domain<T>, debug?: string) : Domain<T> => {

    let constraints = !Array.isArray(initialConstraint) ? [initialConstraint] : initialConstraint;


    let agenda: Set<Constraint> = new Set();

    constraints.forEach(constraint => {
        /* Add normal and inverse propagations */
        addToAgenda(new Constraint(constraint.a, constraint.op, constraint.b), agenda);
        /* TODO: change for > and < (get inverse ops) */
        addToAgenda(new Constraint(constraint.b, constraint.op, constraint.a), agenda);
    })

    let deletions: Domain<T> = new CBidirectionalTupleSet();

    for (let constraint of agenda) {
        const {a, b, op} = constraint;
        const domainA = domain.get(a);
        const domainB = domain.get(b);
        let forCheck = null;

        domainA.forEach(vA => {
            let satisfiesOne = false;
            domainB.forEach(vB => {
                if (check[op](vA, vB)) {
                    satisfiesOne = true;
                    /* TODO: Optimize, skip */
                }
            })
            if (satisfiesOne === false) {
                deletions.add(a, vA);
                /* mark for check */
                forCheck = a;
            }
        })  
        removeFromAgenda(constraint, agenda);
        if (forCheck != null) {
            propagateToRightHandSide(forCheck, constraints, agenda);
        }
    }

    return deletions;
}