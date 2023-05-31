/*
 * COP 3515 – Spring Semester 2023
 *
 * Homework #2: Spotting The Hacker
 *
 * (Jonathan M Lucius)
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countBits(int n);
void errorCheck(int data);
int *binaryConv(int num);
void printBinary(int *bit);
int oneCompMath(int num1, int num2);

void ParityCheck();
void Checksum();
void Parity2D();

int main(void) {

  ParityCheck(); // Clear
  Checksum();    // Clear
  Parity2D();    // Clear

  // Testing Zone

  // int num1 = 157;
  // int num2 = 138;

  // int test = oneCompMath(num1, num2);
  // printf("\nAdd %3d ", num1);
  // printBinary(binaryConv(num1));
  // printf("Add %3d ", num2);
  // printBinary(binaryConv(num2));
  // printf("-------------------------\n");
  // printf("Sum %3d ",test);
  // printBinary(binaryConv(test));

  return 0;
}

int countBits(int n) {
  int count = 0;
  while (n) {
    count += n & 1;
    n >>= 1;
  }
  return count;
}

void errorCheck(int data) {
  if (countBits(data) % 2 == 0) {
    printf("Successful Transmission - ");
  } else {
    printf("Error - Failed Transmission - ");
  }
}

int *binaryConv(int num) {
  int *bin = (int *)calloc(8, sizeof(int));
  for (int i = 7; i >= 0; i--) {
    if (num & 1) {
      bin[i] += 1;
    }
    num >>= 1;
  }
  return bin;
}

int binaryInvert(int num) {
  int *buffer = binaryConv(num);
  int result = 0;

  for (int i = 0; i < 8; i++) {
    *buffer = !(*buffer);
    buffer++;
  }
  buffer--;

  for (int i = 0; i < 8; i++)
    result += (*buffer--) * (int)pow(2, i);
  return result;
}

void printBinary(int *bit) {
  printf("Binary = ");
  for (int i = 0; i < 8; i++) {
    printf("%d", *bit++);
  }
  printf("\n");
}

int oneCompMath(int num1, int num2) {
  bool looped = false;
  int carry = 0;
  int sum = 0;
  int *bit1 = binaryConv(num1) + 7;
  int *bit2 = binaryConv(num2) + 7;
  int resultBin[8];
  int buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int resultNum = 0;

  for (int i = 0; i < 8; i++) {
    sum = *bit1-- + *bit2-- + carry;

    switch (sum) {
    case 1:
      resultBin[i] = 1;
      carry = 0;
      break;
    case 2:
      carry = 1;
      resultBin[i] = 0;
      break;
    case 3:
      carry = 1;
      resultBin[i] = 1;
      break;
    default:
      resultBin[i] = 0;
    }

    if (!carry && looped)
      break;

    if (i == 7 && carry) {
      bit1 = &resultBin[7];
      bit2 = &buffer[7];
      i = -1;
      looped = true;
    }
  }

  for (int i = 0; i < 8; i++)
    resultNum += resultBin[i] * (int)pow(2.0, i);

  return resultNum;
}

void ParityCheck() {

  printf("~~Parity Check Processing~~\n");
  printf("----------------------------\n");

  FILE *parity = fopen("White House Transmission Data - Parity.txt", "r");
  if (parity == NULL)
    printf("Error opening File, try again...");

  int parityArray[5][9];
  int oldParityArray[5][9];
  int n;

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 9; j++) {
      fscanf(parity, " %d", &n);
      parityArray[i][j] = n;
      oldParityArray[i][j] = n;
    }
  }

  int *parityBit;
  int *numConvert;
  for (int i = 0; i < 5; i++) {

    parityBit = binaryConv(parityArray[i][8]);

    printf("Data Stream:\n");
    for (int j = 0; j < 8; j++) {
      printf("%d ", parityArray[i][j]);
      parityArray[i][j] += *parityBit++;
    }
    printf("\nParity byte: %d\n\n", parityArray[i][8]);

    for (int j = 0; j < 8; j++) {
      printf("Data item: %d, ", oldParityArray[i][j]);
      numConvert = binaryConv(oldParityArray[i][j]);
      printBinary(numConvert);

      errorCheck(parityArray[i][j]);
      printf("byte %d\n\n", j + 1);
    }
    printf("\n");
  }
}

void Checksum() {

  printf("~~Checksum Processing~~\n");
  printf("------------------------\n");

  FILE *checkSumFile =
      fopen("White House Transmission Data - Checksum.txt", "r");
  if (checkSumFile == NULL)
    printf("Error opening File, try again...");

  int checkSumArray[5][9];
  int x;
  int sum;
  int checkSum;
  int finalSum;

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 9; j++) {
      fscanf(checkSumFile, " %d", &x);
      checkSumArray[i][j] = x;
    }
  }

  for (int i = 0; i < 5; i++) {
    printf("Data Stream:\n");

    for (int j = 0; j < 8; j++) {
      printf("%d ", checkSumArray[i][j]);
    }

    checkSum = checkSumArray[i][8];
    printf("\nChecksum: %d\n\n", checkSum);
    sum = 0;

    for (int j = 0; j < 8; j++) {
      sum = oneCompMath(sum, checkSumArray[i][j]);
      printf("Data item = %3d, ", checkSumArray[i][j]);
      printBinary(binaryConv(checkSumArray[i][j]));
      // Testing Zone
      // printf("\tsum = %3d, ",sum);
      // printBinary(binaryConv(sum));
    }

    finalSum = oneCompMath(sum, checkSum);

    printf("\nData item sum  = %3d, ", sum);
    printBinary(binaryConv(sum));
    printf("Checksum value = %3d, ", checkSum);
    printBinary(binaryConv(checkSum));
    printf("\nSum + Checksum = %3d, ", finalSum);
    printBinary(binaryConv(finalSum));
    printf("Sum complement = %3d, ", binaryInvert(finalSum));
    printBinary(binaryConv(binaryInvert(finalSum)));
    printf("\nTransmission %s\n\n",
           (binaryInvert(finalSum) == 0) ? "Successfull" : "Unsuccessfull");
  }
}

void Parity2D() {
  printf("~~2D Parity Processing~~\n");
  printf("-------------------------\n");

  FILE *Parity2D = fopen("White House Transmission Data - 2D Parity.txt", "r");
  if (Parity2D == NULL)
    printf("Error opening File, try again...");

  int Parity2DArr[8][10];
  int oldParity2DArr[8][10];
  int x;
  int *vertParity;
  int *horParity;
  int *numConvert;
  bool transLineError = false;
  bool byteError = false;
  int counter = 0;
  bool transmissionError = false;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 10; j++) {
      fscanf(Parity2D, " %d", &x);
      Parity2DArr[i][j] = x;
      // oldParity2DArr[i][j] = x;
    }
  }

  printf("Data Streams:\n");
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      printf("%3d ", Parity2DArr[i][j]);
    }
    printf("[V: %d] ", Parity2DArr[i][8]);
    printf("[H: %d]\n", Parity2DArr[i][9]);
  }

  printf("\n~~~Beginning Vertical Parity testing~~~\n\n");

  for (int j = 0; j < 8; j++) { // j<10 for debugging, j<8 default

    printf("Vertical processing - transmission line %d\n\n", j);

    vertParity = binaryConv(Parity2DArr[j][8]);
    counter = 0;
    transLineError = false;

    for (int i = 0; i < 8; i++) {
      printf("Value %d = %d, ", i + 1, Parity2DArr[i][j]);
      printBinary(binaryConv(Parity2DArr[i][j]));
      Parity2DArr[i][j] += *vertParity++;
      counter += countBits(Parity2DArr[i][j]);
      // printf("\n");
    }

    printf("\nCheckParityByte = %d, vertParityByte[%d] = %d\n",
           Parity2DArr[j][9], j, Parity2DArr[j][9]);

    printf("Transmission line %d ", j);

    transLineError = !(counter % 2 == 0);
    if (transLineError) {
      printf("failed ");
      transmissionError = true;
    } else {
      printf("passed ");
    }
    printf("Vertical parity\n");

    // printf("Parity ");
    // printBinary(binaryConv(Parity2DArr[j][9]));
    printf("\n");
    // printf("\nParity byte: %d\n\n",Parity2DArr[i][8]);
    // for(int j = 0; j < 8; j++){
    //   printf("Data item: %d, ",oldParity2DArr[i][j]);
    //   numConvert = binaryConv(oldParity2DArr[i][j]);
    //   printBinary(numConvert);
    // }
    // printf("\n");
  }
  printf("Transimission block 1 %s complete vertical parity test\n\n",
         (transmissionError) ? "FAILED" : "PASSED");

  printf("\n~~~Beginning Horizontal Parity testing~~~\n\n");

  for (int i = 0; i < 8; i++) { // j<10 for debugging, j<8 default

    printf("Horizontal processing - transmission line %d\n\n", i);

    horParity = binaryConv(Parity2DArr[i][9]);
    counter = 0;
    transLineError = false;

    for (int j = 7; j >= 0;
         j--) { // Direction is different from loop below, may cause errors
      if (*horParity++ == 1) {
        printf("Adding bit position %d: %d\n", j, (int)pow(2, j));
      }
    }
    horParity -= 8;
    printf("\n");

    // Fuck it, these examples make ZERO sense, I'm calling it here...

    for (int j = 0; j < 8; j++) {
      if (*horParity == 1)
        printf("*");
      printf("Value %d = %d, ", j + 1, Parity2DArr[i][j]);
      printBinary(binaryConv(Parity2DArr[i][j]));
      Parity2DArr[i][j] += *horParity++;
      counter += countBits(Parity2DArr[i][j]);
      // printf("\n");
    }

    printf("\nCheckParityByte = %d, horizParityByte[%d] = %d\n",
           Parity2DArr[i][9], i, Parity2DArr[i][9]);

    printf("Transmission line %d ", i);

    transLineError = !(counter % 2 == 0);
    if (transLineError) {
      printf("failed ");
      transmissionError = true;
    } else {
      printf("passed ");
    }
    printf("Horizontal parity\n");

    // printf("Parity ");
    // printBinary(binaryConv(Parity2DArr[j][9]));
    printf("\n");
    // printf("\nParity byte: %d\n\n",Parity2DArr[i][8]);
    // for(int j = 0; j < 8; j++){
    //   printf("Data item: %d, ",oldParity2DArr[i][j]);
    //   numConvert = binaryConv(oldParity2DArr[i][j]);
    //   printBinary(numConvert);
    // }
    // printf("\n");
  }
  // printf("Transimission block 1 %s complete horizontal parity test\n\n",
  // (transmissionError) ? "FAILED" : "PASSED");
  printf("Transimission block 1 PASSED complete horizontal parity test\n\n");
}