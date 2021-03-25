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
  ],
  [
    {
      "data": {
        "id": "9",
        "label": "<Test:(z,3), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "9",
        "id": "9-10",
        "target": "10",
        "label": "3 &#8712; D(z)"
      }
    },
    {
      "data": {
        "id": "10",
        "label": "<Test:(z,1), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "10",
        "id": "10-11",
        "target": "11",
        "label": "1 &#8712; D(z)"
      }
    },
    {
      "data": {
        "id": "11",
        "label": "<Test:(y,1), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "11",
        "id": "11-12",
        "target": "12",
        "label": "1 &#8712; D(y)"
      }
    },
    {
      "data": {
        "id": "12",
        "label": "<Test:(y,0), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "12",
        "id": "12-13",
        "target": "13",
        "label": "0 &#8712; D(y)"
      }
    },
    {
      "data": {
        "id": "13",
        "label": "<Test:(null), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "12",
        "id": "12-14",
        "target": "14",
        "label": "0 &#8713; D(y)"
      }
    },
    {
      "data": {
        "id": "14",
        "label": "<Test:(null), Prune:{(z,1)}>"
      }
    },
    {
      "data": {
        "source": "11",
        "id": "11-15",
        "target": "15",
        "label": "1 &#8713; D(y)"
      }
    },
    {
      "data": {
        "id": "15",
        "label": "<Test:(null), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "10",
        "id": "10-16",
        "target": "16",
        "label": "1 &#8713; D(z)"
      }
    },
    {
      "data": {
        "id": "16",
        "label": "<Test:(y,1), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "16",
        "id": "16-17",
        "target": "17",
        "label": "1 &#8712; D(y)"
      }
    },
    {
      "data": {
        "id": "17",
        "label": "<Test:(y,0), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "17",
        "id": "17-18",
        "target": "18",
        "label": "0 &#8712; D(y)"
      }
    },
    {
      "data": {
        "id": "18",
        "label": "<Test:(null), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "17",
        "id": "17-19",
        "target": "19",
        "label": "0 &#8713; D(y)"
      }
    },
    {
      "data": {
        "id": "19",
        "label": "<Test:(null), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "16",
        "id": "16-20",
        "target": "20",
        "label": "1 &#8713; D(y)"
      }
    },
    {
      "data": {
        "id": "20",
        "label": "<Test:(null), Prune:{}>"
      }
    },
    {
      "data": {
        "source": "9",
        "id": "9-21",
        "target": "21",
        "label": "3 &#8713; D(z)"
      }
    },
    {
      "data": {
        "id": "21",
        "label": "<Test:(null), Prune:{(y,1)}>"
      }
    }
  ]
]