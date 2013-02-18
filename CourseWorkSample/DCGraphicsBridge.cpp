#include "StdAfx.h"
#include "DCGraphicsBridge.h"

CDCGraphicsBridge::CDCGraphicsBridge(CPaintDC& pdc) : dc(pdc) {
}

CDCGraphicsBridge::~CDCGraphicsBridge(void) {
}

void CDCGraphicsBridge::line(int xs, int ys, int xe, int ye, COLORREF color) {
	dc.SetDCPenColor(color);
	dc.MoveTo(xs, ys);
	dc.LineTo(xe, ye);
}

void CDCGraphicsBridge::circle(int x, int y, int r, COLORREF color) {
	dc.SetDCPenColor(color);
	dc.Ellipse(x-r, y-r, x+r, y+r);
}