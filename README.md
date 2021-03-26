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
| ALG | Algorithm Type | * `normal` (genTree, default),   * `simple` (simpleGenTree), * `symmetric` (genTreeSym, in development)              |
|-----|----------------|----------------------------------------------------------------------------------------------------------------------|
| HEU | Heuristic Type | * `SMF` (Smallest Maybe First, default), * `first` (get first tuple),  * `alphabetical` (sort var names and values)  |
|     |                |                                                                                                                      |
|     |                |                                                                                                                      |