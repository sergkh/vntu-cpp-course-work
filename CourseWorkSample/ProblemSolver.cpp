#include "StdAfx.h"
#include "ProblemSolver.h"
#include "graphics.h"

using namespace graphics;

ProblemSolver::ProblemSolver(void) {
	figuresCnt = 0;
	figures = NULL;
	biggestTriangledIntersects = false;
	solved = false;
}

ProblemSolver::~ProblemSolver(void) {
	reset();
}


/*
 * Вирішує завдання № 47: У файлі задано координати вершин трикутників та чотирикутників.
 * Вивести їх на екран. Ті трикутники, периметр яких менший ніж половина
 * середнього значення периметра трьох найбільших за периметром
 * чотирикутників, виділити окремим кольором. Іншим кольором виділити
 * два прямокутних трикутники, що мають найменшу площу. Вивести
 * повідомлення, чи перетинається найбільший виділений трикутник з одним
 * із виділених чотирикутників.
*/
void ProblemSolver::readData(LPCTSTR fileName) {
	reset();

	// Формат файлу: кожна стрічка починається з к-сті точок
	// 3 або 4 для трикутника і чотирикутника далі йдуть координати
	// точок x y, наприклад один трикутник: 
	// 3 3 2 4 2 1 3

	// Обрахунок кількості фігур
	// більш оптимальним варіантом буде використання std::list або CList
	// але для того, щоб не ускладнювати програму можна обмежитись таким варіантом
	int maxFigures = countLines(fileName);

	std::ifstream file;
	file.open(fileName, std::ios::in);

	if (!file.is_open() || !maxFigures) {
		CString msg;
		msg.Format(_T("Cannot open input file: %s"), fileName);
		AfxMessageBox(msg, MB_OK | MB_APPLMODAL | MB_ICONERROR);
		return ;
	}
	
	figures = new AbstractAngledFigure*[maxFigures];
	// Ініціалізація масиву нулями (NULL) для уникнення помилок
	memset(figures, 0, sizeof(AbstractAngledFigure*) * maxFigures);

	while(!file.eof()) {

		int pointsCnt = 0;
		file >> pointsCnt;
		
		if(file.eof()) break;

		if(pointsCnt == 3) {
			int x[3], y[3];
			for(int i = 0; i < 3; i++) file >> x[i] >> y[i];

			figures[figuresCnt] = new Triangle(
				Point(x[0], y[0]),
				Point(x[1], y[1]),
				Point(x[2], y[2])
			);
		} else if (pointsCnt == 4) {
			int x[4], y[4];
			for(int i = 0; i < 4; i++) file >> x[i] >> y[i];

			figures[figuresCnt] = new Quadrangle(
				Point(x[0], y[0]),
				Point(x[1], y[1]),
				Point(x[2], y[2]),
				Point(x[3], y[3])
			);
		} else {
			CString msg;
			msg.Format(_T("Points count is wrong: %d. Only 3 or 4 is accepted."), pointsCnt);
			AfxMessageBox(msg, MB_OK | MB_APPLMODAL | MB_ICONERROR);

			break;
		}

		figuresCnt++;
	}

	file.close();
}

void ProblemSolver::solve() {
	const COLORREF colorBiggest = RGB(255,0,0);
	const COLORREF colorMatches = RGB(0,255,0);

	// Ініціалізація масиву нулями (NULL) для уникнення помилок
	double maxPer = 0;

	for(int i = 0; i < figuresCnt; i++) {
		if(figures[i]->getPointsCount() != 4) continue;

		double per = figures[i]->perimeter();
		if(maxPer < per) maxPer = per;
	}

	// Периметр трикутника має бути більшим
	// за половину периметра найбільшого чотирикутника
	double perThreshold = maxPer/2;

	// Потрібні щоб запам*ятати 2 трикутника з найбільшими периметрами
	int biggestIds[] = {-1, -1};
	double biggestTrianglePer[] = {0, 0};

	for(int i = 0; i < figuresCnt; i++) {
		if(figures[i]->getPointsCount() != 3) continue;
		
		double per = figures[i]->perimeter();
		
		if(per >= perThreshold) continue;
		
		figures[i]->setColor(colorMatches);
		
		if(biggestTrianglePer[0] <= per) {
			if(biggestTrianglePer[1] <= per) {
				biggestTrianglePer[0] = biggestTrianglePer[1];
				biggestTrianglePer[1] = per;
				biggestIds[0] = biggestIds[1];
				biggestIds[1] = i;
			} else {
				biggestTrianglePer[0] = per;
				biggestIds[0] = i;	
			} 
		}
	}

	// найбільші трикутники, що відповідають умові 
	if (biggestIds[0] > 0) figures[biggestIds[0]]->setColor(colorBiggest);
	if (biggestIds[1] > 0) figures[biggestIds[1]]->setColor(colorBiggest);

	// Перевірка перетину найбільших трикутників
	if(biggestIds[0] != -1 && biggestIds[1] != -1) {
		AbstractAngledFigure* t1 = figures[biggestIds[0]];
		AbstractAngledFigure* t2 = figures[biggestIds[1]];

		bool intersects = false;

		for(int i = 0; i < 3 && !intersects; i++) {
			for(int j = 0; j < 3 && !intersects; j++) {
				Segment side = t2->getSide(j);
				if(t1->getSide(i).intersects(side)) {
					intersects = true;
				}
			}
		}

		biggestTriangledIntersects = intersects;
	}

	solved = true;
}

void ProblemSolver::draw(GraphicsBridge& g) {
	if(!solved || !figures) return ;

	// достатньо просто делегувати відображення всім фігурам
	for(int i = 0; i < figuresCnt; i++) {
		if(figures[i]) figures[i]->draw(g);
	}
}

LPCTSTR ProblemSolver::getResultText() {
	return biggestTriangledIntersects ?
		_T("Найбільші трикутники, що задовольняють умові, перетинаються") : 
		_T("Найбільші трикутники, що задовольняють умові, не перетинаються");
}

void ProblemSolver::reset() {
	// Видалення кожної фігури окремо, оскільки вони були створені по new
	// а потім всього масиву
	
	solved = false;
	biggestTriangledIntersects = false;

	if(figures) {
		for(int i = 0; i < figuresCnt; i++) {
			if(figures[i]) delete figures[i];
		}
		
		delete[] figures;
		figures = NULL;
	}

	figuresCnt = 0;
}

int ProblemSolver::countLines(LPCTSTR fileName) {
	std::ifstream file;
	file.open(fileName, std::ios::in);
	if (!file.is_open()) return 0;

	int count = 0;
	char temp[255];
	while (file.getline(temp, 255)) count++;

	file.close();
	return count;
}
