
// WintestView.cpp: CWintestView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Wintest.h"
#endif

#include "WintestDoc.h"
#include "WintestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWintestView

IMPLEMENT_DYNCREATE(CWintestView, CView)

BEGIN_MESSAGE_MAP(CWintestView, CView)
//	ON_COMMAND(IDM_middle, &CWintestView::OnMiddle)
ON_COMMAND(IDM_middle, &CWintestView::OnMiddle)
//ON_COMMAND(ID_BE, &CWintestView::OnBe)
END_MESSAGE_MAP()

// CWintestView 생성/소멸

CWintestView::CWintestView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i < 256; i++)
	{
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbReserved = 0;
	}
}

CWintestView::~CWintestView()
{
	if (BmInfo) delete BmInfo;
}

BOOL CWintestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CWintestView 그리기

void CWintestView::OnDraw(CDC* pDC)
{
	CWintestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
#define WIDTHBYTES(bits)	(((bits)+31)/32*4);        //이미지 가로 바이트 길이는 4바이트의 배수
	//BmInfo;
#define BYTE unsigned char
	int height;
	int width;
	int rwsize;
	if (!pDoc)
		return;
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (pDoc->m_InImg == NULL)
		return;
	//24비트 비트맵 파일의 영상출력

	if (pDoc->dibHi.biBitCount == 24) {
		height = pDoc->dibHi.biHeight;
		width = pDoc->dibHi.biWidth;
		rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount * pDoc->dibHi.biWidth);
		BmInfo->bmiHeader = pDoc->dibHi;
		//SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height, 0, 0, 0, height, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS);
	}
	else	//8비트 컬러일 경우
	{
		int index;
		rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount * pDoc->dibHi.biWidth);
		//팔레트를 읽어들이며 반복출력

		for (int i = 0; i < pDoc->dibHi.biHeight; i++)
			for (int j = 0; j < pDoc->dibHi.biWidth; j++) {
				index = pDoc->m_InImg[i][ j];
				BYTE R = pDoc->palRGB[index].rgbRed;
				BYTE G = pDoc->palRGB[index].rgbGreen;
				BYTE B = pDoc->palRGB[index].rgbBlue;
				//pDC->SetPixel(j,i,RGB(R,G,B));

 //영상 반전출력

				//pDC->SetPixel(j, pDoc->dibHi.biHeight - i - 1, RGB(R, G, B));
			}
		for (int i = 0; i < pDoc->dibHi.biHeight; i++)
		{
			for (int j = 0; j < pDoc->dibHi.biWidth; j++)
			{
				unsigned char InVal = pDoc->m_InImg[i][j];
				unsigned char OutVal = pDoc->m_OutImg[i][j];

				pDC->SetPixel(j, pDoc->dibHi.biHeight - i - 1, RGB(InVal, InVal, InVal));
				pDC->SetPixel(j + 750, pDoc->dibHi.biHeight - i - 1, RGB(OutVal, OutVal, OutVal));
			}
		}
	}
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CWintestView 진단

#ifdef _DEBUG
void CWintestView::AssertValid() const
{
	CView::AssertValid();
}

void CWintestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWintestDoc* CWintestView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWintestDoc)));
	return (CWintestDoc*)m_pDocument;
}
#endif //_DEBUG


// CWintestView 메시지 처리기



void CWintestView::OnMiddle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWintestDoc* pDoc = GetDocument();
	pDoc->m_Median(pDoc->height, pDoc->width);
	pDoc->Otzu_Threshold(pDoc->height, pDoc->width);
	pDoc->m_EdgeSobel(pDoc->height, pDoc->width);
	pDoc->OnHoughCircle();
//	pDoc->grass_label(pDoc->height, pDoc->width);
//	pDoc->m_BorderFollow(pDoc->height, pDoc->width);
	Invalidate(FALSE);
}


//void CWintestView::OnBe()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CWintestDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//
//	//구조 요소를 생성후 값을 지정 
//	pDoc->m_AllocStructureElementBinary(4);
//	pDoc->m_SetStructureElementBinary(0, 4, 0);
//	pDoc->m_SetStructureElementBinary(1, 0, -1);
//	pDoc->m_SetStructureElementBinary(2, 0, 0);
//	pDoc->m_SetStructureElementBinary(3, 0, 1);
//
//	pDoc->m_MorphologyBinaryErosion(256, 256);
//
//	//동적 할당 메모리 해제 
//	pDoc->m_FreeStructureElementBinary();
//
//	Invalidate(FALSE); // 화면 갱신
//}
