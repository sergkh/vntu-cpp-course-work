#pragma once
#include "graphics.h"

using namespace graphics;

class ProblemSolver
{
public:
	ProblemSolver();
	void readData(LPCTSTR file);
	void solve();
	virtual ~ProblemSolver();
	void draw(GraphicsBridge& g);
	LPCTSTR getResultText();
private:
	int figuresCnt;
	AbstractAngledFigure** figures;
	boolean biggestTriangledIntersects;
	boolean solved;
	void reset();
	int countLines(LPCTSTR file);
};
