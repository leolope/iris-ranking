// reading a text file
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

const int DATA_SIZE = 150;
const int K_SIZE = 50;

typedef struct {
  int id;
  float f1;
  float f2;
  float f3;
  float f4;
  char *classe;
} flower;

typedef struct {
	float valor;
	flower flor;
} posicao;

flower *dataset;

posicao *rankingf1, *rankingf2, *rankingf3, *rankingf4;

int *sparseMatrix;

// MERGE SORT
void merge(posicao *arr, int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    posicao L[n1], R[n2]; 
  
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) 
    { 
        if (L[i].valor <= R[j].valor) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
void mergeSort(posicao *arr, int l, int r) 
{ 
    if (l < r) 
    { 
        int m = l+(r-l)/2; 
  
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
}

void criarDataset()
{
	float convert;
	dataset =(flower*) malloc(DATA_SIZE * sizeof(flower));
	rankingf1 = (posicao*) malloc(DATA_SIZE * sizeof(posicao));
	rankingf2 = (posicao*) malloc(DATA_SIZE * sizeof(posicao));
	rankingf3 = (posicao*) malloc(DATA_SIZE * sizeof(posicao));
	rankingf4 = (posicao*) malloc(DATA_SIZE * sizeof(posicao));
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
			float soma = 0;
			all = line.substr(0);
			// ID
			dataset[pos].id = pos;      
			
			//      Primeira feature
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f1 = convert;
			
			// Ranking feature 1
			rankingf1[pos].valor = convert;
			soma += convert;
						
			//      Segunda feature
			all =  all.substr(search+1);
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f2 = convert;
			
			// Ranking feature 2
			rankingf2[pos].valor = convert;
			soma += convert;
			
			//      Terceira feature
			all =  all.substr(search+1);
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f3 = convert;
			
			// Ranking feature 3
			rankingf3[pos].valor = convert;
			soma += convert;
			
			
			//      Quarta feature
			all =  all.substr(search+1);
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f4 = convert;
			soma += convert;
			
			// Ranking feature 4
			rankingf4[pos].valor = convert;
			
			
			// Classe
			all =  all.substr(search+1);
			dataset[pos].classe = new char[all.length() + 1];
		  	strcpy(dataset[pos].classe, all.c_str());
		  	
			// Link do objeto dataset dentro dos rankings de features
			rankingf1[pos].flor = dataset[pos];	
			rankingf2[pos].flor = dataset[pos];	
			rankingf3[pos].flor = dataset[pos];	
			rankingf4[pos].flor = dataset[pos];
			
			rankingf1[pos].valor = rankingf1[pos].valor / soma;	
			rankingf2[pos].valor = rankingf2[pos].valor / soma;	
			rankingf3[pos].valor = rankingf3[pos].valor / soma;	
			rankingf4[pos].valor = rankingf4[pos].valor / soma;		  	
		  	

			pos++;
		}
		myfile.close();
		
		// Ordenacao dos rankings de features
		mergeSort(rankingf1, 0, DATA_SIZE - 1);
		mergeSort(rankingf2, 0, DATA_SIZE - 1);	  
		mergeSort(rankingf3, 0, DATA_SIZE - 1);	  
		mergeSort(rankingf4, 0, DATA_SIZE - 1);	  
	}
	else 
	{
		cout << "Nao foi possivel abrir o dataset."; 
	}
}

int buscaBinaria(posicao *ranking, int inicio, int fim, float valorProcurado)
{
	if(fim >= inicio)
	{
		int meio = inicio + (fim - inicio) / 2;
		
		if(ranking[meio].valor == valorProcurado)
		{
			return meio;
		}
		else if(ranking[meio].valor > valorProcurado)
		{
			return buscaBinaria(ranking, inicio, meio -1, valorProcurado);
		}
		else
		{
			return buscaBinaria(ranking, meio + 1, fim, valorProcurado);
		}
	}
	
	return -1;
}

void exibirDataset()
{
	printf("DATASET COMPLETO\n\n");
	for(int x = 0; x<DATA_SIZE; x++)
	{
		printf("%f %f %f %f %s\n", dataset[x].f1, dataset[x].f2, dataset[x].f3, dataset[x].f4, dataset[x].classe);
	}
	
	printf("\nRANKING F1\n\n");
	for(int x = 0; x < DATA_SIZE; x++)
	{
		printf("%.2f\n", rankingf1[x].valor);
	}
	
	printf("\nRANKING F2\n\n");
	for(int x = 0; x < DATA_SIZE; x++)
	{
		printf("%.2f\n", rankingf2[x].valor);
	}
	
	printf("\nRANKING F3\n\n");
	for(int x = 0; x < DATA_SIZE; x++)
	{
		printf("%.2f\n", rankingf3[x].valor);
	}
	
	printf("\nRANKING F4\n\n");
	for(int x = 0; x < DATA_SIZE; x++)
	{
		printf("%.2f\n", rankingf4[x].valor);
	}
}

float calcularEuclidiana(flower *a, flower *b)
{
	float diferenca = pow((a->f1 - b->f1),2) + pow((a->f2 - b->f2),2) + pow((a->f3 - b->f3),2) + pow((a->f4 - b->f4),2);
	return sqrt(diferenca);
}

void gerarRankingEuclidiana(posicao *ranking, flower *flor)
{
	for(int x = 0; x < DATA_SIZE; x++)
	{
		ranking[x].valor = calcularEuclidiana(&dataset[x], flor);
		ranking[x].flor = dataset[x];
	}
	
	mergeSort(ranking, 0, DATA_SIZE - 1);
}

void filtrarRanking(posicao *novoRanking, posicao *ranking, int index)
{
	int indexAcima = index - 1;
	int indexAbaixo = index + 1;	

	for(int x = 0; x < K_SIZE; x++)
	{
		// Verifica se o �ndice acima est� antes do in�cio do array ou se o �ndice abaixo est� ap�s o fim do array.		
		if(indexAcima < 0)
		{
			novoRanking[x] = ranking[indexAbaixo];
			indexAbaixo++;
			continue;
		}
		else if(indexAbaixo >= DATA_SIZE)
		{
			novoRanking[x] = ranking[indexAcima];
			indexAcima--;
			continue;
		}
		
		// Calcula a diferen�a entre os 2 �ndices e o valor da flor escolhida, adiciona o menor no ranking.
		float diferencaAcima = ranking[index].valor - ranking[indexAcima].valor;
		float diferencaAbaixo = ranking[indexAbaixo].valor - ranking[index].valor;
		if(diferencaAcima <= diferencaAbaixo)
		{
			novoRanking[x] = ranking[indexAcima];
			indexAcima--;
		}
		else
		{
			novoRanking[x] = ranking[indexAbaixo];
			indexAbaixo++;
		}
	}
}

bool vectorSort (int i,int j)
{
	return (sparseMatrix[i] > sparseMatrix[j]);
}

void kRankingToVector(vector<int> *vector, posicao *ranking, int *sparseMatrix)
{
	for(int x = 0; x < K_SIZE; x++)
	{
		int id = ranking[x].flor.id;
		if(sparseMatrix[id] == NULL)
		{
			vector->push_back(id);
		}
		sparseMatrix[id] += K_SIZE - x;
	}
}

void gerarRankingFeatures(vector<int> *ranking, flower *flor)
{
	posicao *kRankingf1, *kRankingf2, *kRankingf3, *kRankingf4;
	float soma = flor->f1 + flor->f2 + flor->f3 + flor->f4;
	int index = 0;
	index = buscaBinaria(rankingf1, 0, DATA_SIZE, flor->f1 / soma);
	kRankingf1 = (posicao*) malloc(K_SIZE * sizeof(posicao));
	filtrarRanking(kRankingf1, rankingf1, index);	
	
	index = buscaBinaria(rankingf2, 0, DATA_SIZE, flor->f2 / soma);
	kRankingf2 = (posicao*) malloc(K_SIZE * sizeof(posicao));
	filtrarRanking(kRankingf2, rankingf2, index);
	
	index = buscaBinaria(rankingf3, 0, DATA_SIZE, flor->f3 / soma);
	kRankingf3 = (posicao*) malloc(K_SIZE * sizeof(posicao));
	filtrarRanking(kRankingf3, rankingf3, index);
	
	index = buscaBinaria(rankingf4, 0, DATA_SIZE, flor->f4 / soma);
	kRankingf4 = (posicao*) malloc(K_SIZE * sizeof(posicao));
	filtrarRanking(kRankingf4, rankingf4, index);
	
	sparseMatrix = (int*) malloc(DATA_SIZE * sizeof(int));
	for(int x = 0; x < DATA_SIZE; x++)
	{
		sparseMatrix[x] = NULL;
	}
	kRankingToVector(ranking, kRankingf1, sparseMatrix);
	kRankingToVector(ranking, kRankingf2, sparseMatrix);
	kRankingToVector(ranking, kRankingf3, sparseMatrix);
	kRankingToVector(ranking, kRankingf4, sparseMatrix);
	
	sort(ranking->begin(), ranking->end(), vectorSort);	
}

float calcularPorcentagemAcerto(posicao *ranking, flower *flor)
{
	int acertos = 0;
	for(int x = 0; x < K_SIZE; x++)
	{
		if(strcmp(ranking[x].flor.classe, flor->classe) == 0 )
		{
			acertos++;
		}
	}
	
	return acertos * 100 / K_SIZE;
}

float calcularPorcentagemAcertoFeatures(vector<int> *ranking, flower *flor)
{
	int acertos = 0;
	for(int x = 0; x < K_SIZE; x++)
	{
		if(strcmp(dataset[ranking->at(x)].classe, flor->classe) == 0 )
		{
			acertos++;
		}
	}
	
	return acertos * 100 / K_SIZE;
}

int main () { 
	criarDataset();

	// Escolha de uma flor gen�rica
	float totalEuclidiana = 0;
	float totalFeatures = 0;
	for(int x = 0; x < DATA_SIZE; x++)
	{
		int idEscolhido = x;
		printf("Flor escolhida: %d - Classe: %s - F1: %.2f - F2: %.2f - F3: %.2f - F4: %.2f\n\n", dataset[idEscolhido].id, dataset[idEscolhido].classe, dataset[idEscolhido].f1, dataset[idEscolhido].f2, dataset[idEscolhido].f3, dataset[idEscolhido].f4);
		
		
		// Ranking Euclidiana	
		posicao *euclidiana;
		euclidiana = (posicao*) malloc(DATA_SIZE * sizeof(posicao));
		gerarRankingEuclidiana(euclidiana, &dataset[idEscolhido]);
		//	
		// C�lculo de porcentagem de acertos da euclidiana
		float porcentagem = calcularPorcentagemAcerto(euclidiana, &dataset[idEscolhido]);
		totalEuclidiana += porcentagem;
		printf("Percentagem do ranking euclidiana: %.2f%%\n", porcentagem);	
		
		// Montagem do ranking beseado em features
		vector<int>	features;
		gerarRankingFeatures(&features, &dataset[idEscolhido]);
		// C�lculo da porcentagem de acertos do ranking de features
		float porcentagemFeatures = calcularPorcentagemAcertoFeatures(&features, &dataset[idEscolhido]);
		totalFeatures += porcentagemFeatures;
		printf("Percentagem do ranking features: %.2f%%\n", porcentagemFeatures);	
	}
	
	printf("\n\n");
	
	totalEuclidiana = totalEuclidiana / DATA_SIZE;
	totalFeatures = totalFeatures / DATA_SIZE;
	
	printf("Media de acertos no ranking euclidiana: %.2f%%\n", totalEuclidiana);
	printf("Media de acertos no ranking de features: %.2f%%", totalFeatures);
}
