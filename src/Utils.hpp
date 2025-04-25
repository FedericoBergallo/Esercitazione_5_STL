#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PoligonalLibrary
{
// Import the poligonal mesh and test if the mesh is correct
// mesh: a PoligonalMesh struct
// return the result of the reading, true if is success, false otherwise
bool ImportMesh(PoligonalMesh& mesh);

// Import the Cell0D properties from Cell0Ds.csv file
// mesh: a PoligonalMesh struct
// return the result of the reading, true if is success, false otherwise
bool ImportCell0Ds(PoligonalMesh& mesh);

// Import the Cell1D properties from Cell1Ds.csv file
// mesh: a PoligonalMesh struct
// return the result of the reading, true if is success, false otherwise
bool ImportCell1Ds(PoligonalMesh& mesh);

// Import the Cell2D properties from Cell2Ds.csv file
// mesh: a PoligonalMesh struct
// return the result of the reading, true if is success, false otherwise
bool ImportCell2Ds(PoligonalMesh& mesh);

}

