#include<iostream>
//常用标准库顺序容器类型
#include<vector>		//可变大小数组。支持快速随机访问。在尾部之外的位置插入/删除元素可能很慢
#include<queue>			//双端队列。支持快速随机访问。在头尾位置插入、删除速度很快
#include<list>			//双向链表。只支持双向顺序访问。在list中任何位置进行插入/删除操作速度都很快
#include<forward_list>	//单向链表。只支持单向顺序访问。在链表任何位置进行插入/删除操作速度都很快
#include<array>			//固定大小数组。支持快速随机访问。不能添加/删除元素
#include<string>		//字符串容器。与vector相似的容器，但专门用于保存字符。随机访问快。在尾部插入、删除速度快
//顺序容器的通用操作
/*
类型别名
iterator		此容器类型的迭代器类型 
const_iterator	可以读取元素，但不能修改元素的迭代器类型 
size_type		无符号整数类型，足够保存此种容器类型最大可能容器的大小
difference_type 带符号整数类型，足够保存两个迭代器之间的距离 
value_type		元素类型 
reference		元素的左值类型;与value_type&含义相同 
const_reference 元素的const左值类型(即const valuetype&) 

构造函数
C c;			默认构造函数，构造空容器
C cl(c2);		构造c2的拷贝c1 
C c(b，e);		构造c，将迭代器b和e指定的范围内的元素拷贝到c(array不支持)
C c{a,bc...};	列表初始化c 

赋值与swap
c1=c2			将cl中的元素替换为c2中元素,c1 和 c2 必须具有相同的类型
cl={a,b,c...}	将cl中的元素替换为列表中元素(不适用于array) 
a.swap(b)		交换a和b的元素a和b必须具有相同的类型。swap通常比从a向b拷贝元素快得多
swap(a，b)		与a.swap(b)等价

assign操作不适用于关联容器和array
seq.assign(b,e)	将seq中的元素替换为迭代器b和e所表示的范围中的元素。迭代器b和e不能指向seq中的元素
seq.assign(il)	将seq中的元素替换为初始化列表il中的元素
seq.assign(n,t)	将seq中的元素替换为n个值为t的元素
注：赋值相关运算会导致指向左边容器内部的迭代器、引用和指针失效。而swap 操作将容器内容交换不会导致指向容器的迭代器、引用和指针失效(容器类型为 array 和 string 的情况除外)。

大小
c.size()	c中元素的数目(不支持forward list) 
cmax size() c可保存的最大元素数目 
c.empty()	若c中存储了元素，返回false，否则返回true 

添加/删除元素(不适用于array)
注:在不同容器中，这些操作的接口都不同
c.insert(args)	将args中的元素拷贝进c 
cemplace(inits) 使用inits构造c中的一个元素 
c.erase(args)	删除args指定的元素 
c.clear()		删除c中的所有元素，返回void 

关系运算符
==，!=			所有容器都支持相等(不等)运算符 
<，<=，>，>=	关系运算符(无序关联容器不支持) 

获取迭代器
c.begin(),c.end()	返回指向c的首元素和尾元素之后位置的迭代器 
c.cbegin()c.cend()	返回const iterator

反向容器的额外成员(不支持forwardlist)
reverse_iterator		按逆序寻址元素的迭代器
const_reverse_iterator	不能修改元素的逆序迭代器
c.rbegin(),crend()		返回指向c的尾元素和首元素之前位置的迭代器
c.crbegin(),c.crend()	返回const_reverseiterator
*/
/*向顺序容器添加元素的操作
这些操作会改变容器的大小;array不支持这些操作。
forward_list	有自己专有版本的insert和emplace; forward_list不支持push_back和emplace_back。
vector和string	不支持push_front和emplace_front。
cpush back(t)	在c的尾部创建一个值为t或由args创建的元素。返回void
c.emplace back(args)
cpush front(t)	在c的头部创建一个值为t或由args创建的元素。返回void
cemplace front(args)
c.insert(p，t)	在迭代器p指向的元素之前创建一个值为t或由args创建的元素。返回指向新添加的元素的迭代器
c.emplace(p,args) 
c.insert(p,n,t)	在迭代器p指向的元素之前插入n个值为t的元素。返回指向新添加的第一个元素的迭代器:若n为0，则返回p
c.insert(p,b,e) 将迭代器b和e指定的范围内的元素插入到迭代器p指向的元素之前。b和e不能指向c中的元素。返回指向新添加的第一个元素的迭代器;若范围为空，则返回p
cinsert(p,il)	i1是一个花括号包围的元素值列表。将这些给定值插入到迭代器p指向的元素之前。返回指向新添加的第一个元素的迭代器:若列表为空，则返回p
注:向一个vector string或deque插入元素会使所有指向容器的选代器、引用和指针失效。
*/
/*
在顺序容器中访问元素的操作
at和下标操作只适用于string、vector、deque和arrayo back不适用于forward listo
c.back()	返回c中尾元素的引用。若c为空，函数行为未定义
c.front()	返回c中首元素的引用。若c为空，函数行为未定义
c[n]		返回c中下标为n的元素的引用n是一个无符号整数。若n>=csize()，则函数行为未定义
c.at(n)		返回下标为n的元素的引用。如果下标越界，则抛出一out_of_range异常
注:对一个空容器调用front和back，就像使用一个越界的下标一样，是一种严重的程序设计错误。
*/
/*
顺序容器的删除操作
这些操作会改变容器的大小，所以不适用于arrayo
forward_list有特殊版本的erase
forward_list不支持pop_back;vector和string不支持pop_front。
c.pop_back()	删除c中尾元素。若c为空，则函数行为未定义。函数返回void
c.pop_front()	删除c中首元素。若c为空，则函数行为未定义。承数返回void
c.erase(p)		删除迭代器p所指定的元素，返回一个指向被删元素之后元素的迭代器，若p指向尾元素，则返回尾后(off-the-end)迭代器。若p是尾后迭代器，则函数行为未定义
c.erase(b,e)		删除迭代器b和e所指定范围内的元素。返回一个指向最后一个被删元素之后元素的迭代器，若e本身就是尾后迭代器，则函数也返回尾后迭代器
c.clear()		删除c中的所有元素。返回void
注:删除deque中除首尾位置之外的任何元素都会使所有迭代器、引用和指针失效。指向vector或string中删除点之后位置的迭代器、引用和指针都会失效。
*/
/*
在forward_list中插入或删除元素的操作
lst.before_begin()		返回指向链表首元素之前不存在的元素的迭代器。此迭代器不能解引用。
lst.cbefore_begin()		c.before_begin()返回一个const_iterator
lst.insert_after(p,t)	在迭代器p之后的位置插入元素。t是一个对象，n是数量，b和e是表示范围的一对迭代器(b和e不能指向lst内)
lst.insert_after(p,n,t) 
lst_insert_after(p,b,e) 
lst.insert_after(p,il)	il是一个花括号列表。返回一个指向最后一个插入元素的迭代器。如果范围为空，则返回p。若p为尾后迭代器，则函数行为未定义
emplace_after(p,args)	使用args在p指定的位置之后创建一个元素。返回一个指向这个新元素的迭代器。若p为尾后迭代器，则函数行为未定义
lst.erase_after(p)		删除p指向的位置之后的元素，或删除从b之后直到(但不包含)e之间的元素。返回一个指向被删元素之后元素的迭代
器，若不存在这样的元素，则返回尾后迭代器。如果p指向 lst的尾元素或者是一个尾后迭代器，则函数行为未定义
lsteraseafter(b,e) 
*/
/*
容器大小管理操作
shrink_to_fit		只适用于vector、string和deque。
capacity和reserve	只适用于vector和string。
c.shrink_to_fit()	请将capacity()减少为与size()相同大小
c.capacity()		不重新分配内存空间的话，c可以保存多少元素
c.reserve(n)		分配至少能容纳n个元素的内存空间
*/
using namespace std;
int main()
{
	vector<int> v1 = { 1, 3, 5, 7, 9, 12 };
	vector<int> v2 = { 1, 3, 9 };
	vector<int> v3 = { 1, 3, 5, 7 };
	vector<int> v4 = { 1, 3, 5, 7, 9, 12 };

	//	容器的比较——关系运算符
	cout << (v1 < v2)<<endl;	// true		v1和v2 在元素[2]处不同：v1[2]小于等于v2[2]
	v1 < v3;	// false	所有元素都相等，但v3 中元素数目更少
	v1 == v4;	// true		每个元素都相等，且v1 和 v4 大小相同
	v1 == v2;	// false	v2 元素数目比v1少

	cout<<v2.capacity();
	return 0;
}