export const australiaGraph = {
    nodes: [
        {id: 'WA'},
        {id: 'NT'},
        {id: 'SA'},
        {id: 'NSW'},
        /* COMMENTED OUT TO IMPROVE SPEED */
        // {id: 'Q'},
        // {id: 'V'},
        // {id: 'T'}
    ],
    edges: [
        {from: 'WA', to: 'NT'},
        {from: 'WA', to: 'SA'},
        {from: 'NT', to: 'SA'},
        {from: 'NSW', to: 'SA'},
        /* COMMENTED OUT TO IMPROVE SPEED */
        // {from: 'NT', to: 'Q'},
        // {from: 'SA', to: 'Q'},
        // {from: 'Q', to: 'NSW'},
        // {from: 'NSW', to: 'V'},
        // {from: 'SA', to: 'V'}
    ]
}