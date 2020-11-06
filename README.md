# A geant4 simulation program modified from example/extended/optical/OpNovice2

## Geometry

A uniform water medium as physical world. The water is set up with a material property table which describes optical property of water (presently just copied from OpNovice2).

An array of DOMs distributed in water.

Export the geometry by `G4GDMLParser` as in `example/extended/persistency/gdml/G02`.

<!-- 

Old code, need update for new usages.

## Physics list

Physics list with only EM cascade process and optical process.

## User Action

Cut and store medium energy particles.

## Analysis

Used to dump the medium energy particles. -->