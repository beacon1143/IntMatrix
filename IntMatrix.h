#include<stdio.h>

typedef struct {
  int** elements;
  unsigned int nRows;
  unsigned int nColumns;
} IntMatrix;

IntMatrix MakeMatrix(const char* filename);
void DeleteMatrix(IntMatrix* pMatr);
int PrintMatrixToFile(const IntMatrix* const pMatr, const char* filename);

int DeleteColumn(IntMatrix* const pMatr, const unsigned int iColumn);
int DeleteRow(IntMatrix* const pMatr, const unsigned int iRow);

int ComputeSumOfAllElements(const IntMatrix* const pMatr);
int FindSuitableElement(const IntMatrix* const pMatr, int* const rowNo, int* const columnNo);