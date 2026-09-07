#include "List.h"

void test01()
{
    // 目标：实现基础功能
    mylist::list<int> lt;
    lt.push_back(1);
    lt.push_back(3);
    lt.push_back(7);
    lt.push_back(4);
    lt.push_back(5);
    lt.push_back(2);

    lt.pop_back();
    lt.pop_back();
    lt.pop_back();

    // 迭代器
    mylist::list<int>::iterator it = lt.begin();
    while(it != lt.end())
    {
        std::cout << *it << " ";
        it++;
    }
    std::cout << std::endl;

    std::cout << lt.size() << std::endl;
}

void test02()
{
    mylist::list<int> lt;
    lt.push_back(1);
    lt.push_back(3);
    lt.push_back(7);
    lt.push_back(4);
    lt.push_back(5);
    lt.push_back(2);

    // 迭代器
    mylist::list<int>::const_iterator ctit = lt.begin();
    while(ctit != lt.end())
    {
        // *ctit += 10;
        std::cout << *ctit << " ";
        ctit++;
    }
    std::cout << std::endl;

    std::cout << lt.size() << std::endl;
}

void print(const mylist::list<int>& lt)
{
	// const迭代器的要求不是本身不能修改，而是指向的内容不能修改
	//const list<int>::iterator it = lt.begin();
	mylist::list<int>::const_iterator it = lt.begin();
	while (it != lt.end())
	{
		//*it = 10;
		std::cout << *it << " ";
		++it;
	}
	std::cout << std::endl;
}

void list_test04()
{
	mylist::list<int> lt;

	lt.push_back(1);
	lt.push_back(2);
	lt.push_back(3);
	lt.push_back(4);
	lt.push_back(5);

	print(lt);
}

int main()
{
    // test01();
    // test02();
    list_test04();

    return 0;
}

