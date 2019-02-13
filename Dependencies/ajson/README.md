ajson
=====
0.2.0版本要求C++11的编译器

a utility for serialize C++ and json.

动机：
=====
为什么要设计ajson？

在工作中经常有这样的需求，获得一段json编码的文本，将它解析为dom对象（比如rapidjson）。

然后读取各节点的值，为了方便，经常将其保存到某个数据结构。

然后这个dom就不需要了，这里dom的解析和构建就感觉到了浪费。

也有反过来的情况，将一个C++结构体，序列化到dom，然后将dom再次序列化到文本。

再次，感觉到了dom的浪费，能跳过dom这层吗？

这时，我希望能有一个像javascript那样方便的将json文本到数据结构直接方便的转换。

于是ajson就诞生了。

简介
=====
```cpp
------------------------------------------------------------------

struct Person
{

  std::string  Name;
  
  int          Age;
  
};

AJSON(Person , Name , Age)

Person obj;

char * json= "{	\"Name\" : \"Boo\",	\"Age\" : 28}";

ajson::load_from_buff(obj,json);

```

以上代码解释了什么是AJSON,AJSON还支持stl的顺序容器。

再来个复杂点的例子

```cpp
enum PhoneType

{

  MOBILE = 0,

  HOME = 1,

  WORK = 2

};

struct PhoneNumber

{
  std::string number;

  PhoneType   type = MOBILE;

  PhoneNumber(std::string const& number_  = "", PhoneType type_ = MOBILE)

    :number(number_),type(type_)

  {}

};

struct Person

{

  std::string name;

  int32_t     id;

  std::string mail;

  std::vector<PhoneNumber> phones;

  Person(std::string name_  = "" , int32_t id_  = 0, std::string mail_ = "")

    :name(name_),id(id_),mail(mail_)

  {}

};

struct AddressBook

{

  std::vector<Person> peoples;

};

AJSON(PhoneNumber, number, type)

AJSON(Person, name, id, mail, phones)

AJSON(AddressBook, peoples)

```
结构体支持嵌套，stl容器也支持嵌套。

--------------------------------------------------------
特点
======
ajson反序列化扫描的时候，直接将字面量类型同时解析，并将结果直接存入对应的数据结构字段。

如此一来就不需要临时的DOM，减少了中间处理的工作以及内存的申请释放，大大提升了性能。

使用方式简单，只要定义相应的宏AJSON，便可方便的序列化/反序列化操作。

依赖小，完全不依赖第三方库，只有一个头文件，省去了编译的麻烦。

要点
======
如果json内部附带了转义符，ajson会改动原始文本来避免新申请内存。

如果不希望原始文本被修改，请创建一份新的拷贝。

由于ajson全是内存操作，文件读取操作时，将会全部读入内存，所以不适合大文件操作。

其他
======

为什么要选择json而不是xml？

因为json的数据描述更符合C++的数据描述，
举个例子，xml有Node和Attribute两种方式，
给出一个C++的Struct的Member field，
应该对应那种类型呢，如果是写入，
那么写成Node呢还是Attribute呢，
这里面对一个二义性问题。

---------------------------------------------------------

AJSON完全免费，没有任何限制，使用boost的授权方式。

0.2.2版本
=====
修改原先的内存修改策略，改为不修改源数据内存，
老的方式重新声明文件为ajson_msd.hpp，
习惯老方式的用户可以沿用，
新的方式会有轻微的性能降低，但是使用方式的限制取消了

0.2.1版本
=====
去掉了宏定义中的v.语法，感谢[奏之章]( https://github.com/mm304321141 )的建议
注意：由于该改动，ajson不能使用在final修饰的class上

0.2.0版本
=====
相对于0.1.x版本，ajson没有功能上的增加，主要的改动是去掉了对rapidjson和boost的依赖，完全独立了。