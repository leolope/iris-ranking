// reading a text file
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <math.h>

using namespace std;

const int DATA_SIZE = 150;
const int K_SIZE = 20;

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

posicao *rankingMedias;


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
	rankingMedias = (posicao*) malloc(DATA_SIZE * sizeof(posicao));
	string line, part, all;
	size_t search;
	char * pch;
	string delimiter = ",";
	ifstream myfile ("iris.data");
	float soma = 0;
	if (myfile.is_open())
	{
		int pos = 0; 	
		while ( getline (myfile,line) )
		{
			soma = 0;
			all = line.substr(0);
			// ID
			dataset[pos].id = pos;      
			
			//      Primeira feature
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f1 = convert;
			
			// Soma para totalizar média
			soma += convert * 1;
						
			//      Segunda feature
			all =  all.substr(search+1);
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f2 = convert;
			
			// Soma para totalizar média
			soma += convert * 2;
			
			//      Terceira feature
			all =  all.substr(search+1);
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f3 = convert;
			
			// Soma para totalizar média
			soma += convert * 3;
			
			
			//      Quarta feature
			all =  all.substr(search+1);
			search = all.find(delimiter);
			convert = strtof(all.c_str(),0);
			dataset[pos].f4 = convert;
			
			// Soma para totalizar média
			soma += convert * 4;
			
			
			// Classe
			all =  all.substr(search+1);
			dataset[pos].classe = new char[all.length() + 1];
		  	strcpy(dataset[pos].classe, all.c_str());
		  	
			// Preenchimento do ranking de médias
			rankingMedias[pos].valor = soma / 10;
			rankingMedias[pos].flor = dataset[pos];	  	

			pos++;
		}
		myfile.close();
		
		// Ordenacao dos rankings de features
		mergeSort(rankingMedias, 0, DATA_SIZE - 1);
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
	
	printf("\nRANKING MEDIAS\n\n");
	for(int x = 0; x < DATA_SIZE; x++)
	{
		printf("%.2f\n", rankingMedias[x].valor);
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
		// Verifica se o índice acima está antes do início do array ou se o índice abaixo está após o fim do array.		
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
		
		// Calcula a diferença entre os 2 índices e o valor da flor escolhida, adiciona o menor no ranking.
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

void gerarRankingMedias(posicao *ranking, flower *flor)
{
	float mediaFlor = (flor->f1 * 1 + flor->f2 * 2 + flor->f3 * 3 + flor->f4 * 4) / 10;
	int index = 0;
	index = buscaBinaria(rankingMedias, 0, DATA_SIZE, mediaFlor);
	filtrarRanking(ranking, rankingMedias, index);
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

int main () { 
	criarDataset();
	
	for(int x = 0; x < DATA_SIZE; x++)
	{
		printf("index: %d -- valor: %.2f\n", x, rankingMedias[x].valor);
	}
	
	printf("\n\n");

	float totalEuclidiana = 0;
	float totalMedias = 0;
	for(int x = 0; x < DATA_SIZE; x++)
	{
		int idEscolhido = x;
		printf("Flor escolhida: %d - Classe: %s - F1: %.2f - F2: %.2f - F3: %.2f - F4: %.2f\n\n", dataset[idEscolhido].id, dataset[idEscolhido].classe, dataset[idEscolhido].f1, dataset[idEscolhido].f2, dataset[idEscolhido].f3, dataset[idEscolhido].f4);
		
		// Ranking Euclidiana	
		posicao *euclidiana;
		euclidiana = (posicao*) malloc(DATA_SIZE * sizeof(posicao));
		gerarRankingEuclidiana(euclidiana, &dataset[idEscolhido]);
		//	
		// Cálculo de porcentagem de acertos da euclidiana
		float porcentagem = calcularPorcentagemAcerto(euclidiana, &dataset[idEscolhido]);
		totalEuclidiana += porcentagem;
		printf("Percentagem do ranking euclidiana: %.2f%%\n", porcentagem);	
		
		// Montagem do ranking beseado em features
		posicao *medias = (posicao*) malloc(K_SIZE * sizeof(posicao));
		gerarRankingMedias(medias, &dataset[idEscolhido]);
		float porcentagemMedia = calcularPorcentagemAcerto(medias, &dataset[idEscolhido]);
		totalMedias += porcentagemMedia;
		printf("Porcentagem do ranking de medias: %.2f%%\n", porcentagemMedia);
	}
	
	printf("\n\n");
	
	totalEuclidiana = totalEuclidiana / DATA_SIZE;
	totalMedias = totalMedias / DATA_SIZE;
	
	printf("Media de acertos no ranking euclidiana: %.2f%%\n", totalEuclidiana);
	printf("Media de acertos no ranking de medias: %.2f%%", totalMedias);
}
