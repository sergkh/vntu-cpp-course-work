#pragma once
#include "graphics.h"

// Реалізація функцій малювання для CPaintDC
class CDCGraphicsBridge: public graphics::GraphicsBridge
{
public:
	CDCGraphicsBridge(CPaintDC& dc);
	~CDCGraphicsBridge(void);

	virtual void line(int xs, int ys, int xe, int ye, COLORREF color);
	virtual void circle(int x, int y, int r, COLORREF color);

private:
	CPaintDC& dc;
};
