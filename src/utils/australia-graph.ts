export const australiaGraph = {
    nodes: [
        {id: 'WA'},
        {id: 'NT'},
        {id: 'SA'},
        {id: 'NSW'},
        {id: 'Q'},
        {id: 'V'},
        {id: 'T'}
    ],
    edges: [
        {from: 'WA', to: 'NT'},
        {from: 'WA', to: 'SA'},
        {from: 'NT', to: 'SA'},
        {from: 'NT', to: 'Q'},
        {from: 'SA', to: 'Q'},
        {from: 'Q', to: 'NSW'},
        {from: 'NSW', to: 'SA'},
        {from: 'NSW', to: 'V'},
        {from: 'SA', to: 'V'}
    ]
}