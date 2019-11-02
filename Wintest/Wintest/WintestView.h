
// WintestView.h: CWintestView 클래스의 인터페이스
//

#pragma once
#include"WintestDoc.h"

class CWintestView : public CView
{
protected: // serialization에서만 만들어집니다.
	CWintestView() noexcept;
	DECLARE_DYNCREATE(CWintestView)

// 특성입니다.
public:
	CWintestDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	BITMAPINFO* BmInfo;            //비트맵 영상헤드를 선언
protected:

// 구현입니다.
public:
	virtual ~CWintestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnMiddle();
	afx_msg void OnMiddle();
//	afx_msg void OnBe();
};

#ifndef _DEBUG  // WintestView.cpp의 디버그 버전
inline CWintestDoc* CWintestView::GetDocument() const
   { return reinterpret_cast<CWintestDoc*>(m_pDocument); }
#endif

