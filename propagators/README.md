# PGC - Propagators

## Setup & Installation

- Install Node.js from https://nodejs.org/en/download/

```
git clone https://github.com/rafaelcalpena/pgc  &&
cd pgc                                          &&
npm ci                                          &&  # Install package dependencies
npm start                                           # Start Site
```

## Options

| ENV VAR | Description    | Options                                                                                                        |
|---------|----------------|----------------------------------------------------------------------------------------------------------------|
| ALG     | Algorithm Type | `normal` (genTree, default),  `simple` (simpleGenTree), `symmetric` (genTreeSym, in development)               |
| HEU     | Heuristic Type | `alphabetical` (sort var names and values, default), `SMF` (Smallest Maybe First), `first` (get first tuple), `random`    |
| EX      | Problem Example | `article` (problem for article, default), `diff1`, `diff2`, `australia` (graph problem)  

## Examples
* SimpleGenTree with Article Example
`ALG=simple npm start`

* GenTree with Article Example
`npm start`

* Australia Map with SimpleGenTree
`ALG=simple EX=australia npm start`

* Australia Map with GenTree and SMF heuristic
`EX=australia HEU=SMF npm start`