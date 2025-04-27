#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PoligonalLibrary
{
bool ImportMesh(PoligonalMesh& mesh)  //controllo se ho aperto correttamente i file
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}
//***************************************************************************
//voglio estrarre le proprietà dal file csv relativo a cell0D, il quale contiene le informazioni sui NODI:
//ID=identifica di quale nodo sto parlando;
//Marker= indica quale funzione ha quel nodo (sta sul bordo, etc);
//X,Y=coordinate spaziali del nodo nel piano.
bool ImportCell0Ds(PoligonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");  //creo un oggetto di tipo ifstream per poter lavorare sul file

    if(file.fail())    //se non trova il file, restituisco errore
        return false;

    list<string> listLines;  //inizializzo una lista vuota

    string line;         //quindi leggo il file nel solito modo: creo una variabile stringa in cui memorizzo quello che leggo dal file
    while (getline(file, line))  //in questo modo leggo una riga ad ogni ciclo
        listLines.push_back(line); //in questo modo leggo tutto il file e memorizzo tutto nella lista

    file.close();    //quindi chiudo il file 

	listLines.pop_front(); //elimino la prima riga

	for (std::string& line : listLines) 
	{
        std::replace(line.begin(), line.end(), ';', ' ');  // Sostituisce i ';' con uno spazio
    }
	std::cout<<"La lista in cui salvo i valori del file Cell0Ds è: "<<endl;
	for (const std::string& val:listLines)
	{
		std::cout<<val<<" "<<endl;
	}

	//adesso incomincio a creare un'istanza da PoligonalMesh, andando quindi a definire i suoi attributi
    mesh.NumCell0Ds = listLines.size();    //il numero di celle è dato dalla dimensione dalla lista

    if (mesh.NumCell0Ds == 0)  //se il numero di nodi è 0, non fare nulla
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);  //con il comando reserve, voglio assicurarmi di avere abbastanza memoria per memorizzare tutti gli ID
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds); //inizializzo la matrice delle coordinate

    for (const string& line : listLines)   //scorciatoia per ciclare sugli elementi della lista
    {
        //ogni volta che leggo un elemento della lista:
		istringstream converter(line);  //converto la stringa

        unsigned int id;  //inizializzo una variabile che mi memorizzerà l'id del nodo corrente
        unsigned int marker; //inizializzo una variabile che mi memorizzerà i marker del nodo corrente  
        Vector2d coord;  //inizializzo un vettore 2d che mi memorizzerà le coordinate del nodo
		
		//quindi assegno i valori letti dalla lista mediante ">>"
        converter >>  id >> marker >> mesh.Cell0DsCoordinates(0, id) >> mesh.Cell0DsCoordinates(1, id);
		//nota che la matrice ha come terza riga tutta 0 in quanto le z sono nulle in nostro esercizio
        mesh.Cell0DsId.push_back(id); //metto nel mio vettore i vari ID (so di avere abbastanza memoria)
		
        // Memorizza i marker nel dizionario
        if(marker != 0) //se il marker è diverso da 0:
        {
            const auto it = mesh.MarkerCell0Ds.find(marker);
            if(it == mesh.MarkerCell0Ds.end())
            {
                mesh.MarkerCell0Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell0Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }
	
    }
	//mi assicuro di aver ottenuto quello che mi aspetto
	std::cout<<"Gli ID dei punti sono: "<<endl;
	for (int valore:mesh.Cell0DsId) //gli Id sono effettivamente quelli che mi aspetto
	{
		std::cout<<valore <<" ";
	}
	std::cout<<" "<<endl;
	//proviamo a stampare i marker
	for (const auto& [marker, ids] : mesh.MarkerCell0Ds) {
    std::cout << marker << ": [";
		for (auto it = ids.begin(); it != ids.end(); ++it) {
			std::cout << *it;
			if (std::next(it) != ids.end())
				std::cout << ", ";
    }
    std::cout << "]\n";
}
	
	return true;
}
// ***************************************************************************

//voglio estrarre le proprietà dal file csv relativo a cell0D, il quale contiene le informazioni sugli spigoli
//ID=identifica di quale segmento sto parlando;
//Marker= é un'etichetta per ogni segmento;
//Origin= ID nodo d'origine
//End= ID nodo finale 

bool ImportCell1Ds(PoligonalMesh& mesh) 
{
    ifstream file("./Cell1Ds.csv");  

    if(file.fail())
        return false;

    list<string> listLines;  //memorizzo il contenuto del file all'interno di una lista, come fatto in precedenza
    string line;
    while (getline(file, line))
        listLines.push_back(line);  //leggo il file riga per riga (fino al carattere \n)

    file.close();  //quindi chiudo il file

    // togliamo la prima riga che è inutile ai nostri scopi
    listLines.pop_front();
	for (std::string& line : listLines) 
	{
        std::replace(line.begin(), line.end(), ';', ' ');  // Sostituisce i ';' con uno spazio
    }
    mesh.NumCell1Ds = listLines.size();  //il numero di segmneti è uguale al numero di elementi della lista

    if (mesh.NumCell1Ds == 0)  //se il numero di celle è pari a 0, non fare nulla 
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);  //mi assicuro di avere sufficiente spazio per memorizzare tutti gli ID dei miei segmemti
											  // ovviamente è chimato il costruttore di default
	mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds); // inizizlizzo la matrice che mi memorizzerà gli estremi

    for (const string& line : listLines)  //sintassi abbreviata per ciclare sugli elementi della lista
    {
        //faccio quindi la stessa cosa fatta per i nodi: aggiorno gli attributi relativi a cell(1)
		istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id >> marker >>  mesh.Cell1DsExtrema(0, id) >>  mesh.Cell1DsExtrema(1, id);
        mesh.Cell1DsId.push_back(id);

        // Memorizza i marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell1Ds.find(marker);
            if(it == mesh.MarkerCell1Ds.end())
            {
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else
            {
                // mesh.MarkerCell1Ds[marker].push_back(id);
                it->second.push_back(id);
            }
        }
    }
	//mi assicuro di aver ottenuto quello che mi aspetto
	std::cout<<"Gli ID dei segmenti sono: "<<endl;
	for (int valore:mesh.Cell0DsId) //gli Id sono effettivamente quelli che mi aspetto
	{
		std::cout<<valore <<" ";
	}
	std::cout<<" "<<endl;

	for (const auto& [marker, ids] : mesh.MarkerCell1Ds) {
    std::cout << marker << ": [";
		for (auto it = ids.begin(); it != ids.end(); ++it) {
			std::cout << *it;
			if (std::next(it) != ids.end())
				std::cout << ", ";
    }
    std::cout << "]\n";
}
    return true;
}
// ***************************************************************************
//voglio estrarre le proprietà dal file csv relativo a cell0D, il quale contiene le informazioni sulle celle
//ID=identifica di quale cella sto parlando;
//Vertices= Lista dei nodi che definiscono gli spigoli della cella
//Edges= Lista dei segmenti che definiscono la cella

bool ImportCell2Ds(PoligonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv"); //apro il file

    if(file.fail())
        return false;   //mi assicuro sia aperto correttamente

    list<string> listLines;   //definisco la lista in cui memorizzo i dati del file
    string line;
    while (getline(file, line))
        listLines.push_back(line);  //leggo il file riga per riga

    file.close();  //quindi chiudo il file

    // rimuovo la prima riga che non mi serve per i nostri scopi
    listLines.pop_front();
	for (std::string& line : listLines) 
	{
        std::replace(line.begin(), line.end(), ';', ' ');  // Sostituisce i ';' con uno spazio
    }
    mesh.NumCell2Ds = listLines.size(); //il numero di celle è pari alla lunghezza della stringa

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }
	
	mesh.Cell2DsId.reserve(mesh.NumCell2Ds);  //mi assicuro di avere abbastanza capacità per memorizzare tutti i dati in ciascun vettore
	mesh.Cell2DsMarker.reserve(mesh.NumCell2Ds);
	mesh.Cell2DsNumVertices.reserve(mesh.NumCell2Ds); 
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsNumEdges.reserve(mesh.NumCell2Ds);
	mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);
	
    for (const string& line : listLines)  //ciclo sugli elementi della stringa
    {
        istringstream converter(line);

        unsigned int id;
        converter >>  id; //assegno alla variabile id il suo valore letto dal file
        mesh.Cell2DsId.push_back(id); //metto il valore letto nel vettore riservato agli ID
		
		unsigned int marker; 
		converter >> marker; //assegno alla variabile marker il suo valore letto dal file
		mesh.Cell2DsMarker.push_back(marker); //metto il valore letto nel vettore riservato ai marker
		
		unsigned int numVertices;  
		converter >> numVertices; //assegno alla variabile numVertices il suo valore letto dal file
		mesh.Cell2DsNumVertices.push_back(numVertices);//metto il valore letto nel vettore riservato ai numeri di vertici
		
		vector<unsigned int> vertices(numVertices);  //inizializzo un vettore contenente numVertices[i] elementi
		for(unsigned int j = 0; j < numVertices; j++)
			converter >> vertices[j];
		mesh.Cell2DsVertices.push_back(vertices); //riempo il vettore con i suoi valori

		unsigned int numEdges;
		converter >> numEdges; //assegno alla variabile numEdges il suo valore letto dal file
		mesh.Cell2DsNumEdges.push_back(numEdges); //metto il valore letto nel vettore riservato al numero di lati
		
		vector<unsigned int> edges(numEdges);
		for(unsigned int j = 0; j < numEdges; j++)
			converter >> edges[j];
		mesh.Cell2DsEdges.push_back(edges);			
		
    }
	//mi assicuro di aver ottenuto quello che mi aspetto
	std::cout<<"Gli ID dei poligoni sono: "<<endl;
	for (int valore:mesh.Cell0DsId) //Gli ID sono effettivamente quelli che mi aspetto
	{
		std::cout<<valore <<" ";
	}
	std::cout<<" "<<endl;
	return true;
}
// ***************************************************************************
}
