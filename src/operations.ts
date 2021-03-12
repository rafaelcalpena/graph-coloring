export const DIFFERENT = '!=';
export type Operation = typeof DIFFERENT;

/* Checks for ensuring some constraint is valid or not */
export const check = {
    [DIFFERENT]: <T>(a: T, b: T) => {
        return a !== b;
    }
}