#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *lerArquivo(char *caminho) {
  FILE *arquivo = fopen(caminho, "r");
  if (!arquivo) {
    perror("Erro ao abrir o arquivo de entrada");
    exit(EXIT_FAILURE);
  }

  fseek(arquivo, 0, SEEK_END);
  long fileSize = ftell(arquivo);
  rewind(arquivo);

  char *buffer = (char *)malloc((fileSize + 1) * sizeof(char));
  fread(buffer, 1, fileSize, arquivo);
  buffer[fileSize] = '\0';

  fclose(arquivo);
  return buffer;
}

void escreverArquivo(char *caminho, char *data) {
  FILE *arquivo = fopen(caminho, "w");
  if (!arquivo) {
    perror("Erro ao abrir o arquivo de saída");
    exit(EXIT_FAILURE);
  }

  fprintf(arquivo, "%s", data);
  fclose(arquivo);
  printf("Arquivo de saída atualizado com sucesso!\n");
}

void trocar(char *array[], int i, int j) {
  char *temp = array[i];
  array[i] = array[j];
  array[j] = temp;
}

int particionar(char *array[], int inicio, int fim, int (*comparar)(char *, char *)) {
  char *pivo = array[fim];
  int i = inicio - 1;

  for (int j = inicio; j < fim; j++) {
    if (comparar(array[j], pivo) <= 0) {
      i++;
      trocar(array, i, j);
    }
  }

  trocar(array, i + 1, fim);
  return i + 1;
}

void quickSort(char *array[], int inicio, int fim, int (*comparar)(char *, char *)) {
  if (inicio < fim) {
    int posicaoPivo = particionar(array, inicio, fim, comparar);
    quickSort(array, inicio, posicaoPivo - 1, comparar);
    quickSort(array, posicaoPivo + 1, fim, comparar);
  }
}

double calcularDistancia(char *ponto) {
  int x, y;
  sscanf(ponto, "(%d,%d)", &x, &y);
  return sqrt(x * x + y * y);
}

int compararDistancia(char *a, char *b) {
  double distanciaA = calcularDistancia(a);
  double distanciaB = calcularDistancia(b);
  return (distanciaA > distanciaB) - (distanciaA < distanciaB);
}

int compararString(char *a, char *b) { 
    return strcmp(a, b); 
}

int compararNumero(char *a, char *b) {
  double numA = atof(a);
  double numB = atof(b);
  return (numA > numB) - (numA < numB);
}

int verificarInteiro(char *str) {
  char *endptr;
  double val = strtod(str, &endptr);
  return *endptr == '\0' && val == (int)val;
}

int verificarCoordenada(char *str) {
  return str[0] == '(' && str[strlen(str) - 1] == ')';
}


int main() {
  char *inputCaminho = "./L0Q2.in";
  char *outputCaminho = "./L0Q2.out";

  char *conteudo = lerArquivo(inputCaminho);

  char *linhas[1000];
  int linhasCount = 0;

  char *linha = strtok(conteudo, "\r\n");
  while (linha) {
    linhas[linhasCount++] = strdup(linha);
    linha = strtok(NULL, "\r\n");
  }

  char output[10000] = "";

  for (int i = 0; i < linhasCount; i++) {
    char *elements[100];
    int elementCount = 0;

    char *token = strtok(linhas[i], " ");
    while (token) {
      elements[elementCount++] = strdup(token);
      token = strtok(NULL, " ");
    }

    char *strings[100], *inteiros[100], *decimais[100], *coordenadas[100];
    int stringCount = 0, intCount = 0, decCount = 0, cordCount = 0;

    for (int j = 0; j < elementCount; j++) {
      if (verificarCoordenada(elements[j])) {
        coordenadas[cordCount++] = elements[j];
      } else if (verificarInteiro(elements[j])) {
        inteiros[intCount++] = elements[j];
      } else if (isdigit(elements[j][0]) || elements[j][0] == '-') {
        decimais[decCount++] = elements[j];
      } else {
        strings[stringCount++] = elements[j];
      }
    }

    quickSort(strings, 0, stringCount - 1, compararString);
    quickSort(coordenadas, 0, cordCount - 1, compararDistancia);
    quickSort(decimais, 0, decCount - 1, compararNumero);
    quickSort(inteiros, 0, intCount - 1, compararNumero);

    char linhaOutput[1024] = "";
    strcat(linhaOutput, "str:");
    for (int j = 0; j < stringCount; j++) {
      strcat(linhaOutput, strings[j]);
      strcat(linhaOutput, " ");
    }
    if (stringCount == 0)
      strcat(linhaOutput, " ");

    strcat(linhaOutput, "int:");
    for (int j = 0; j < intCount; j++) {
      strcat(linhaOutput, inteiros[j]);
      strcat(linhaOutput, " ");
    }
    if (intCount == 0)
      strcat(linhaOutput, " ");

    strcat(linhaOutput, "float:");
    for (int j = 0; j < decCount; j++) {
      strcat(linhaOutput, decimais[j]);
      strcat(linhaOutput, " ");
    }
    if (decCount == 0)
      strcat(linhaOutput, " ");

    strcat(linhaOutput, "p:");
    for (int j = 0; j < cordCount; j++) {
      strcat(linhaOutput, coordenadas[j]);
      if (j + 1 < cordCount)
        strcat(linhaOutput, " ");
    }

    strcat(output, linhaOutput);
    strcat(output, "\n");
  }

  escreverArquivo(outputCaminho, output);

  free(conteudo);
  for (int i = 0; i < linhasCount; i++) {
    free(linhas[i]);
  }

  return 0;
}