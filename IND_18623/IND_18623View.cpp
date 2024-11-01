
// IND_18623View.cpp : implementation of the CIND18623View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18623.h"
#endif

#include "IND_18623Doc.h"
#include "IND_18623View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND18623View

IMPLEMENT_DYNCREATE(CIND18623View, CView)

BEGIN_MESSAGE_MAP(CIND18623View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND18623View construction/destruction

CIND18623View::CIND18623View() noexcept
{
	// TODO: add construction code here
	auto current_directory = file_utils::get_current_directory();
	
	green_part_path = file_utils::combine_paths(current_directory, green_part_name);

	yellow_part_path = file_utils::combine_paths(current_directory, yellow_part_name);

}

CIND18623View::~CIND18623View()
{
}

BOOL CIND18623View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND18623View drawing

void CIND18623View::draw_trapezoid(CDC *pDC, CPoint position, int v_side, int h_side)
{
	return std::vector<CPoint>();
}

void CIND18623View::draw_grid(CDC* pDC, int &grid_width, int &grid_height, int &grid_unit_size) {
	CPen* OldPen;
	CPen gridPen(BS_SOLID, 1, RGB(0, 255, 255));
	OldPen = pDC->SelectObject(&gridPen);
	int x_offset = 25;
	int y_offset = 25;
	for (int i = 0; i <= grid_width / grid_unit_size; i++) {
		pDC->MoveTo(grid_unit_size * i, 0);
		pDC->LineTo(grid_unit_size * i, grid_height);
	}
	for (int i = 0; i <= grid_height / grid_unit_size; i++) {
		pDC->MoveTo(0, grid_unit_size * i);
		pDC->LineTo(grid_width, grid_unit_size * i);
	}
	pDC->SelectObject(OldPen);
}

void CIND18623View::OnDraw(CDC* pDC)
{
	CIND18623Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	int g_width = 500, g_height = 500, g_unit_size = 20;

	if (do_grid_draw)
		draw_grid(pDC, g_width, g_height, g_unit_size);


	CString EMFname = _T("cactus_part.emf");
	HENHMETAFILE hMetaFile = GetEnhMetaFile(EMFname);
	int old_mode = pDC->SetGraphicsMode(GM_ADVANCED);
	if (hMetaFile) {
		// Save the current world transform
		XFORM oldTransform;
		pDC->GetWorldTransform(&oldTransform);

		// Define the angle for rotation
		float angle = 90.0f; // Adjust as necessary
		int x = 200, y = 200;
		XFORM translation = transforms::get_translation_matrix(x, y);
		XFORM rotation = transforms::get_rotational_matrix(angle);
		pDC->SetWorldTransform(&translation);
		pDC->ModifyWorldTransform(&rotation, MWT_LEFTMULTIPLY);
		
		// Step 2: Play the metafile at the origin (0, 0)
		pDC->PlayMetaFile(hMetaFile, CRect(0, 0, 200, 200)); // Play the metafile relative to the origin

		// Step 3: Restore the old world transform
		pDC->SetWorldTransform(&oldTransform);

		// Clean up
		DeleteEnhMetaFile(hMetaFile);
	}
	else {
		AfxMessageBox(_T("Failed to load the Enhanced Metafile."));
	}
	old_mode = pDC->SetGraphicsMode(old_mode);
	


	// TODO: add draw code for native data here
}


// CIND18623View printing

BOOL CIND18623View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18623View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18623View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

BOOL CIND18623View::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) // Check for key down message
	{
		switch (pMsg->wParam) { // Check the key code
		case 'G': // Check if the 'G' key is pressed
			// Handle 'G' key press here
			do_grid_draw = !do_grid_draw;
			Invalidate();
			return TRUE; // Return TRUE to indicate it was handled
		}
	}
	return FALSE;

}


// CIND18623View diagnostics

#ifdef _DEBUG
void CIND18623View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18623View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18623Doc* CIND18623View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18623Doc)));
	return (CIND18623Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18623View message handlers