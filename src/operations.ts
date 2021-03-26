export const DIFFERENT = '!=';
export const OR = '||';
export type Operation = typeof DIFFERENT | typeof OR;

/* Checks for ensuring some constraint is valid or not */
export const check = {
    [DIFFERENT]: <T>(a: T, b: T) => {
        return a !== b;
    },
    [OR]: <T>(a: T, b: T) => {
        return a || b;
    }
}