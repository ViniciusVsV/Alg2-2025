#ifndef Geradoras
#define Geradoras

void imprimeVetor(int *vet, int tam);

int *geraVetor(int tam, int porcentagemOrdenada);
//Para gerar um vetor ALEATÓRIO: porcentagemOrdenada = 0
//Para gerar um vetor ORDENADO: porcentagemOrdenada = 100
//Para gerar um vetor PARCIALMENTE ORDENADO: 0 < porcentagemOrdenada < 100
//Para gerar um vetor DECRESCENTE: porcentagemOrdenada = -1


#endif