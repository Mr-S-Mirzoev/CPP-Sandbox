#include <inttypes.h>
#include <random>
#include <ctime>
#include <fstream>
#include <iostream>

inline void writeln (const std::string &s, bool append = true) {
	std::ofstream lines;
	if (append)
		lines.open ("lines.txt", std::ios::out | std::ios::app);
	else
		lines.open ("lines.txt");
	if (lines.is_open()) {
		lines << s << std::endl;
		lines.close();
	} else 
		throw std::runtime_error("Unable to open file lines.txt");
}

class pt {
	double x_c, y_c;
public:
	pt (const double &a, const double &b): x_c(a), y_c(b) {}
	double x () const {return x_c;}
	double y () const {return y_c;}
	friend std::ostream& operator << (std::ostream& o, const pt &x);
};

class sign {
	int8_t sgn;
	const double EPS_POS = 0.0001;
	const double EPS_NEG = -0.0001;
public:
	sign (const double &val) {
		if (val <= EPS_POS && val >= EPS_NEG) {
			sgn = 0;
		} else if (val > EPS_POS) {
			sgn = 1;
		} else {
			sgn = -1;
		}
	}
	explicit sign (const char &sign) {
		switch (sign) {
		case '+': 
			sgn = 1;
			break;
		case '-':
			sgn = -1;
			break;
		default:
			sgn = 0;
		}
	}
	int8_t sn() const {
		return sgn;
	}
	sign operator*(const sign &rhs) const {
		switch (sgn * rhs.sgn) {
		case 0:
			return sign('0');
		case 1:
			return sign('+');
		default:
			return sign('-');
		}
	}
};
 
inline sign area_sign (const pt &a, const pt &b, const pt &c) {
	return sign((b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x()));
}
 
inline bool intersect_1 (double a, double b, double c, double d) {
	if (a > b)  std::swap (a, b);
	if (c > d)  std::swap (c, d);
	return std::max(a,c) <= std::min(b,d);
}
 
bool intersect (pt a, pt b, pt c, pt d) {
	return intersect_1 (a.x(), b.x(), c.x(), d.x())
		&& intersect_1 (a.y(), b.y(), c.y(), d.y())
		&& ((area_sign(a, b, c) * area_sign(a, b, d)).sn() <= 0)
		&& ((area_sign(c, d, a) * area_sign(c, d, b)).sn() <= 0);
}

inline double rand_val() {
	return double(std::rand() - RAND_MAX / 2) / RAND_MAX;
}

class segment {
	pt st, fn;
public:
	segment (const double &ba, 
			const double &bb, 
			const double &ea, 
			const double &eb): st(ba, bb), fn(ea, eb) {
		writeln(std::to_string(ba) + " " +
				std::to_string(bb) + " " +
				std::to_string(ea) + " " +
				std::to_string(eb));
	}
	pt begin () const {return st;}
	pt end () const {return fn;}
	friend std::ostream& operator << (std::ostream& o, const segment &x);
};

std::ostream& operator << (std::ostream& o, const segment &x) {
	o << "Segment is:\na: " << x.st << "\nb: " << x.fn << std::endl;
	return o;
}

std::ostream& operator << (std::ostream& o, const pt &x) {
	o << "(" << x.x_c << ", " << x.y_c << ")";
	return o;
}

int main () {
	srand(time(NULL));
	try {
		writeln("##x1 y1 x2 y2", false);
		segment a(rand_val(), rand_val(), rand_val(), rand_val());
		segment b(rand_val(), rand_val(), rand_val(), rand_val());
		std::cout << a << b;
		if (intersect(a.begin(), a.end(), b.begin(), b.end())) {
			std::cout << "Segments intersect!" << std::endl; 
		} else {
			std::cout << "Segments don't instersect!" << std::endl; 
		}
		system("gnuplot < plot.cmd");
		system("open result.png");
	} catch (std::runtime_error &x) {
		std::cout << "Runtime Error occured: " << x.what() << std::endl;
	}
	return 0;
}