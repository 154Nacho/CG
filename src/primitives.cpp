// Aqui � apra fazer os pontos das cenas
#include "ponto.h"
#include <math.h>
#include <vector>

using namespace std;


// Cria��o de um plano na superf�cie do plano XZ.
vector<Ponto> plano(float size) {
	
	//vector<Ponto> � o mesmo que ter um array dinamico de Ponto
	vector<Ponto> pontos;

	float distancia = size / 2;

	pontos.push_back(Ponto(distancia, 0, distancia));
	pontos.push_back(Ponto(distancia, 0, -distancia));
	pontos.push_back(Ponto(-distancia, 0, -distancia));

	pontos.push_back(Ponto(-distancia, 0, -distancia));
	pontos.push_back(Ponto(-distancia, 0, distancia));
	pontos.push_back(Ponto(distancia, 0, distancia));

	return pontos;

}


vector<Ponto> criaCubo(float iniX, float iniY, float iniZ, float dimX, float dimY, float dimZ, int divX, int divY, int divZ) {
	
	vector<Ponto> pontos;

	//CONTRUINDO POR FACES TEMOS:


	//Baixo
	pontos.push_back(Ponto(-iniX + divX * dimX, -iniY, -iniZ + (divZ + 1) * dimZ));
	pontos.push_back(Ponto(-iniX + divX * dimX, -iniY, -iniZ + divZ * dimZ));
	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, -iniY, -iniZ + divZ * dimZ));

	pontos.push_back(Ponto(-iniX + divX * dimX, -iniY, -iniZ + (divZ + 1) * dimZ));
	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, -iniY, -iniZ + divZ * dimZ));
	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, -iniY, -iniZ + (divZ + 1) * dimZ));

	//Cima
	pontos.push_back(Ponto(-iniX + divX * dimX, iniY, -iniZ + (divZ + 1) * dimZ));
	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, iniY, -iniZ + divZ * dimZ));
	pontos.push_back(Ponto(-iniX + divX * dimX, iniY, -iniZ + divZ * dimZ));

	pontos.push_back(Ponto(-iniX + divX * dimX, iniY, iniZ + (divZ + 1) * dimZ));
	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, iniY, -iniZ + (divZ + 1) * dimZ));
	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, iniY, -iniZ + divZ * dimZ));

	
	//Frente
	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, iniY - dimY * dimY, iniZ));
	pontos.push_back(Ponto(-iniX + divX * dimX, iniY - dimY * dimY, iniZ));
	pontos.push_back(Ponto(-iniX + divX * dimX, iniY - (dimY + 1) * dimY, iniZ));

	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, iniY - (dimY + 1) * dimY, iniZ));
	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, iniY - dimY * dimY, iniZ));
	pontos.push_back(Ponto(-iniX + divX * dimX, iniY - (dimY + 1) * dimY, iniZ));

	//Tr�s
	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, iniY - dimY * dimY, -iniZ));
	pontos.push_back(Ponto(-iniX + divX * dimX, iniY - (dimY + 1) * dimY, -iniZ));
	pontos.push_back(Ponto(-iniX + divX * dimX, iniY - dimY * dimY, -iniZ));

	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, iniY - (dimY + 1) * dimY, -iniZ));
	pontos.push_back(Ponto(-iniX + divX * dimX, iniY - (dimY + 1) * dimY, -iniZ));
	pontos.push_back(Ponto(-iniX + (divX + 1) * dimX, iniY - dimY * dimY, -iniZ));

	
	//Direita
	pontos.push_back(Ponto(iniX, iniY - (divY + 1) * dimY, iniZ - divZ * dimZ));
	pontos.push_back(Ponto(iniX, iniY - (divY + 1) * dimY, iniZ - (divZ + 1) * dimZ));
	pontos.push_back(Ponto(iniX, iniY - divY * dimY, iniZ - (divZ + 1) * dimZ));

	pontos.push_back(Ponto(iniX, iniY - (divY + 1) * dimY, iniZ - divZ * dimZ));
	pontos.push_back(Ponto(iniX, iniY - divY * dimY, iniZ - (divZ + 1) * dimZ));
	pontos.push_back(Ponto(iniX, iniY - divY * dimY, iniZ - divZ * dimZ));

	//Esquerda
	pontos.push_back(Ponto(-iniX, iniY - (divY + 1) * dimY, iniZ - divZ * dimZ));
	pontos.push_back(Ponto(-iniX, iniY - (divY + 1) * dimY, iniZ - (divZ + 1) * dimZ));
	pontos.push_back(Ponto(-iniX, iniY - divY * dimY, iniZ - (divZ + 1) * dimZ));

	pontos.push_back(Ponto(-iniX, iniY - (divY + 1) * dimY, iniZ - divZ * dimZ));
	pontos.push_back(Ponto(-iniX, iniY - divY * dimY, iniZ - (divZ + 1) * dimZ));
	pontos.push_back(Ponto(-iniX, iniY - divY * dimY, iniZ - divZ * dimZ));


	return pontos;
}

// Cria��o de uma box com as dimens�es dadas e com um numero de Divis�es em iguais sec��es da caixa
vector<Ponto> box(float dimX, float dimY, float dimZ, int nrDivisoes) {
	
	vector<Ponto> pontos;

	float inicialX = dimX / 2;
	float inicialY = dimY / 2;
	float inicialZ = dimZ / 2;
	
	float distanicaX = dimX / nrDivisoes;
	float distanicaY = dimY / nrDivisoes;
	float distanicaZ = dimZ / nrDivisoes;

	// Se tivermos 2 divis�es, pasamos a ter 8 cubos. 
	// Temos de fazer ent�o 3 for (um para a divis�o em cada eixo)
	for (int divX = 0; divX < nrDivisoes; divX++) {
		for (int divY = 0; divY < nrDivisoes; divY++) {
			for (int divZ = 0; divZ < nrDivisoes; divZ++) {

				//Cria um vector de Pontos para todos os cubos que v�o fazer parte da Box
				vector<Ponto> novosPontos = criaCubo(inicialX, inicialY, inicialZ, distanicaX, distanicaY, distanicaZ, divX, divY, divZ);

				//Junta os pontos de um cubinho aos pontos que j� tinha de cubos anteriores que formavam a caixa
				pontos.insert(pontos.end(), novosPontos.begin(), novosPontos.end());

			}
		}
	}

	return pontos;
}