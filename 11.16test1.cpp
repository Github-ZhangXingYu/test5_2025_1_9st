//11.17同花顺面试题，头文件是复制的时候没了
#include<bits/stdc++.h>
#include
#include
#include

class Base
{
public:
    virtual ~Base()
    {
        printf("it is ~Base()\n");
    }

    virtual void test() = 0;
};

class Derived : public Base
{
public:
    void test()
    {
        printf("it is test()\n");
    }
};

class Test
{
public:
    Test()
    {
        m_value  = 10;
        m_pvalue = new int(5);
        m_pt     = std::make_shared();
    }

    ~Test()
    {
        delete m_pvalue;
    }

    void test()
    {
        printf("Test test() (%d)(%d)\n", m_value, *m_pvalue);
        m_t.test();
        m_pt->test();
    }

protected:
    int                   m_value{5};
    int*                  m_pvalue{nullptr};
    Derived               m_t;
    std::shared_ptr m_pt{nullptr};
} ;

int main(int ac, char* av[])
{
    auto ptest = std::make_shared();
    auto fns   = std::bind(&Test::test, ptest.get());
    ptest      = nullptr;
    fns();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}