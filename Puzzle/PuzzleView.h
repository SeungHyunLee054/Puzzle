
// PuzzleView.h: CPuzzleView 클래스의 인터페이스
//

#pragma once


class CPuzzleView : public CView
{
protected: // serialization에서만 만들어집니다.
	CPuzzleView() noexcept;
	DECLARE_DYNCREATE(CPuzzleView)

// 특성입니다.
public:
	CPuzzleDoc* GetDocument() const;

// 작업입니다.
public:
	int pic[4][4];
	int m_picnum;
	BOOL m_bStart;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CPuzzleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	void Onshuffle();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFileNew();
	afx_msg void OnShufflepic();
	afx_msg void OnChangepic();
	int CheckWansung();
};

#ifndef _DEBUG  // PuzzleView.cpp의 디버그 버전
inline CPuzzleDoc* CPuzzleView::GetDocument() const
   { return reinterpret_cast<CPuzzleDoc*>(m_pDocument); }
#endif

