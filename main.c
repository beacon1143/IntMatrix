#include "IntMatrix.h"

int main(void)
{
  IntMatrix matrix;
  int columnNo, rowNo;
  int isElementFound, isColumnDeleted, isRowDeleted, isMatrixPrinted;

  matrix = MakeMatrix("data.txt");
  
  isElementFound = FindSuitableElement(&matrix, &rowNo, &columnNo);

  if (isElementFound == 0) {
    isColumnDeleted = DeleteColumn(&matrix, columnNo);
    if (isColumnDeleted != 0) {
      DeleteMatrix(&matrix);
      return 2;
    }
    isRowDeleted = DeleteRow(&matrix, rowNo);
    if (isRowDeleted != 0) {
      DeleteMatrix(&matrix);
      return 1;
    }
  }

  isMatrixPrinted = PrintMatrixToFile(&matrix, "results.txt");
  if (isMatrixPrinted != 0) {
    DeleteMatrix(&matrix);
    return 1;
  }
  DeleteMatrix(&matrix);
  return 0;
}
