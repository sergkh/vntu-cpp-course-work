#pragma once

// http://ru.wikipedia.org/wiki/Sgn
#define sign(a) ((a)>0? 1 : (a)<0 ? -1 : 0)

// область імен дозволяє уникнути
// конфліктів в іменах класів з іншими бібліотеками
namespace graphics 
{

class Point {
	int x, y;

public:
	Point(int x, int y);	
	Point(const Point& p);
	inline int getX() const { return x; }
	inline int getY() const { return y; }

	// обчислення відстані до іншої точки
	// http://www.math.by/geometry/distptp.html
	double dist(const Point &p) const;

	int operator == (const Point& that);
};

// Інтерфейс, що дозволяє фігурі відобразити себе.
// Використовується щоб відокремити бібліотеку від конкретної 
// реалізації графічної системи - це дозволить вільно використовувати її
// як з MFC так і з іншими бібліотеками. 
class GraphicsBridge {
public:
	virtual void line(int xs, int ys, int xe, int ye, COLORREF color) = 0;
	virtual void circle(int x, int y, int r, COLORREF color) = 0;
};

// Інтерфейс для будь-якої фігури (від відрізка й багатокутника до кола)
class Figure {
public:
	// обчислення периметра фігури (або довжини для кола)
	virtual double perimeter() = 0;
	
	// малює фігуру використовуючи методи предоставлені
	// інтерфейсом GraphicsBridge
	virtual void draw(GraphicsBridge& g) = 0;
	
	// задає колір фігури
	virtual void setColor(COLORREF color) = 0;
	
	// повертає колір фігури
	virtual COLORREF getColor() = 0;

	// деструктор для батьківських класів завжди має бути віртуальним,
	// інакше операція delete pFigure; не викличе деструктор нащадка
	virtual ~Figure();
};

class Segment;

// Абстрактний клас, що містить методи спільні для 
// всіх N-кутників та відрізка
class AbstractAngledFigure : public Figure {
protected:
	Point* points;
	int pointsCount;
	COLORREF color;
public:
	AbstractAngledFigure(int pointsCount);
	virtual ~AbstractAngledFigure();

	Point& getPoint(int index);

	// повертає точку що передує вказаній,
	// для першої точки (індекс=0) повертає останню точку
	Point& getPrevPoint(int index);

	// повертає точку що слідує за вказаною,
	// для останньої точки повертає першу
	Point& getNextPoint(int index);

	// Повернути сторону багатокутника - 0 перша сторона
	Segment getSide(int i);

	// Обчислення периметра як суми відстаней між усіма точками
	double perimeter();

	// Повертає кількість точок багатокутника
	int getPointsCount();

	// малювання фігури, є однаковим для 
	// всіх багатокутників
	virtual void draw(GraphicsBridge& g);

	// Встановлення кольору фігури
	virtual void setColor(COLORREF color);

	// Повернення кольору фігури
	virtual COLORREF getColor();
};

// Відрізок. Можна розглядати, як вироджений варіант N-кутника
class Segment : public AbstractAngledFigure {
public:
	Segment(const Point& p1, const Point& p2);

	// методи, що повертають параметри рівняння прямої (Ax + By + C = 0), 
	// на якій лежить відрізок 
	// http://ru.wikipedia.org/wiki/Прямая (секція "Уравнение прямой, проходящей 
	// через две заданные несовпадающие точки")
	double getA();
	double getB();
	double getC();

	// Результат підстановки вказаної точки у рівняння прямої
	// дозволяє визначити положення її відносно даного відрізка
	// <0 або >0 - точка знаходиться по той чи інший бік прямої
	// == 0 точка лежить на прямій  
	double getRelLocation(Point& p);

	// Перевірка перетину відрізка з вказаним
	// Повертає true, якщо відрізки перетинаються, і false - якщо ні
	bool intersects(Segment& s);

	// Визначає Кут між відрізками
	double angle(Segment& s);
};
 

class Triangle : public AbstractAngledFigure {
public:
	Triangle(const Point& p1, const Point& p2, const Point& p3);
};

class Quadrangle : public AbstractAngledFigure {

public:
	Quadrangle(const Point& p1, const Point& p2, const Point& p3, const Point& p4);
};

} // end of namespace