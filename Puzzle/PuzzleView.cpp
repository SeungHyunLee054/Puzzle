
// PuzzleView.cpp: CPuzzleView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Puzzle.h"
#endif

#include "PuzzleDoc.h"
#include "PuzzleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPuzzleView

IMPLEMENT_DYNCREATE(CPuzzleView, CView)

BEGIN_MESSAGE_MAP(CPuzzleView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FILE_NEW, &CPuzzleView::OnFileNew)
	ON_COMMAND(ID_SHUFFLEPIC, &CPuzzleView::OnShufflepic)
	ON_COMMAND(ID_CHANGEPIC, &CPuzzleView::OnChangepic)
END_MESSAGE_MAP()

// CPuzzleView 생성/소멸

CPuzzleView::CPuzzleView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	int k = 0;

	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			pic[row][col] = k++;
	pic[3][3] = -1;
	m_picnum = 0;
	m_bStart = false;
}

CPuzzleView::~CPuzzleView()
{
}

BOOL CPuzzleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CPuzzleView 그리기

void CPuzzleView::OnDraw(CDC* pDC)
{
	CPuzzleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	int picID[2] = { IDB_BITMAP1,IDB_BITMAP2 };

	CDC memdc;

	memdc.CreateCompatibleDC(pDC);
	CBitmap bitmap, * oldbitmap;

	bitmap.LoadBitmap(picID[m_picnum]);

	oldbitmap = memdc.SelectObject(&bitmap);

	pDC->StretchBlt(600, 0, 250, 250, &memdc, 0, 0, 500, 500, SRCCOPY);

	memdc.Rectangle(375, 375, 500, 500);

	pDC->StretchBlt(0, 0, 500, 500, &memdc, 0, 0, 500, 500, SRCCOPY);

	int sx, sy, ex, ey;
	int row, col, val, nrow, ncol;

	for (row = 0; row < 4; row++) {
		for (col = 0; col < 4; col++) {
			val = pic[row][col];
			if (val == -1) {
				sy = row * 125;
				sx = col * 125;
				ey = 3 * 125;
				ex = 3 * 125;

				pDC->StretchBlt(sx, sy, 125, 125, &memdc, ex, ey, 125, 125, SRCCOPY);
			}
			else {
				nrow = val / 4;
				ncol = val % 4;
				sy = row * 125;
				sx = col * 125;
				ey = nrow * 125;
				ex = ncol * 125;

				pDC->StretchBlt(sx, sy, 125, 125, &memdc, ex, ey, 125, 125, SRCCOPY);
			}
		}
	}

	for (row = 0; row < 5; row++) {
		sx = 0; sy = 125 * row;
		ex = 500; ey = 125 * row;
		pDC->MoveTo(sx, sy);
		pDC->LineTo(ex, ey);
	}
	for (col = 0; col < 5; col++) {
		sx = 125 * col; sy = 0;
		ex = 125 * col; ey = 500;
		pDC->MoveTo(sx, sy);
		pDC->LineTo(ex, ey);
	}
	memdc.SelectObject(oldbitmap);
}


// CPuzzleView 인쇄

BOOL CPuzzleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CPuzzleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CPuzzleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CPuzzleView 진단

#ifdef _DEBUG
void CPuzzleView::AssertValid() const
{
	CView::AssertValid();
}

void CPuzzleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPuzzleDoc* CPuzzleView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPuzzleDoc)));
	return (CPuzzleDoc*)m_pDocument;
}
#endif //_DEBUG


// CPuzzleView 메시지 처리기


void CPuzzleView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int row, col, row1, col1, row2, col2;

	if (m_bStart == false) return;

	row = point.y / 125;
	col = point.x / 125;

	if (pic[row][col] == -1) return;

	row1 = row - 1;
	col1 = col - 1;
	row2 = row + 1;
	col2 = col + 1;

	if (row1 >= 0 && pic[row1][col] == -1) {
		pic[row1][col] = pic[row][col];
		pic[row][col] = -1;
	}

	else if (col1 >= 0 && pic[row][col1] == -1) {
		pic[row][col1] = pic[row][col];
		pic[row][col] = -1;
	}

	else if (row2 <= 3 && pic[row2][col] == -1) {
		pic[row2][col] = pic[row][col];
		pic[row][col] = -1;
	}

	else if (col2 <= 3 && pic[row][col2] == -1) {
		pic[row][col2] = pic[row][col];
		pic[row][col] = -1;
	}

	Invalidate(false);
	if (CheckWansung()) {
		AfxMessageBox(_T("그림이 완성되었습니다."), MB_OK);
		OnChangepic();
	}
	CView::OnLButtonDown(nFlags, point);
}


void CPuzzleView::OnFileNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Onshuffle();
	m_bStart = true;
}


void CPuzzleView::Onshuffle()
{
	// TODO: 여기에 구현 코드 추가.
	int temp[16], k;

	for (int i = 0; i < 16; i++)
		temp[i] = 100;

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			if (row == 3 && col == 3) {
				pic[row][col] = -1;
				break;
			}

			srand((unsigned)time(NULL));
			k = rand() % 15;
			while (k == temp[k]) {
				k = rand() % 15;
			}
			pic[row][col] = k;
			temp[k] = k;
		}
	}
	Invalidate(false);
}


void CPuzzleView::OnChangepic()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_picnum == 0) m_picnum = 1;
	else m_picnum = 0;

	int k = 0;

	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			pic[row][col] = k++;
	pic[3][3] = -1;

	m_bStart = false;
	Invalidate(true);
}


void CPuzzleView::OnShufflepic()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Onshuffle();
	m_bStart = true;
	Invalidate(false);
}


int CPuzzleView::CheckWansung()
{
	// TODO: 여기에 구현 코드 추가.
	int num = 0;
	int result = 0;

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			if (row == 3 && col == 3 && pic[row][col] == -1);
			else if (pic[row][col] == num++);
			else result++;
		}
	}

	if (result == 0)
		return 1;
	else
		return 0;
}
