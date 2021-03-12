const graphResult = [
  [
    {
      "data": {
        "id": "0",
        "label": "<Test:(y,1), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "0",
        "id": "0-1",
        "target": "1",
        "label": "1 &#8712; D(y)"
      }
    },
    {
      "data": {
        "id": "1",
        "label": "<Test:(y,0), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "1",
        "id": "1-2",
        "target": "2",
        "label": "0 &#8712; D(y)"
      }
    },
    {
      "data": {
        "id": "2",
        "label": "<Test:(x,1), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "2",
        "id": "2-3",
        "target": "3",
        "label": "1 &#8712; D(x)"
      }
    },
    {
      "data": {
        "id": "3",
        "label": "<Test:(x,0), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "3",
        "id": "3-4",
        "target": "4",
        "label": "0 &#8712; D(x)"
      }
    },
    {
      "data": {
        "id": "4",
        "label": "<Test:(null), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "3",
        "id": "3-5",
        "target": "5",
        "label": "0 &#8713; D(x)"
      }
    },
    {
      "data": {
        "id": "5",
        "label": "<Test:(null), Prune:{(y,1)}>"
      }
    },
    {
      "data": {
        "source": "2",
        "id": "2-6",
        "target": "6",
        "label": "1 &#8713; D(x)"
      }
    },
    {
      "data": {
        "id": "6",
        "label": "<Test:(null), Prune:{(y,0)}>"
      }
    },
    {
      "data": {
        "source": "1",
        "id": "1-7",
        "target": "7",
        "label": "0 &#8713; D(y)"
      }
    },
    {
      "data": {
        "id": "7",
        "label": "<Test:(null), Prune:{(x,1)}>"
      }
    },
    {
      "data": {
        "source": "0",
        "id": "0-8",
        "target": "8",
        "label": "1 &#8713; D(y)"
      }
    },
    {
      "data": {
        "id": "8",
        "label": "<Test:(null), Prune:{(x,0)}>"
      }
    }
  ]
]