#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PoligonalLibrary {

struct PoligonalMesh   //creo una struttura chimata poligonalMesh 
{
    unsigned int NumCell0Ds = 0; //inizializzo a 0 il numero di celle 
    std::vector<unsigned int> Cell0DsId = {}; //definisco un vettore che mi memorizza gli ID dei vari nodi
    Eigen::MatrixXd Cell0DsCoordinates = {}; //definisco una matrice che mi memorizza le coordinate dei nodi, size 3 x NumberCell0D
    std::map<unsigned int, list<unsigned int>> MarkerCell0Ds = {}; //memorizzo i marker dei vari nodi

    unsigned int NumCell1Ds = 0; //inizializzo a 0 il numero di segmenti
    std::vector<unsigned int> Cell1DsId = {}; //definisco un vettore che mi memorizza gli ID dei vari segmenti
    Eigen::MatrixXi Cell1DsExtrema = {}; //matrice che memorizza l'id del nodo di partenza e di quello in arrivo
    std::map<unsigned int, list<unsigned int>> MarkerCell1Ds = {}; //in un dizionario memorizzo i marker di ciascun segmento

    unsigned int NumCell2Ds = 0; //inizializzo a 0 il numero di segmenti
    std::vector<unsigned int> Cell2DsId = {};//definisco un vettore che mi memorizza gli ID delle varie celle
	std::vector<unsigned int> Cell2DsMarker = {};
	std::vector<unsigned int> Cell2DsNumVertices={};//inizializzo un vettore che mi permette di memorizzare
    std::vector<vector<unsigned int>> Cell2DsVertices = {}; ///< Cell2D Vertices indices, size 1 x NumberCell2DVertices[NumberCell2D]
    std::vector<unsigned int> Cell2DsNumEdges = {};
	std::vector<vector<unsigned int>> Cell2DsEdges = {}; ///< Cell2D Cell1D indices, size 1 x NumberCell2DEdges[NumberCell2D]
};

}

