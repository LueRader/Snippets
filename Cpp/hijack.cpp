#include <iostream>
#include <vector>
#include <memory>


class A {
public:
    void print() {
        std::cout << x << ' ' << y << ' ' << s << '\n';
    }
private:
    int x, y;
    std::string s;
  void f(int) { std::cout << "whoops" << std::endl; }
};


template<class T, class C>
using Mem = T (C::*);

class any {
    struct base {
        virtual ~base() {}
    };

    template<class T, class C>
    struct derived : public base {
        Mem<T,C> m;
        derived(Mem<T,C> mem) : m{mem} {}
        T& get(C& c) {
            return c.*m;
        }
    };
    
public:
    template<class T, class C>
    T& get(C&& c) {
        return static_cast<derived<T,typename std::remove_reference<C>::type>*>(holder_.get())
            ->get(std::forward<C>(c));
    }
    
    template<class T, class C>
    any(Mem<T,C> mem) : holder_{new derived<T, C>{mem}} {}


    std::shared_ptr<base> holder_;

};

std::vector<any> hijack();

template<class... Ts>
struct Wrapper {
    template <Ts... pf>
    struct Hijack {
        friend std::vector<any> hijack() {
            return std::vector<any>{any{pf}...};
        }
    };
};

template struct Wrapper<Mem<int,A>, Mem<int,A>, Mem<std::string,A>>::Hijack<&A::x, &A::y, &A::s>;

int main() {
    A a;
    auto arr = std::move(hijack());
    arr[0].get<int>(a) = 10;
    arr[1].get<int>(a) = 20;
    arr[2].get<std::string>(a) = "Ayyy";
    a.print();
}
