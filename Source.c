#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
//--------------------------------------------------------------------------------
struct SSDateOfBirth{
  int m_nDay;
  int m_nMonth;
  int m_nYear;
};
//--------------------------------------------------------------------------------
struct SStudent{
  char m_cSurname[20];
  char m_cName[20];
  struct SSDateOfBirth m_sDate;   
  int m_nMarks[3];
  double m_dGPA;
  struct SStudent *pNext;
};
//--------------------------------------------------------------------------------
//�������� �������������� ������� �������
void TabElem();
//����� ���������� � �����
void ScanData(struct SStudent *p, FILE *input);
//���� ���������� �����
void Tab(struct SStudent *start);
//���� ����� ���� �������
void Tab2(struct SStudent *start);
//������ ������� ���
double findGPA(struct SStudent *p);
//���������� �� ������� ����� �� ���������
void SortingGPA(struct SStudent *start);
//��������, �� ������ ������ ������� �� ������� ��� ������,
//�� 4.5, ���� ���, �� �-� ������� ����� �������� � ������
void CheckFirstElemGPA(struct SStudent **pStart);
//������ � ������ �������� ����, �� ���������� �����
void SortingSummer(struct SStudent *start);
//��������, �� ������ ������� ��������� �����
//���� �, �� ������� ���� � ������
void CheckFirstElemSummer(struct SStudent **pStart);
//���������� �� ������� �����
struct SStudent *BubbleSort(struct SStudent *start);
//���� ������ � ������ ���� ��������
struct SStudent *swap(struct SStudent *p);
//�������� �������� � ����� ������
void insert(struct SStudent **pStart);
//������� ���'���
void FreeList(struct SStudent **pStart);
//--------------------------------------------------------------------------------
int main(){

    FILE* fInput;
    fInput = fopen("input.txt", "r");
    if (!fInput) return 1;
    struct SStudent *p = (struct SStudent*)malloc(sizeof(struct SStudent));
    struct SStudent *pStart = (struct SStudent*)malloc(sizeof(struct SStudent));

    pStart->pNext = NULL;
    while (!feof(fInput)) {
      ScanData(p, fInput);
      if (pStart->pNext == NULL) {
        pStart = p;
      }
      if (!feof(fInput)) {
        struct SStudent *pp = (struct SStudent*)calloc(1, sizeof(struct SStudent));
        p->pNext = pp;
        p = pp;
      }
    }
    fclose(fInput);
    fInput = NULL;
    printf("\n Before sorting:\n");
    Tab(pStart);

    SortingGPA(pStart);
    CheckFirstElemGPA(&pStart);
    pStart = BubbleSort(pStart);
    printf(" After removing students with GPA more than 4.5:\n");
    Tab2(pStart);

    SortingSummer(pStart);
    CheckFirstElemSummer(&pStart);
    printf(" After summer sorting:\n");
    Tab2(pStart);


    insert(&pStart);
    pStart = BubbleSort(pStart);
    printf("\n After insert:\n");
    Tab2(pStart);

    FreeList(&pStart);
    return 0;
}
//--------------------------------------------------------------------------------
//�-� ����� ��� ��� �������� � �����, ��� ������� �������� �������� �� ������
//������� ������ �� ��'� ����� �����
void ScanData(struct SStudent *p, FILE *input){
  struct SStudent *add = p;
  fscanf(input, "%s %s", &add->m_cSurname, &add->m_cName);
  fscanf(input, "%d %d %d", &add->m_sDate.m_nDay, &add->m_sDate.m_nMonth, &add->m_sDate.m_nYear);
  fscanf(input, "%d %d %d", &add->m_nMarks[0], &add->m_nMarks[1], &add->m_nMarks[2]);
}
//--------------------------------------------------------------------------------
//�-� �������� ������ � �������, �������� �������� �� ������
void Tab(struct SStudent *start){
  TabElem();
  printf(" ||Surname\t||Name\t\t||Date of birth\t||Marks\t\t|| GPA\t\t||\n");
  TabElem();
  while (start != NULL){
    printf(" ||%10s\t||%10s\t||%2d.%2d.%d"
    "", start->m_cSurname, start->m_cName, start->m_sDate.m_nDay, start->m_sDate.m_nMonth, start->m_sDate.m_nYear);
    printf("\t||%2d  %2d  %2d\t||%.2lf\t\t"
    "||\n", start->m_nMarks[0], start->m_nMarks[1], start->m_nMarks[2], findGPA(start));
    start = start->pNext;
  }
  TabElem();
  printf("\n");
}
//--------------------------------------------------------------------------------
void TabElem(){
  printf(" ==================================================="
      "==============================\n");
}
//--------------------------------------------------------------------------------
//�-� ��������� ������� ���, �������� �������� �� ������� ������
//������� ����� ����� ���� double
double findGPA(struct SStudent *p) {
  double g;
  g = p->m_nMarks[0] + p->m_nMarks[1] + p->m_nMarks[2];
  p->m_dGPA = g / 3;
  return p->m_dGPA;
}
//--------------------------------------------------------------------------------
//�-� ����� ������� �� ������� ����� � ������� ��������
//��� ������� �������� �������� �� ������� ������
void SortingGPA(struct SStudent *start) {
  struct SStudent *p = start;
  while (1) {
    struct SStudent* pp = p;
    p = p->pNext;
    if (!p) break; /* ��� ��������� ���� ������ ����� � ����� */
    if (p->m_dGPA >= 4.5) {
      if (!p->pNext) {
        pp->pNext = NULL; /* �������, �� ��������� ������� ������ ���
������� ���������, � �������� � ����� */
        break;
        }
      pp->pNext = p->pNext; /* ���� � ��������� ������� ����, �� ������� � ���
��������� �������, � �������� ������� p ��������� */
      free(p);
      p = pp;
    }
  }
}
//--------------------------------------------------------------------------------
//�-� ��������, �� ������ ������� �� ������� ��� ������ �� 4.5
// ���� ���, �� ������� ����� �������� � ������
//��� ������� �������� �������� �������� �� ������� ������
void CheckFirstElemGPA(struct SStudent **pStart) {
  struct SStudent *p = *pStart;
  if (p->m_dGPA >= 4.5) {
    if (p->pNext)
      *pStart = p->pNext;
    else pStart = NULL;
  }
  free(p);
}
//--------------------------------------------------------------------------------
//�-� ������� � ������ ���, ��� �� ��������� �����
//��� ������� �������� �������� �� ������� ������
void SortingSummer(struct SStudent *start) {
  struct SStudent *p = start;
  while (1) {
    struct SStudent *pp = p;
    p = p->pNext;
    if (!p) break; /* ��� ��������� ���� ������ ����� � ����� */
    if ((p->m_sDate.m_nMonth != 7) && (p->m_sDate.m_nMonth != 8) && (p->m_sDate.m_nMonth != 9)) {
      if (!p->pNext) {
        pp->pNext = NULL; /* �������, �� ��������� ������� ������ ���
������� ���������, � �������� � ����� */
        break;
      }
      pp->pNext = p->pNext; /* ���� � ��������� ������� ����, �� ������� � ���
��������� �������, � �������� ������� p ��������� */
      free(p);
      p = pp;
    }
  }
}
//--------------------------------------------------------------------------------
//�-� ��������, �� ������ ������� ��������� �����
// ���� �, �� ������� ����� �������� � ������
//��� ������� �������� �������� �������� �� ������� ������
void CheckFirstElemSummer(struct SStudent ** pStart) {
  struct SStudent* p = *pStart;
  if ((p->m_sDate.m_nMonth != 7) && (p->m_sDate.m_nMonth != 8) && (p->m_sDate.m_nMonth != 9)) {
    if (p->pNext)
      *pStart = p->pNext;
    else pStart = NULL;
  }
  free(p);
}
//--------------------------------------------------------------------------------
//�-� ����� �������� �� ��������� ������������ ����
//������� �������� �� ������ �������
//��������������� ���: pStart = BubbleSort(pStart);
//�� ����  CheckFirstElemGPA(&pStart); ���  CheckFirstElemSummer(&pStart);
struct SStudent* BubbleSort(struct SStudent *start){
  struct SStudent *pp = start;
  while (1){
    struct SStudent *p = (struct SStudent*)calloc(1, sizeof(struct SStudent));
    p->pNext = pp;
      int i = 0;
    while (p->pNext->pNext != NULL){
      if ((p->pNext->m_dGPA) > (p->pNext->pNext->m_dGPA)){
        i = 1;
        if (p->pNext == pp){
         pp = swap(p);
        } else {
         swap(p);
        }
      }
      p = p->pNext;
    }
    if (!i){
      return pp;
    }
  }
}
//--------------------------------------------------------------------------------
//�-� ���� ������ �������� � ������
struct SStudent* swap(struct SStudent *p){
  struct SStudent *tmp;
  int i = 0;
  tmp = p->pNext;
  p->pNext = p->pNext->pNext;
  tmp->pNext = tmp->pNext->pNext;
  p->pNext->pNext = tmp;
  return p->pNext;
}
//--------------------------------------------------------------------------------
//�-� �������� ���������� ������ � ������ �������
//����������� �� �-� Tab ���, �� �� �������� ����������� ���,
//� ����������� ��� �����������
void Tab2(struct SStudent *start){
  TabElem();
  printf(" ||Surname\t||Name\t\t||Date of birth\t||Marks\t\t|| GPA\t\t||\n");
  TabElem();
  while (start != NULL){
    printf(" ||%10s\t||%10s\t||%2d.%2d.%d"
    "", start->m_cSurname, start->m_cName, start->m_sDate.m_nDay, start->m_sDate.m_nMonth, start->m_sDate.m_nYear);
    printf("\t||%2d  %2d  %2d\t||%.2lf\t"
    "\t||\n", start->m_nMarks[0], start->m_nMarks[1], start->m_nMarks[2], start->m_dGPA);
    start = start->pNext;
  }
  TabElem();
  printf("\n");
}
//--------------------------------------------------------------------------------
//�-� ���������� ������ �������� � ������, ��������� ����� � ���������
void insert(struct SStudent **pStart){
  char str[50];
  int temp = 0;
  struct SStudent *first = (struct SStudent*)calloc(1, sizeof(struct SStudent));
  printf(" Enter surname: ");
  fgets(str, sizeof(str), stdin);
  str[strlen(str) - 1] = 0;
  strcpy(first->m_cSurname, str);
  printf(" Enter name: ");
  fgets(str, sizeof(str), stdin);
  str[strlen(str) - 1] = 0;
  strcpy(first->m_cName, str);
  printf(" Enter day of birth: ");
  scanf("%d", &temp);
  first->m_sDate.m_nDay = temp;
  printf(" Enter month of birth: ");
  scanf("%d", &temp);
  first->m_sDate.m_nMonth = temp;
  printf(" Enter year of birth: ");
  scanf("%d", &temp);
  first->m_sDate.m_nYear = temp;
  printf(" Enter 1 mark: ");
  scanf("%d", &temp);
  first->m_nMarks[0] = temp;
  printf(" Enter 2 mark: ");
  scanf("%d", &temp);
  first->m_nMarks[1] = temp;
  printf(" Enter 3 mark: ");
  scanf("%d", &temp);
  first->m_nMarks[2] = temp;
  first->m_dGPA = findGPA(first);
  first->pNext = (*pStart);
  (*pStart) = first;
}
//--------------------------------------------------------------------------------
void FreeList(struct SStudent **pStart){
  struct SStudent *pTemp = *pStart;
  while (pTemp){
    *pStart = pTemp->pNext;
    free(pTemp);
    pTemp = *pStart;
  }
  *pStart = NULL;
}

