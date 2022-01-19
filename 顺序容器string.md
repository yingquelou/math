构造string的其他方法|解释说明|
|:-:|:-|
string s(cp，n)		|	s是cp指向的数组中前n个字符的拷贝。此数组至少应该包含n个字符
string s(s2,pos2)		|s是string s2从下标pos2开始的字符的拷贝。若pos2>s2.size(),构造函数的行为未定义
string s(s2,pos2,len2)	|s是string s2从下标pos2开始len2个字符的拷贝。若pos2>s2.size()构造函数的行为未定义。不管len2的值是多少，构造函数至多拷贝s2.size()\-pos2个字符
注:n、len2和pos2都是无符号值

---
修改string的操作|解释说明|
|:-:|:-|
s.insert(pos,args)		|在pos之前插入args指定的字符。pos可以是一个下标或一个迭代器。接受下标的版本返回一个指向s的引用;接受迭代器的版本返回指向第一个插入字符的迭代器
s.erase(pos,len)		|删除从位置pos开始的len个字符。如果len被省略，则删除从pos开始直至s末尾的所有字符。返回一个指向s的引用
s.assign(args)			|将s中的字符替换为args指定的字符。返回一个指向s的引用
s.append(args)		|	将args追加到s。返回一个指向s的引用
s.replace(range,args)|	删除s中范围range内的字符,替换为args指定的字符。range或者是一个下标和一个长度，或者是一对指向s的迭代器。返回一个指向s的引用
---
- args可以是下列形式之一;
- append和assign可以使用所有形式。 
- str不能与s相同，迭代器b和e不能指向s。

|args|解释说明|
|:-:|:-|
str			|	字符串str
str,pos,len	|	str中从pos开始最多len个字
cp,len			|从cp指向的字符数组的前(最多)len个字符
cp			|	cp指向的以空字符结尾的字符数组
n,c				|n个字符c
b,e			|	迭代器b和e指定的范围内的字符
初始化列表	|	花括号包围的，以逗号分隔的字符列表
---
replace和insert所允许的args形式依赖于range和pos是如何指定的。
args可以是|replace(pos,len,args)|replace(b,e,args)|insert(pos,args)|insert(iter,args)
|:-|:-:|:-:|:-:|:-:| 
str|是| 是| 是|否|  
str,pos,len|是 |否 |是|否| 
cp,len|是 |是 | 是 |否 |
cp|是 |是|否| 否 | 
n,c| 是|是|是|是|  
b2,e2|否| 是|否 | 是|
初始化列表|否| 是|否|是|
---
子字符串操作|解释说明|
|:-:|:-|
s.substr(pos,n) |返回一个string，包含s中从pos开始的n个字符的拷贝。pos的默认值为0。n的默认值为s.size()-pos，即拷贝从pos开始的所有字符
---
string搜索操作|解释说明|
|:-:|:-|
s.find(args) |查找s中args第一次出现的位置 
s.rfind(args) |查找s中args最后一次出现的位置 
s.find_first_of(args) |在s中查找args中任何一个字符第一次出现的位置 
s.find_last_of(args) |在s中查找args中任何一个字符最后一次出现的位置 
s.find_first_not_of(args) |在s中查找第一个不在args中的字符 
s.find_last_not_of(args) |在s中查找最后一个不在args中的字符
注:搜索操作返回指定字符出现的下标，如果未找到则返回nposo

---
- args必须是以下形式之一

|args|解释说明|
|:-:|:-|
c,pos |从s中位置pos开始查找字符c。pos默认为0 
s2,pos |从s中位置pos开始查找字符串s2。pos默认为0 
cp,pos |从s中位置pos开始查找指针cp指向的以空字符结尾的C风格字符串。pos默认为0
cp,pos,n |从s中位置pos开始查找指针cp指向的数组的前n个字符。pos和n无默认值
---
s.compare的几种参数形式|解释说明|
|:-:|:-|
s2 |比较s和s2 
pos1,n1,s2| 将s中从pos1开始的n1个字符与s2进行比较 
pos1,n1,s2,pos2,n2| 将s中从pos1开始的n1个字符与s2中从pos2开始的n2个字符进行比较
cp |比较s与cp指向的以空字符结尾的字符数组 
pos1,nl,cp |将s中从pos1开始的n1个字符与cp指向的以空字符结尾的字符数组进行比较
pos1,n1,cp,n2 |将s中从pos1开始的n1个字符与指针cp指向的地址开始的n2个字符进行比较
---
<table>
<tr>
    <th>string和数值之间的转换</th>
    <th>解释说明</th>
</tr>

<tr>
    <td align="center">to_string(val)</td>
    <td>一组重载函数，返回数值val的string表示。val可以是任何算术类型。对每个浮点类型和int或更大的整型，都有相应版本的tostring。与往常一样，小整型会被提升</td>
</tr>

<tr>
    <td align="center">stoi(s,p,b)</td>
    <td rowspan="5">返回s的起始子串(表示整数内容)的数值,返回值类型分别是int、long、unsigned long、long long、unsigned long long。b表示转换所用的基数，默认值为10。p是sizet指针，用来保存s中第一个非数值字符的下标，p默认为0，即，函数不保存下标</td>
</tr>
 
<tr><td align="center">stol(s,p,b)</td></tr>
<tr><td align="center">stoul(s,p,b)</td></tr>
<tr><td align="center">stoll(s,p,b)</td></tr>
<tr><td align="center">stoull(s,p,b)</td></tr>
  
<tr><td align="center">stof(s,p)</td><td rowspan="3">返回s的起始子串(表示浮点数内容)的数值，返回值类型分别是float、double或long double.参数p的作用与整数转换函数中一样</td></tr>
<tr><td align="center">stod(s,p)</td></tr>
<tr><td align="center">stold(s,p)</td></tr>

</table>

---