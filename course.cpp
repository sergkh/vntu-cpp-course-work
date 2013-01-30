#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>

// http://ru.wikipedia.org/wiki/Sgn
#define sign(a) ((a)>0? 1 : (a)<0 ? -1 : 0)

using namespace std;

// область імен дозволяє уникнути
// конфліктів в іменах класів з іншими бібліотеками
namespace graphics 
{

class Point {
  int x,y;

public:
	Point() { 
		x = 0; 
		y = 0; 
	}

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}	

	Point(const Point& p) {
		x = p.x;
		y = p.y;
	}

	int getX() const { return x; }
	int getY() const { return y; }

	// обчислення відстані до іншої точки
	// http://www.math.by/geometry/distptp.html
	double dist(const Point &p) const {
		double dx = x - p.x;
		double dy = y - p.y;
		return sqrt(dx*dx + dy*dy);
	}

	int operator == (const Point& that) {
    	return x == that.x && y == that.y;
	}
};

// Інтерфейс для будь-якої фігури (від відрізка й багатокутника до кола)
class Figure {
public:
	// обчислення периметра фігури (або довжини для кола)
	virtual double perimeter() = 0;
	
	// малює фігуру заданим кольором
	virtual void draw(int color) = 0;
	
	// деструктор для батьківських класів завжди має бути віртуальним,
	// інакше операція delete pFigure; не викличе деструктор нащадка
	virtual ~Figure(){}
};

// Абстрактний клас, що містить методи спільні для 
// всіх N-кутників та відрізка
class AbstractAngledFigure : public Figure {

protected:
	Point* points;
	int pointsCount;

public:
	AbstractAngledFigure(int pointsCount) {
		if(pointsCount <= 0) {
			throw "Illegal points count";
		}

		this->points = new Point[pointsCount];
		this->pointsCount = pointsCount;
	}

	virtual ~AbstractAngledFigure() {
		if(points) {
			delete points;
			points = NULL;
		}
	}

	Point& getPoint(int index) {
		return points[index];
	}

	// повертає точку що передує вказаній,
	// для першої точки (індекс=0) повертає останню точку
	Point& getPrevPoint(int index) {
		return index == 0 ? points[pointsCount-1] : points[index - 1];
	}

	// повертає точку що слідує за вказаною,
	// для останньої точки повертає першу
	Point& getNextPoint(int index) {
		return index == pointsCount-1 ? points[0] : points[index + 1];
	}

	// Обчислення периметра як суми відстаней між усіма точками
	double perimeter() {
		// вироджений випадок для відрізка
		if(pointsCount == 2) {
			return getPoint(0).dist(getNextPoint(0)); 	
		}

		double per = 0;
		for(int i = 0; i < pointsCount; i++) {
			per += getPoint(i).dist(getNextPoint(i));
		}

		return per;
	}

	// в данному випадку замість малювання дані про фігуру 
	// виводяться у поток виведення
	virtual void draw(int color){ 
		cout<<"Drawing figure: ";
		for(int i = 0; i < pointsCount; i++) {
			cout<<"("<<points[i].getX()<<","<<points[i].getY()<<") ";
		}

		cout<<"with color: "<<color<<", perimeter = "<<perimeter()<<endl;
	}
};

// Відрізок. Можна розглядати, як вироджений варіант N-кутника
class Segment : public AbstractAngledFigure {
public:
	Segment(const Point& p1, const Point& p2) : AbstractAngledFigure(2) {
		points[0] = p1;
		points[1] = p2;
	}

	// методи, що повертають параметри рівняння прямої (Ax + By + C = 0), 
	// на якій лежить відрізок 
	// http://ru.wikipedia.org/wiki/Прямая (секція "Уравнение прямой, проходящей 
	// через две заданные несовпадающие точки")
	double getA() {
		return points[0].getY() - points[1].getY();
	}

	double getB() {
		return points[1].getX() - points[0].getX();
	}

	double getC() {
		return (double)points[0].getX()*points[1].getY() - 
			(double)points[1].getX()*points[0].getY();
	}

	// Результат підстановки вказаної точки у рівняння прямої
	// дозволяє визначити положення її відносно даного відрізка
	// <0 або >0 - точка знаходиться по той чи інший бік прямої
	// == 0 точка лежить на прямій  
	double getRelLocation(Point& p) {
		return getA()*p.getX() + getB()*p.getY() + getC();
	}

	// Перевірка перетину відрізка з вказаним
	// Повертає true, якщо відрізки перетинаються, і false - якщо ні
	bool intersects(Segment& s) {
		// Перевіряється положення кінців переданого відрізка
		// відносно поточного і навпаки 
		double sp0 = getRelLocation(s.getPoint(0));
		double sp1 = getRelLocation(s.getPoint(1));

		double p0 = s.getRelLocation(getPoint(0));
		double p1 = s.getRelLocation(getPoint(1));

		return (sign(sp0) != sign(sp1) && 
				sign(p0) != sign(p1));
	}

	// Визначає Кут між відрізками
	double angle(Segment& s) {
		// angle between two straights equals angle beetwen
		// direction vectors (-B1;A1) and (-B2;A2)

		double A1 = getA();
		double A2 = s.getA();
		double B1 = getB();
		double B2 = s.getB();

		double num = B1*B2 + A1*A2;
		double denum = sqrt(B1*B1 + A1*A1) * sqrt(B2*B2 + A2*A2);

		double val = num/denum;
		
		// precision fix - sometimes value becаme slightly > 1 (e.g. 1.00000001)
		if(val > 1 || val < 1) val = round(val);

		return acos(val);
	}
};

// Трикутник
class Triangle : public AbstractAngledFigure {

public:
	Triangle(const Point& p1, const Point& p2, const Point& p3) : AbstractAngledFigure(3) {
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
	}

	// Повернути сторону трикутника (0-3)
	Segment getSide(int i) {
		return Segment(points[i], getNextPoint(i));
	}
};

// Чотирикутник
class Quadrangle : public AbstractAngledFigure {

public:
	Quadrangle(const Point& p1, const Point& p2, const Point& p3, const Point& p4) : AbstractAngledFigure(4) {
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
		points[3] = p4;
	}

};

}

using namespace graphics;

// Перевіряє чи містить масив вказане значення
bool containsVal(int array[], int size, int value) {
	for(int i = 0; i < size; i++) if(array[i] == value) return true;
	return false;
}

// Основні обчислення по задачі
void checkPerimeters(Triangle* triangles[], 
					 int trianglesCnt, 
					 Quadrangle* quadrangles[], 
					 int quadranglesCnt) {

	// Знаходимо найбільший периметр чотирикутника
	double maxPer = quadrangles[0]->perimeter();

	for(int i = 1; i < quadranglesCnt; i++) {
		double per = quadrangles[i]->perimeter();
		if(maxPer < per) maxPer = per;
	}

	// Периметри трикутників мають бути більші
	// за половину периметра найбільшого чотирикутника
	double perThreshold = maxPer/2;

	// масив що зберігатиме трикутники з більшим периметром
	// (мають бути намальовані іншим кольором)
	int* colored = new int[trianglesCnt];
	int coloredCnt = 0;

	// Потрібні щоб запам*ятати 2 трикутника з найбільшими периметрами
	int biggestIds[] = {-1, -1};
	double biggestTrianglePer[] = {0, 0};

	for(int i = 0; i < trianglesCnt; i++) {
		double per = triangles[i]->perimeter();
		if(per < perThreshold) {
			colored[coloredCnt++] = i;
			
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
	}

	// малювання всіх фігур
	// Колір 1 - чотирикутники, 2 - 2 найбільші трикутники, 
	// 3 - трикутники, що відповідають умові
	// 4 - трикутники, що не відповідають умові
	for(int i = 0; i < quadranglesCnt; i++) {
		 quadrangles[i]->draw(1);
	}

	for(int i = 0; i < trianglesCnt; i++) {
		int color = containsVal(biggestIds, 2, i) ? 2 : containsVal(colored, coloredCnt, i) ? 3 : 4; 
		triangles[i]->draw(color);
	}

	// Перевірка перетину найбільших трикутників
	if(biggestIds[0] != -1 && biggestIds[1] != -1) {
		Triangle* t1 = triangles[biggestIds[0]];
		Triangle* t2 = triangles[biggestIds[1]];

		bool intersects = false;

		for(int i = 0; i < 3 && !intersects; i++) {
			for(int j = 0; j < 3 && !intersects; j++) {
				Segment side = t2->getSide(j);
				if(t1->getSide(i).intersects(side)) {
					intersects = true;
				}
			}
		}

		if(intersects) cout<<"Biggest triangles are intersecting\n";
	}

	delete colored;
}


int main() {
	char fileName[] = "test.txt";
	
	int trianglesCnt = 0, quadranglesCnt = 0;
	Triangle* triangles[20];
	Quadrangle* quadrangles[20]; 

	// Формат файлу: кожна стрічка починається з к-сті точок
	// 3 або 4 для трикутника і чотирикутника далі йдуть координати
	// точок x y, наприклад один трикутник: 
	// 3 3 2 4 2 1 3

	FILE *file = NULL;
	if ((file = fopen(fileName, "rt")) == NULL){
		printf("Cannot open input file: %s \n", fileName);
		return 1;
	}

	while(!feof(file)) {
		int pCnt = 0;
		int p[4][2];
		fscanf(file, "%d", &pCnt);

		for(int i = 0; i < pCnt; i++) {
			fscanf(file, "%d %d", &p[i][0], &p[i][1]);
		}

		if(pCnt == 3) {
			// triangle
			triangles[trianglesCnt++] = new Triangle(
					Point(p[0][0], p[0][1]), 
					Point(p[1][0], p[1][1]), 
					Point(p[2][0], p[2][1])
				);
		} else if(pCnt == 4) {
			// quadrangle
			quadrangles[quadranglesCnt++] = new Quadrangle(
				Point(p[0][0], p[0][1]), 
				Point(p[1][0], p[1][1]), 
				Point(p[2][0], p[2][1]), 
				Point(p[3][0], p[3][1]) 
			);
		}

	}
	
	fclose(file);
	
	checkPerimeters(triangles, trianglesCnt, quadrangles, quadranglesCnt);

	// delete 
	for(int i = 0; i < trianglesCnt; i++) {
		delete triangles[i];
	}

	for(int i = 0; i < quadranglesCnt; i++) {
		delete quadrangles[i];
	}

	return 0;
}


/*
Зчитування через потокове введення

ifstream myfile;
	myfile.open ("points.txt", ios::in);

if (!myfile.is_open()) {
	cerr<<"File points.txt is not found!"<<endl;
}

while (!myfile.eof()) {
	int x, y;
	myfile>>x>>y;
	cout<<"x="<<x<<", y="<<y<<endl;
}
myfile.close();
*/

