#include <iostream>
#include <string>

namespace Y {
	class A {
		std::string s;
		int x;
		int y;
		void f() { std::cout << "whoops" << std::endl; }
	public:
		void print_xy() {
			std::cout << x << ' ' << y << ' ' << s << '\n';
		}
	};
}

template<int>
struct num {};

template<class T, class C>
using PMember = T(C::*);

template<class T, class C, int N>
T& hijack(C&, num<N>);

template<class T, class C>
struct Wrapper {
	template <PMember<T, C> pf, int N>
	struct Hijack {
		template<class Ts, class Cs>
		friend Ts& hijack(Cs& s, num<N>) {
			return s.*pf;
		}
	};
};


template struct Wrapper<int, Y::A>::Hijack<&Y::A::x, 0>;
template struct Wrapper<int, Y::A>::Hijack<&Y::A::y, 1>;
template struct Wrapper<std::string, Y::A>::Hijack<&Y::A::s, 2>;


int main() {
	Y::A a;;
	int& x = hijack<int>(a, num<0>{});
	x = 10;
	int& y = hijack<int>(a, num<1>{});
	y = 11;
	auto& s = hijack<std::string>(a, num<2>{});
	s = "AYY";
	a.print_xy();
}
