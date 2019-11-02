
// WintestDoc.cpp: CWintestDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Wintest.h"
#endif
#include "WintestDoc.h"
#include <iostream>

#include <propkey.h>
#define BACKGROUND 255
#define FOREGROUND 0

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWintestDoc

IMPLEMENT_DYNCREATE(CWintestDoc, CDocument)

BEGIN_MESSAGE_MAP(CWintestDoc, CDocument)
//	ON_COMMAND(IDM_middle, &CWintestDoc::OnMiddle)
//ON_COMMAND(IDM_middle, &CWintestDoc::OnMiddle)
END_MESSAGE_MAP()

// CWintestDoc 생성/소멸

CWintestDoc::CWintestDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CWintestDoc::~CWintestDoc()
{
}

BOOL CWintestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CWintestDoc serialization

void CWintestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CWintestDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CWintestDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWintestDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWintestDoc 진단

#ifdef _DEBUG
void CWintestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWintestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWintestDoc 명령


BOOL CWintestDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CFile hFile;        //파일을 읽어들이면 이 클래스 형식으로 저장.        //이미지의 가로길이를 저장할 변수
	hFile.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);    //파일 열기
	hFile.Read(&dibHf, sizeof(BITMAPFILEHEADER));                          //dibHf에 파일헤더를 읽어들인다.
	//이 파일이 BMP파일인지 검사
	if (dibHf.bfType != 0x4D42)
	{
		AfxMessageBox(L"Not BMP file!!");                                        //프로젝트 생성시 유니코드를 사용하게 할 경우
		return FALSE;                                                                      //L을 붙여준다
	}
	hFile.Read(&dibHi, sizeof(BITMAPINFOHEADER));                             //영상정보의 header를 읽기
	if (dibHi.biBitCount != 8 && dibHi.biBitCount != 24)                                //8,24비트가 아닐경우
	{
		AfxMessageBox(L"Gray/True Color Possible!!");
		return FALSE;
	}
	if (dibHi.biBitCount == 8)
		//8비트의 경우 팔레트를 생성해 주어야 한다. 총 256가지 색이므로 그 길이만큼 읽어들인다
		hFile.Read(palRGB, sizeof(RGBQUAD) * 256);
	//메모리 할당
	int ImgSize;
	if (dibHi.biBitCount == 8)
	{
		ImgSize = hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - 256 * sizeof(RGBQUAD);    
		//이미지의 크기는 파일 총 길이에서, 두 헤드와 팔레트의 사이즈를 제외한다.
	}                                                                      
	else if (dibHi.biBitCount == 24) //컬러영상
	{
		ImgSize = hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	}
	//m_InImg = new unsigned char[ImgSize];
	//m_OutImg = new unsigned char[ImgSize];
	hFile.Read(m_InImg, ImgSize);
	hFile.Close();
	height = dibHi.biHeight;
	width = dibHi.biWidth;
	return TRUE;
}


BOOL CWintestDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CFile hFile;
	if (!hFile.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		return FALSE;
	//정보저장
	hFile.Write(&dibHf, sizeof(BITMAPFILEHEADER));
	hFile.Write(&dibHi, sizeof(BITMAPINFOHEADER));
	if (dibHi.biBitCount == 8)
		hFile.Write(palRGB, sizeof(RGBQUAD) * 256);
	hFile.Write(m_InImg, dibHi.biSizeImage);
	hFile.Close();
	return TRUE;
}

void CWintestDoc::Otzu_Threshold(int height, int width)
{
	register int i, t;

	// Histogram 계산
	int hist[256];
	float   prob[256];
	unsigned char orgImg[256][256];
	unsigned char* outImg;
	for (i = 0; i < 256; i++) { hist[i] = 0; prob[i] = 0.0f; }
	for (i = 0; i < height ; i++)
		for (int j = 0; j < width; j++)
		{
			hist[(int)m_OutImg[i][j]]++;
		}
	for (i = 0; i < 256; i++) prob[i] = (float)hist[i] / (float)(height * width);


	float wsv_min = 1000000.0f;
	float wsv_u1, wsv_u2, wsv_s1, wsv_s2;
	int wsv_t;

	for (t = 0; t < 256; t++)
	{
		// 누적확률 q1, q2 계산 
		float q1 = 0.0f, q2 = 0.0f;

		for (i = 0; i < t; i++)
			q1 += prob[i];
		for (i = t; i < 256; i++) q2 += prob[i];

		if (q1 == 0 || q2 == 0) continue;

		// 평균 u1, u2 계산 
		float u1 = 0.0f, u2 = 0.0f;
		for (i = 0; i < t; i++)   u1 += i * prob[i]; u1 /= q1;
		for (i = t; i < 256; i++) u2 += i * prob[i]; u2 /= q2;

		// 분산 s1, s2 계산 
		float s1 = 0.0f, s2 = 0.0f;
		for (i = 0; i < t; i++)   s1 += (i - u1) * (i - u1) * prob[i]; s1 /= q1;
		for (i = t; i < 256; i++) s2 += (i - u2) * (i - u2) * prob[i]; s2 /= q2;
		//총 분산 계산
		float wsv = q1 * s1 + q2 * s2;

		if (wsv < wsv_min)
		{
			wsv_min = wsv;
			wsv_t = t; 		// 최소치 저장
			wsv_u1 = u1; wsv_u2 = u2;
			wsv_s1 = s1; wsv_s2 = s2;
		}
	}

	// thresholding
	for (i = 0; i < height ; i++)
		for (int j = 0; j < width; j++)
		{
			if (m_OutImg[i][j] < wsv_t)
				m_OutImg[i][j] = 0;
			else
				m_OutImg[i][j] = 255;
		}
}




//void CWintestDoc::OnMiddle()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	Otzu_Threshold(m_InImg, m_OutImg, height, width);
//}

void CWintestDoc::m_Median(int height, int width)
{
	int MaskMedX[6][6] = { {1,1,1,1,1,1},{1,1,1,1,1,1},{1,1,1,1,1,1},{1, 1,1,1,1,1},{1, 1,1,1,1,1} ,{1,1,1,1,1,1} };
		int heightm1 = height - 2;
		int widthm1 = width - 2;
		int *pImgMedX;
		int arr[36] = { 0, };
		int newValue;
		pImgMedX=new int[height * width];
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				m_OutImg[i][j] = 0;
				int where = i * width + j;
				pImgMedX[where] = 0;
			}
	
		for (int i = 2; i < heightm1; i++)
		{
			for (int j = 2; j < widthm1; j++)
			{	
				newValue = 0; //0으로 초기화
				int a = 0;
				for (int mr = 0; mr < 6; mr++)
				{
					for (int mc = 0; mc < 6; mc++)
					{
						newValue = (MaskMedX[mr][mc] * m_InImg[(i + mr - 2)][j + mc -2]);
						pImgMedX[i * width + j] = newValue;
						arr[a++] = newValue;
					}
				}
				for (int q = 0; q < 36; q++)
				{
					for (int w = q + 1; w < 35; w++)
					{
						if (arr[q] > arr[w])
						{
							int tmp;
							tmp = arr[q];
							arr[q] = arr[w];
							arr[w] = tmp;
						}
						
					}
				}
				//m_OutImg[i][j] = arr[4];//배열의 중간값 출력
				for (int q = i-2; q < i+4; q++)
				{
					for (int w = j-2; w < j + 4; w++)
					{
						m_OutImg[q][w] = arr[18];
					}
				}
			}
		}

}

void CWintestDoc::grass_label(int height, int width)
{
	// 라벨링된 영상을 저장하기 위한 배열의 메모리 할당 
	int* coloring = new int[height * width];

	int i, j, curColor = 0;
	for (i = 0; i < height * width; i++) coloring[i] = 0;

	// 입력 영상의 라벨링 
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (m_InImg[i][j] == 255 && coloring[i * width + j] == 0)
			{
				curColor++;
				//m_BlobPixelSum = 0;
				grass(coloring, height, width, i, j, curColor);
			}
		}
	}

	float grayGap = 250.0f / (float)curColor;

	// 라벨링된 데이터를 m_OutImg배열을 이용하여 화면출력 
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			int value = (int)(coloring[i * width + j] * grayGap);
			if (value == 0) m_OutImg[i][j] = 255;
			else m_OutImg[i][j] = value;
		}
	}


	delete[]coloring; // 메모리 해제 
}
void CWintestDoc::grass(int* coloring, int height, int width, int i, int j, int curColor)
{
	int k, l, index;
	for (k = i - 1; k <= i + 1; k++)
	{
		for (l = j - 1; l <= j + 1; l++)
		{
			// 영상의 경계를 벗어나면 라벨링하지 않음 
			if (k < 0 || k >= height || l < 0 || l >= width) continue;

			index = k * width + l;
			// 아직 방문하지 않은 픽셀이고 값이 255라면 라벨링함 
			if (m_InImg[k][l] == 255 && coloring[index] == 0)
			{
				coloring[index] = curColor;
				//m_BlobPixelSum++;
				grass(coloring, height, width, k, l, curColor);
			}
		}
	}
}
void CWintestDoc::m_AllocStructureElementBinary(int HowMany)
{
	m_pSEBinary = new StructureElementBinary[HowMany];
}

void CWintestDoc::m_FreeStructureElementBinary()
{
	if (m_pSEBinary != NULL)
		delete[] m_pSEBinary;
}

void CWintestDoc::m_SetStructureElementBinary(int which, int row, int col)
{
	m_pSEBinary[which].row = row;
	m_pSEBinary[which].col = col;
}
void CWintestDoc::m_MorphologyBinaryErosion(int height, int width)
{
	int flagPassed;
	int r, c, i;
	int mx, my;
	float mean, sum;
	//먼저 입력 영상을 이치화함 
	sum = 0;
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			sum += m_OutImg[r][c];
	mean = sum / (height * width);
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			if (m_OutImg[r][c] > mean)
				m_OutImg[r][c] = FOREGROUND;
			else
				m_OutImg[r][c] = BACKGROUND;
	//결과 이미지를 BACKGROUND=255값으로 모두 초기화 
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			m_InImg[r][c] = BACKGROUND;
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			flagPassed = 1;
			for (i = 1; i < m_pSEBinary[0].row; i++)
			{
				mx = c + m_pSEBinary[i].col;
				my = r + m_pSEBinary[i].row;
				//범위 검사
				if (mx >= 0 && mx < width && my >= 0 && my < height)
					if (m_OutImg[my][mx] == BACKGROUND) //하나라도 BACKGROUND=255값을 포함하면 제일 안쪽 for loop를 빠져나감.
					{
						flagPassed = 0;
						break;
					}
			}
			if (flagPassed)
				m_InImg[r][c] = FOREGROUND;
		}
}

void CWintestDoc::m_BorderFollow(int height, int width)
{
	// 영역의 경계정보를 저장하기 위한 구조체 메모리 
	typedef struct tagBORDERINFO { short* x, * y; short n, dn; } BORDERINFO;
	BORDERINFO stBorderInfo[1000];

	// 영상에 있는 픽셀이 방문된 점인지를 마크하기 위해 영상 메모리 할당 
	unsigned char* visited = new unsigned char[height * width];
	memset(visited, 0, height * width * sizeof(char)); // 메모리 초기화 


	// 추적점을 임시로 저장하기 위한 메모리 
	short* xchain = new short[10000];
	short* ychain = new short[10000];

	// 관심 픽셀의 시계방향으로 주위점을 나타내기 위한 좌표 설정 
	const POINT nei[8] =           // clockwise neighbors
	{
		{1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
	};
	int c0, c1, x0, y0, k, n;	int a = 0;
	int border_count, diagonal_count, numberBorder = 0;
	for (int x = 1; x < height; x++)
	{
		for (int y = 1; y < width; y++)
		{
			c0 = m_OutImg[x][y];
			c1 = m_OutImg[x - 1][y];

			if (c0 != c1 && c0 == (unsigned char)255 && visited[x * width + y] == 0)
				//c0!=c1(경계이고)
			{

				border_count = 0;  // 경계점의 갯수를 세기 위한 카운터
				diagonal_count = 0;  // 

				x0 = x;
				y0 = y;
				n = 4;
				do
				{
					// 관심점 주위에서 같은 칼라를 가진 경계점을 찾기 위함 
					for (k = 0; k < 8; k++, n = ((n + 1) & 7))  // 01234567
					{									                              // 12345670
						short  u = (short)(x + nei[n].x);
						short  v = (short)(y + nei[n].y);
						if (u < 0 || u >= height || v < 0 || v >= width) continue;
						if (m_OutImg[u][v] == c0) break; // 관심점의 주위를 돌다가 같은 밝기의     
												 // 경계를 만나면 다음으로 추적할 점이 됨 
					}
					if (k == 8) break;    // isolated point occurs

					visited[x * width + y] = 255;  // 방문한 점으로 마크 
					xchain[border_count] = x;
					ychain[border_count++] = y;

					if (border_count >= 10000) break;

					x = x + nei[n].x;
					y = y + nei[n].y;

					if (n % 2 == 1) diagonal_count++;  // 01234567 
					n = (n + 5) & 7;                    // 56701234
				} while (!(x == x0 && y == y0));

				if (k == 8) continue;    // isolated point occurs

					  // 경계정보를 저장 
				if (border_count < 10) continue; // 너무작은 영역의 경계이면 무시한다.  

				stBorderInfo[numberBorder].x = new short[border_count];
				stBorderInfo[numberBorder].y = new short[border_count];

				for (k = 0; k < border_count; k++)
				{
					stBorderInfo[numberBorder].x[k] = xchain[k];
					stBorderInfo[numberBorder].y[k] = ychain[k];
				}
				int n = 0;
				stBorderInfo[numberBorder].n = border_count;
				n = border_count;
				if (n != 0&&n>1000)
				{
					len[a++] = n;
				}
				stBorderInfo[numberBorder++].dn = diagonal_count;

			}

		}
	}
}

void CWintestDoc::OnHoughCircle()
{
	// TODO: Add your command handler code here
	int height = 720, width = 720;
	register int i, j;

	unsigned char* orgImg = new unsigned char[height * width];
	unsigned char* outImg = new unsigned char[height * width];

	for (i = 0; i < height; i++) for (j = 0; j < width; j++) orgImg[i * width + j] = m_OutImg[i][j];

	HT_Circle(orgImg, outImg, height, width);

	for (i = 0; i < height; i++) for (j = 0; j < width; j++) m_OutImg[i][j] = outImg[i * width + j];

	delete[]orgImg;
	delete[]outImg;

	UpdateAllViews(NULL);
}

void CWintestDoc::HT_Circle(unsigned char* orgImg, unsigned char* outImg, int height, int width)
{
	register int i, j, k, ang;
	int index;
	int rr, cc,cc1;
	int r_min = 18, r_max = 100;
	float p2d = 3.141592654f / 180.0f;

	unsigned int thres = 90;

	// Memory allocation
	unsigned int*** H = new unsigned int** [height];
	for (i = 0; i < height; i++)
	{
		H[i] = new unsigned int* [width];
		for (j = 0; j < width; j++)
		{
			H[i][j] = new unsigned int[r_max];
			for (k = 0; k < r_max; k++) H[i][j][k] = 0;
		}
	}

	float* LUT_COS = new float[360];
	float* LUT_SIN = new float[360];

	for (i = 0; i < 360; i++)
	{
		LUT_COS[i] = (float)cos(i * p2d);
		LUT_SIN[i] = (float)sin(i * p2d);
	}

	for (i = 0; i < height * width; i++) outImg[i] = 255;


	// For vote
	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			if (orgImg[index + j] == 255)
			{
				for (k = r_min; k < r_max; k++)
				{
					for (ang = 0; ang < 360; ang++)
					{
						rr = (int)(i - k * LUT_COS[ang]);
						cc = (int)(j - k * LUT_SIN[ang]);

						if (rr < height && rr>0 && cc < width && cc>0) H[rr][cc][k]++;
					}
				}
			}
		}
	}

	// For display
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			for (k = r_min; k < r_max; k++)
			{
				if (H[i][j][k] > thres)
				{
					int a = k;
					if (a>=47&&a<=51)
					{
						while (a >= 0)
						{
							for (ang = 0; ang < 360; ang++)
							{
								rr = (int)(i + a * LUT_COS[ang]);
								cc = (int)(j + a * LUT_SIN[ang]);
								if (rr > 0 && rr < height && cc>0 && cc < width)
								{
									outImg[rr * width + cc] = 0;
								}

							}
							a--;
						}
					}
					else if (a >= 61 && a <= 68)
					{
						while (a >= 0)
						{
							for (ang = 0; ang < 360; ang++)
							{
								rr = (int)(i + a * LUT_COS[ang]);
								cc = (int)(j + a * LUT_SIN[ang]);
								if (rr > 0 && rr < height && cc>0 && cc < width)
								{
									outImg[rr * width + cc] = 60;
								}

							}
							a--;
						}
					}
					else if(a > 70 && a < 75)
					{
						while (a >= 0)
						{
							for (ang = 0; ang < 360; ang++)
							{
								rr = (int)(i + a * LUT_COS[ang]);
								cc = (int)(j + a * LUT_SIN[ang]);
								if (rr > 0 && rr < height && cc>0 && cc < width)
								{
									outImg[rr * width + cc] =120;
								}

							}
							a--;
						}
					}
					else if (a>=52&&a<=56)
					{
						while (a >= 18)
						{
							for (ang = 0; ang < 360; ang++)
							{
								rr = (int)(i + a * LUT_COS[ang]);
								cc = (int)(j + a * LUT_SIN[ang]);
								if (rr > 0 && rr < height && cc>0 && cc < width)
								{
									outImg[rr * width + cc] = 180;
								}

							}
							a--;
						}
					}
					else if (a>=80&&a<=86)
					{
						while (a >= 22)
						{
							for (ang = 0; ang < 360; ang++)
							{
								rr = (int)(i + a * LUT_COS[ang]);
								cc = (int)(j + a * LUT_SIN[ang]);
								if (rr > 0 && rr < height && cc>0 && cc < width)
								{
									outImg[rr * width + cc] = 240;
								}

							}
							a--;
						}
					}
				}
			}
		}
	}


	// Delete arrays
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++) delete[] H[i][j];
		delete[] H[i];
	}
	delete[]H;

	delete[]LUT_COS;
	delete[]LUT_SIN;
}
void CWintestDoc::m_EdgeSobel(int height, int width)
{
	int MaskSobelX[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
	int MaskSobelY[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} };
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int mr, mc;
	int newValue;
	int i, j;
	int* pImgSobelX, * pImgSobelY;
	int min, max, where;
	float constVal1, constVal2;

	//정수값을 갖는 이미지 동적 메모리 할당
	pImgSobelX = new int[height * width];
	pImgSobelY = new int[height * width];

	//결과 이미지 0으로 초기화
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			where = i * width + j;
			pImgSobelX[where] = 0;
			pImgSobelY[where] = 0;
		}

	//X 방향 에지 강도 계산 
	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			newValue = 0; //0으로 초기화
			for (mr = 0; mr < 3; mr++)
				for (mc = 0; mc < 3; mc++)
					newValue += (MaskSobelX[mr][mc] * m_OutImg[i + mr - 1][j + mc - 1]);
			pImgSobelX[i * width + j] = newValue;
		}
	}
	//Y 방향 에지 강도 계산 
	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			newValue = 0; //0으로 초기화
			for (mr = 0; mr < 3; mr++)
				for (mc = 0; mc < 3; mc++)
					newValue += (MaskSobelY[mr][mc] * m_OutImg[i + mr - 1][j + mc - 1]);
			pImgSobelY[i * width + j] = newValue;
		}
	}//에지 강도 계산 절대값(X)+절대값(Y)후 pImgSobelX[]에 저장
	for (i = 1; i < heightm1; i++)
		for (j = 1; j < widthm1; j++)
		{
			where = i * width + j;
			constVal1 = pImgSobelX[where];
			constVal2 = pImgSobelY[where];
			if (constVal1 < 0)
				constVal1 = -constVal1;
			if (constVal2 < 0)
				constVal2 = -constVal2;
			pImgSobelX[where] = constVal1 + constVal2;
		}
	//디스플레이를 위해 0에서 255사이로 값의 범위를 매핑
		//이를 위해 먼저 최대,최소값을 찾은후 이를 이용하여 매핑한다.
	min = (int)10e10;
	max = (int)-10e10;
	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			newValue = pImgSobelX[i * width + j];
			if (newValue < min)
				min = newValue;
			if (newValue > max)
				max = newValue;
		}
	}
	//변환시 상수값을 미리 계산 
	constVal1 = (float)(255.0 / (max - min));
	constVal2 = (float)(-255.0 * min / (max - min));
	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			//[min,max]사이의 값을 [0,255]값으로 변환 
			newValue = pImgSobelX[i * width + j];
			newValue = constVal1 * newValue + constVal2;
			m_OutImg[i][j] = (BYTE)newValue;
		}
	}

	//동적 할당 메모리 해제
	delete[] pImgSobelX;
	delete[] pImgSobelY;
}
