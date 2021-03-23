#include <iostream>
#include <random>
#include <ctime>
const int COUNT = 10000;
const int ITERCOUNT = 10000;
const int RADIUS = 100;


void initializeMatrix(int*** matrix, int rows, int columns);
void outputMatrix(int** matrix, int rows, int columns);
int* findOutRowsSums(int** matrix, int rows, int columns);
int* findOutRowsSumsWO(int** matrix, int rows, int columns);
void whatSum(int* arrays, int size);
void whatTime(clock_t start, clock_t end, const char* option);
void clearAfterMyself(int** matrix, int rows);
void clearAfterMyself(int* arrays);


int* optionOnC(int** matrix, int rows, int columns);
int* optionOnMMX(int** matrix, int rows, int columns);
int* optionOnAsm(int** matrix, int rows, int columns);
int* optionOnCwithoutOptimize(int** matrix, int rows, int columns);

/*------------MAIN----------------------*/

int main(void) {
	srand(time(NULL));
	int* arrayOfSumsC = NULL, * arrayOfSumsAsm = NULL, * arrayOfSumsCwithoutOptimize = NULL, * arrayOfSumsMMX = NULL;
	int ** matrix = NULL;
	initializeMatrix(&matrix, COUNT, COUNT);
	printf("Matrix initialized\nStarting computing...\n\n");
	
	arrayOfSumsC = optionOnC(matrix, COUNT, COUNT);
	//whatSum(arrayOfSumsC, COUNT);
	clearAfterMyself(arrayOfSumsC);
	
	arrayOfSumsAsm= optionOnAsm(matrix, COUNT, COUNT);
	//whatSum(arrayOfSumsAsm, COUNT);
	clearAfterMyself(arrayOfSumsAsm);

	arrayOfSumsMMX = optionOnMMX(matrix, COUNT, COUNT);
	//whatSum(arrayOfSumsMMX, COUNT);
	clearAfterMyself(arrayOfSumsMMX);
	
	arrayOfSumsCwithoutOptimize = optionOnCwithoutOptimize(matrix, COUNT, COUNT);
	//whatSum(arrayOfSumsCwithoutOptimize, COUNT);
	clearAfterMyself(arrayOfSumsCwithoutOptimize);
	
	clearAfterMyself(matrix, COUNT);
	return 0;

}


/*---------------FUNCTIONS--------------------*/

void initializeMatrix(int*** matrix, int rows, int columns)
{
	*matrix = (int**)malloc(sizeof(int*) * rows);
	for (int i = 0; i < rows; i++) {
		(*matrix)[i] = (int*)malloc(sizeof(int) * columns);
		for (int k = 0; k < columns; k++) {
			(*matrix)[i][k] = rand() % RADIUS ;
		}
	}
}


void outputMatrix(int** matrix, int rows, int columns)
{
	for (int i = 0; i < rows; i++) {
		for (int k = 0; k < columns; k++) {
			std::cout << matrix[i][k] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl<<std::endl;
}



void whatSum(int* arrays, int size)
{
	std::cout << "Array of sums : \n";
	for (int i = 0; i < size; i++)
		std::cout << arrays[i] << " ";
	std::cout << "\n\n";
}
void whatTime(clock_t start, clock_t end, const char* option) {
	double time1 = ((double)(end - start) / CLK_TCK);
	std::cout << "Time - " << option <<" - "<<(double)time1 << " sec\n";
}

void clearAfterMyself(int** matrix, int rows)
{
	for (int i = 0; i < rows; i++)
		free(matrix[i]);
	free(matrix);
}

void clearAfterMyself(int* arrays)
{
	free(arrays);
}

#pragma optimize("", off)
int* findOutRowsSumsWO(int** matrix, int rows, int columns)
{
	int* sumArray = (int*)malloc(sizeof(int) * rows);
	int tempSum = 0;
	for (int D = 0; D < ITERCOUNT; D++) {
		for (int i = 0; i < rows; i++) {
			for (int k = 0; k < columns; k++) {
				tempSum += matrix[i][k];
			}
			sumArray[i] = tempSum;
			tempSum = 0;
		}
	}
	return sumArray;
}
#pragma optimize("", on)

int* findOutRowsSums(int** matrix, int rows, int columns)
{
	int* sumArray = (int*)malloc(sizeof(int) * rows);
	int tempSum = 0;
	for (int D = 0; D < ITERCOUNT; D++) {
		for (int i = 0; i < rows; i++) {
			for (int k = 0; k < columns; k++) {
				tempSum += matrix[i][k];
			}
			sumArray[i] = tempSum;
			tempSum = 0;
		}
	}
	return sumArray;
}

int* optionOnC(int** matrix, int rows, int columns)
{
	int* arrayOfSumsC;
	clock_t start = clock();
	arrayOfSumsC = findOutRowsSums(matrix, COUNT, COUNT);
	clock_t end = clock();
	whatTime(start, end, "C");
	return arrayOfSumsC;
}

#pragma optimize("", off)
int* optionOnCwithoutOptimize(int** matrix, int rows, int columns)
{
	clock_t start = clock();

	int* sumArray = NULL;
	int tempSum = 0;
	sumArray = findOutRowsSumsWO(matrix, rows, columns);
	clock_t end = clock();
	whatTime(start, end, "C : without optimization");
	return sumArray;
}
#pragma optimize("", on)

int* optionOnMMX(int** matrix, int rows, int columns)
{
	int* arrayOfSumsAsm = (int*)malloc(sizeof(int) * rows);
	clock_t start, end;
	start = clock();

	int32_t middle = (columns / 2);
	int D;
	for ( D = 0; D < ITERCOUNT; D++) {
		_asm {
			mov ecx, rows
			mov ebx, [matrix]
			mov edi, [arrayOfSumsAsm]
			mov edx, middle
			mov esi, 1
			and esi, edx
		
			ROWS:
				push ecx
				mov eax, DWORD PTR [ebx]
				pxor mm7, mm7
				mov ecx, edx
					COLUMNS :
					paddd mm7, QWORD PTR [eax]
					paddd mm7, QWORD PTR [eax + (edx)*4]
					add eax, 4
					loop COLUMNS
				cmp esi, 1
				je  ADDITION
				jmp BACK
				ADDITION:
				paddd mm7, QWORD PTR[eax + edx * 4]
				
				
				BACK:
				pop ecx
				movd QWORD PTR [edi], mm7
				add edi, 4
				add ebx, 4
				loop ROWS
			emms
		}
	}
	
	
	end = clock();
	whatTime(start, end, "Assembler : MMX");
	return arrayOfSumsAsm;
}

int* optionOnAsm(int** matrix, int rows, int columns)
{
	int* arrayOfSumsAsm = (int*)malloc(sizeof(int) * rows);

	clock_t start = clock();
	for (int D = 0; D < ITERCOUNT; D++) {
		_asm {
			xor edx, edx
			xor esi, esi
			xor edi, edi 
			xor ebx, ebx
			mov ecx, rows
			COLUMNS_LOOP :
			push ecx
				mov ebx, matrix
				mov ebx, [ebx + esi * 4]
				xor edx, edx
				xor eax, eax
				mov ecx, columns
				ADD_ROWS_LOOP :
			add eax, DWORD PTR[ebx + edx]
				add edx, 4
				loop ADD_ROWS_LOOP
				mov ebx, [arrayOfSumsAsm]
				mov[ebx + esi * 4], eax
				inc esi
				pop ecx
				loop COLUMNS_LOOP
		}
	}
	clock_t end = clock();
	whatTime(start, end, "Assembler");
	return arrayOfSumsAsm;
}
