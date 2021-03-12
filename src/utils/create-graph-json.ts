import { PropagatorTreeNode } from "../classes";

let id = 0;
/* TODO: Improve types */
export const createGraphJSON = <T>(node: PropagatorTreeNode<T>, step: any[] = [], parent?: number): any[] => {
    const item: any = {
        id: `${id}`,
        label: `<Test:(${node.test}), Prune:${node.prune}>`,
    };
    step.push({
        data: item
    });
    const parentId = id;
    id++;
    if (node.left) {
        step.push({
            data: {
                source: `${parentId}`,
                id: `${parentId}-${id}`,
                target: `${id}`,
                label: `${(node.test as [string, T])[1]} &#8712; D(${(node.test as [string, T])[0]})`
            }
        })
        createGraphJSON(node.left, step, id);
    }
    if (node.right) {
        step.push({
            data: {
                source: `${parentId}`,
                id: `${parentId}-${id}`,
                target: `${id}`,
                label: `${(node.test as [string, T])[1]} &#8713; D(${(node.test as [string, T])[0]})`
            }
        })       
        createGraphJSON(node.right, step, id);
    }
    return step;
}