#ifndef __BOARD__
#define __BOARD__

typedef struct {
	char name[10];
	int score;
	int size;
} Record;

void SetSizeInBoard(int row, int col);

void CopyRecord(Record* r1, Record* r2);
void LoadBoard(Record** record, int* num);
void SaveBoard(Record* record, int num);

void StartBoard();

#endif