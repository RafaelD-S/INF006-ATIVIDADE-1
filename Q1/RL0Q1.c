#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 1024

float calculateDistance(float x1, float y1, float x2, float y2) {
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void swap(float arr[][3], int i, int j) {
  float temp[3];
  memcpy(temp, arr[i], sizeof(temp));
  memcpy(arr[i], arr[j], sizeof(temp));
  memcpy(arr[j], temp, sizeof(temp));
}

int partition(float arr[][3], int low, int high) {
  float pivot = arr[high][2];
  int i = low - 1;

  for (int j = low; j < high; j++) {
    if (arr[j][2] < pivot) {
      i++;
      swap(arr, i, j);
    }
  }

  swap(arr, i + 1, high);
  return i + 1;
}

void quickSort(float arr[][3], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

char *readFile(const char *filePath) {
  FILE *file = fopen(filePath, "r");
  if (!file) {
    perror("Erro ao abrir o arquivo de entrada");
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  long int fileSize = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc((fileSize + 1) * sizeof(char));
  fread(buffer, 1, fileSize, file);
  buffer[fileSize] = '\0';

  fclose(file);
  return buffer;
}

void writeFile(const char *filePath, const char *data) {
  FILE *file = fopen(filePath, "w");
  if (!file) {
    perror("Erro ao abrir o arquivo de saída");
    exit(EXIT_FAILURE);
  }

  fprintf(file, "%s", data);
  fclose(file);
  printf("Arquivo de saída atualizado com sucesso!\n");
}

int main() {
  const char *inputFilePath = "./L0Q1.in";
  const char *outputFilePath = "./L0Q1.out";

  char *fileContent = readFile(inputFilePath);

  char *lines[MAX_LINES];
  int lineCount = 0;

  char *token = strtok(fileContent, "\r\n");
  while (token) {
    lines[lineCount++] = strdup(token);
    token = strtok(NULL, "\r\n");
  }

  char output[MAX_LINES * MAX_LINE_LENGTH] = "";

  for (int i = 0; i < lineCount; i++) {
    char *line = lines[i];

    if (!strstr(line, "points")) {
      strcat(output, "Linha de entrada inválida!\n");
      continue;
    }

    float cords[MAX_LINES][3];
    int cordCount = 0;

    char *start = strchr(line, '(');
    while (start) {
      float x, y;
      sscanf(start, "(%lf,%lf)", &x, &y);
      cords[cordCount][0] = x;
      cords[cordCount][1] = y;
      cords[cordCount][2] = sqrt(x * x + y * y);
      cordCount++;
      start = strchr(start + 1, '(');
    }

    float distance = 0.0;
    for (int j = 0; j < cordCount - 1; j++) {
      distance += calculateDistance(cords[j][0], cords[j][1], cords[j + 1][0],
                                    cords[j + 1][1]);
    }

    float shortcut =
        calculateDistance(cords[0][0], cords[0][1], cords[cordCount - 1][0],
                          cords[cordCount - 1][1]);

    quickSort(cords, 0, cordCount - 1);

    char lineOutput[1024] = "points ";
    for (int j = 0; j < cordCount; j++) {
      char point[50];
      snprintf(point, sizeof(point), "(%.0f,%.0f) ", cords[j][0], cords[j][1]);
      strcat(lineOutput, point);
    }

    char result[100];
    snprintf(result, sizeof(result), "distance %.2f shortcut %.2f\n", distance,
             shortcut);
    strcat(lineOutput, result);

    strcat(output, lineOutput);
  }

  writeFile(outputFilePath, output);

  free(fileContent);
  for (int i = 0; i < lineCount; i++) {
    free(lines[i]);
  }

  return 0;
}
