// reading a text file
#include <stdio.h>
#include <stdlib.h>>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

const int DATA_SIZE = 150;

typedef struct {
  int id;
  float f1;
  float f2;
  float f3;
  float f4;
  char *classe;
} flower;

flower *dataset;

void criarDataset()
{
	float convert;
	dataset =(flower*) malloc(DATA_SIZE * sizeof(flower));
	string line, part, all;
	size_t search;
	char * pch;
	string delimiter = ",";
	ifstream myfile ("iris.data");
	if (myfile.is_open())
	{
		int pos = 0; 	
		while ( getline (myfile,line) )
		{
			all = line.substr(0);
			// ID
			dataset[pos].id = pos;      
			//      Primeira feature
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f1 = convert;
			//      Segunda feature
			all =  all.substr(search+1);
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f2 = convert;
			//      Terceira feature
			all =  all.substr(search+1);
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f3 = convert;
			//      Quarta feature
			all =  all.substr(search+1);
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f4 = convert;
			// Classe
			all =  all.substr(search+1);
			dataset[pos].classe = new char[all.length() + 1];
		  	strcpy(dataset[pos].classe, all.c_str());
			pos++;
		}
		myfile.close();
	}
	else 
	{
		cout << "Nao foi possivel abrir o dataset."; 
	}
}

void exibirDataset()
{
	for(int x = 0; x<150; x++)
	{
		printf("%f %f %f %f %s\n", dataset[x].f1, dataset[x].f2, dataset[x].f3, dataset[x].f4, dataset[x].classe);
	}
}

int main () { 
	criarDataset();
	exibirDataset();
  	return 0;
}
