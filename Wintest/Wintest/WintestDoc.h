
// WintestDoc.h: CWintestDoc 클래스의 인터페이스
//


#pragma once

class CWintestDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CWintestDoc() noexcept;
	DECLARE_DYNCREATE(CWintestDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CWintestDoc();
public:
	
	unsigned char m_InImg[720][720];       // bmp파일의 이미지 내용 저장
	unsigned char m_OutImg[720][720];  // bmp이미지 출력용 배열
	int len[1000];
	int height;        //이미지의 높이를 저장할 변수
	int width;
	BITMAPFILEHEADER dibHf;    // 비트맵 파일헤드 구조체
	BITMAPINFOHEADER dibHi;    // 비트맵 영상헤드 구조체
	RGBQUAD palRGB[256];        // 팔레트 정보 구조체 배열
	void grass(int* coloring, int height, int width, int i, int j, int curColor);
	void grass_label(int height, int width);
	int arr[256];
	int space[256];
	typedef struct {
		int row, col;     //좌표값 
	} StructureElementBinary, * pStructureElementBinary;

	pStructureElementBinary m_pSEBinary;

	void m_AllocStructureElementBinary(int HowMany);
	void m_FreeStructureElementBinary();
	void m_SetStructureElementBinary(int which, int row, int col);

	void m_MorphologyBinaryDilation(int height, int width);
	void m_MorphologyBinaryErosion(int height, int width);
	void HT_Circle(unsigned char* orgImg, unsigned char* outImg, int height, int width);
	void	OnHoughCircle();
	void m_EdgeSobel(int height, int width);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void Otzu_Threshold(int height, int width);
	void m_Median(int height, int width);
	void m_BorderFollow(int height, int width);
//	afx_msg void OnMiddle();
//	afx_msg void OnMiddle();
};
