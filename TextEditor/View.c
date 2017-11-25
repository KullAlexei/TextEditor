#define _CRT_SECURE_NO_WARNINGS

#include <windowsx.h>
#include <Richedit.h>
#include <stdio.h>
#include <share.h>
#include "View.h"

void DelCBorders(HWND hwnd)
{
	LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	lExStyle &= ~(WS_EX_CLIENTEDGE);
	SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
}

OPENFILENAME InitOFN(HWND hwnd,  char *szOpenedFileName)
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "��������� ��������� (*.txt)\0*.txt\0��� ����� (*.*)\0*.*\0";
	ofn.lpstrFile = szOpenedFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";
	
	return ofn;
}

OPENFILENAME InitSFN(HWND hwnd, char *szSavedFileName)
{
	OPENFILENAME sfn;

	ZeroMemory(&sfn, sizeof(sfn));

	sfn.lStructSize = sizeof(sfn);
	sfn.hwndOwner = hwnd;
	sfn.lpstrFilter = "��������� ��������� (*.txt)\0*.txt\0��� ����� (*.*)\0*.*\0";
	sfn.lpstrFile = szSavedFileName;
	sfn.nMaxFile = MAX_PATH;
	sfn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	sfn.lpstrDefExt = "txt";

	return sfn;
}

BOOLEAN openFile(HWND hwnd, FILE *stream, const char *szOpenedFileName, const char *mode)
{
	errno_t err = fopen_s((FILE**)stream, szOpenedFileName, mode);

	if (err)
	{
		char text[512];
		sprintf_s(text, 512, "�� ������� ������� ���� \"%s\".", szOpenedFileName);
		MessageBox(hwnd, text, "������", MB_OK | MB_ICONERROR);
		
		return FALSE;
	}

	return TRUE;
}

void append(char *subject, const char *insert, int pos) {
	// ��������� ������ �� 2 ���� ������, ��� ��� ���������� ������� ������������ ������ ���� �����
	char *buf = (char*)calloc(strlen(subject) + strlen(insert) + 2, sizeof(char));
	int len;

	strncpy(buf, subject, pos); // �������� � buf �� subject pos-��������
	len = strlen(buf);
	strcpy(buf + len, insert); // �������� � ����� buf ��� ������� �� insert
	len += strlen(insert); 
	strcpy(buf + len, subject + pos); // �������� � ����� buf ���������� � subject �������

	strcpy(subject, buf); // �������� �� buf � subject 
}

void getCaretPos(HWND hwndEdit, PCOORD pos)
{	
	DWORD endSel = 0;
	LONG firstCharacter;
	
	// ��������� ������, �� ������� ��������� ������� (�������� wParam = -1, ����� �������� ������� ������� �������)				
	pos->Y = (LONG)SendMessage(hwndEdit, EM_LINEFROMCHAR, (WPARAM)-1, 0) + 1;
	
	// ��������� �������, � ������� ��������� �������		
	SendMessage(hwndEdit, EM_GETSEL, 0, (LPARAM)&endSel);

	firstCharacter = SendMessage(hwndEdit, EM_LINEINDEX, (WPARAM)-1, 0);
	pos->X = endSel - firstCharacter;
}