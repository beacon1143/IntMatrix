#include<stdlib.h>

#include "IntMatrix.h"

IntMatrix MakeMatrix(const char* filename) {
  IntMatrix retMatr;
  FILE* inputFile;
  unsigned int i, j, k;

  inputFile = fopen(filename, "r");
  if (!inputFile) {
    printf("Error: in MakeMatrix(): Cannot open file %s!\n", filename);
    exit(1);
  }
  
  if (fscanf(inputFile, "%u", &retMatr.nRows) != 1) {
    printf("Error: in MakeMatrix(): Cannot read number of rows from file %s!\n", filename);
    fclose(inputFile);
    exit(2);
  }

  if (fscanf(inputFile, "%u", &retMatr.nColumns) != 1) {
    printf("Error: in MakeMatrix(): Cannot read number of columns from file %s!\n", filename);
    fclose(inputFile);
    exit(2);
  }

  retMatr.elements = (int**)malloc(retMatr.nRows * sizeof(int*));
  if (!retMatr.elements) {
    printf("Error: in MakeMatrix(): Cannot allocate memory!\n");
    fclose(inputFile);
    exit(3);
  }

  for (i = 0; i < retMatr.nRows; i++) {
    retMatr.elements[i] = (int*)malloc(retMatr.nColumns * sizeof(int));
    if (!retMatr.elements[i]) {
      printf("Error: in MakeMatrix(): Cannot allocate memory!\n");
      for (j = 0; j < i; j++) {
        free(retMatr.elements[j]);
      }
      free(retMatr.elements);
      fclose(inputFile);
      exit(3);
    }
  }

  for (i = 0; i < retMatr.nRows; i++) {
    for (j = 0; j < retMatr.nColumns; j++) {
      if (fscanf(inputFile, "%d", &retMatr.elements[i][j]) != 1) {
        printf("Error: in MakeMatrix(): Wrong format of input file %s!\n", filename);
        for (k = 0; k < retMatr.nRows; k++) {
          free(retMatr.elements[k]);
	}
        free(retMatr.elements);
        fclose(inputFile);
        exit(2);
      }
    }
  }

  if (!feof(inputFile)) {
    printf("Warning: in MakeMatrix(): Input file %s contains extra data!\n", filename);
  }

  fclose(inputFile);

  printf("IntMatrix created succesfully\n");    // debugging info
  return retMatr;
}

void DeleteMatrix(IntMatrix* const pMatr) {
  unsigned int i;

  if (!pMatr) {
    printf("Error: in DeleteMatrix(): Matrix doesn't exist!\n");
    return;
  }

  for (i = 0; i < pMatr->nRows; i++) {
    free(pMatr->elements[i]);
  }
  free(pMatr->elements);
  pMatr->elements = NULL;

  printf("IntMatrix deleted successfully!\n");    // debugging info
}

int PrintMatrixToFile(const IntMatrix* const pMatr, const char* filename) {
  FILE* outFile;
  unsigned int i, j;

  if (!pMatr) {
    printf("Error: in PrintMatrixToFile(): Matrix doesn't exist!\n");
    return 1;
  }

  outFile = fopen(filename, "w");
  if (!outFile) {
    printf("Error: in PrintMatrixToFile(): Cannot open file %s!\n", filename);
    return 2;
  }

  for (i = 0; i < pMatr->nRows; i++) {
    for (j = 0; j < pMatr->nColumns; j++) {
      fprintf(outFile, "%d ", pMatr->elements[i][j]);
    }
    fprintf(outFile, "\n");
  }

  fclose(outFile);

  printf("IntMatrix printed successfully\n");    // debugging info

  return 0;
}

int DeleteColumn(IntMatrix* const pMatr, const unsigned int iColumn) {
  unsigned int i, j;
  int* tempColumn;

  if (!pMatr) {
    printf("Error: in DeleteColumn(): Matrix doesn't exist!\n");
    return 1;
  }
  if (iColumn >= pMatr->nColumns) {
    printf("Error: in DeleteColumn(): No such column!\n");
    return 2;
  }

  for (i = 0; i < pMatr->nRows; i++) {
    for (j = iColumn; j < pMatr->nColumns; j++) {
      pMatr->elements[i][j] = pMatr->elements[i][j + 1];
    }
  }

  for (i = 0; i < pMatr->nRows; i++) {
    tempColumn = (int*)realloc(pMatr->elements[i], (pMatr->nColumns - 1)*sizeof(int));
    if (!tempColumn) {
      printf("Error: in DeleteColumn(): cannot reallocate memory!\n");
      DeleteMatrix(pMatr);
      exit(3);
    }
    pMatr->elements[i] = tempColumn;
  }
  pMatr->nColumns--;

  printf("Column No%u deleted successfully\n", iColumn);    // debugging info
  return 0;
}

int DeleteRow(IntMatrix* const pMatr, const unsigned int iRow) {
  unsigned int i;
  int** tempRow;

  if (!pMatr) {
    printf("Error: in DeleteRow(): Matrix doesn't exist!\n");
    return 1;
  }
  if (iRow >= pMatr->nRows) {
    printf("Error: in DeleteRow(): No such row!\n");
    return 2;
  }

  free(pMatr->elements[iRow]);

  for (i = iRow; i < pMatr->nRows; i++) {
    pMatr->elements[i] = pMatr->elements[i+1];
  }

  tempRow = (int**)realloc(pMatr->elements, (pMatr->nRows - 1) * sizeof(int*));
  if (!tempRow) {
    printf("Error: in DeleteRow(): cannot reallocate memory!\n");
    DeleteMatrix(pMatr);
    exit(3);
  }
  pMatr->elements = tempRow;
  pMatr->nRows--;

  printf("Row No%u deleted successfully\n", iRow);    // debugging info
  return 0;
}

int ComputeSumOfAllElements(const IntMatrix* const pMatr) {
  int sum;
  unsigned int i, j;

  if (!pMatr) {
    printf("Error: in ComputeSumOfAllElements(): Matrix doesn't exist!\n");
    exit(1);
  }

  sum = 0;
  for (i = 0; i < pMatr->nRows; i++) {
    for (j = 0; j < pMatr->nColumns; j++) {
      sum += pMatr->elements[i][j];
    }
  }

  printf("Sum of all matrix elements computed successfully\n");    // debugging info
  return sum;
}

int FindSuitableElement(const IntMatrix* const pMatr, int* const rowNo, int* const columnNo) {
  unsigned int i, j;
  int sum, nElements, mean;

  sum = ComputeSumOfAllElements(pMatr);
  nElements = pMatr->nRows * pMatr->nColumns;

  if (sum % nElements != 0) {
    printf("Suitable element not found!\n");
    return 1;
  }

  mean = sum / nElements;

  for (i = 0; i < pMatr->nRows; i++) {
    for (j = 0; j < pMatr->nColumns; j++) {
      if (pMatr->elements[i][j] == mean) {
        *rowNo = i;
        *columnNo = j;
        printf("Suitable element found in row %u, column %u\n", i, j);    // debugging info
        return 0;
      }
    }
  }

  printf("Suitable element not found!\n");
  return 2;
}

