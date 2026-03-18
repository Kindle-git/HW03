// solution.h
#pragma once


#ifdef RUN_SOLUTION_BASIC
/* 인벤토리 시스템 구현을 위해서 필요한 것들
* 일단, 글로 적어봅시다.
* - 인벤토리 시스템이 무엇을 의미하는 걸까요?
* > 플레이어가 주은 아이템들을 모아서 하나의 창의 형태로 표현되는 것.
* > 아이템은 다양한 종류로 나뉘게 되며, 같은 종류는 모을 수 있으며(모을 수 있는 한도는 정해져 있다)
* > 최대치가 존재하며, 꽉 차면 들어가지 않습니다.
*
* - 이번 과제에서 무엇을 이용해야 할까요? / 어떤 배운 내용을 토대로 구현할 것인가요?
* > 클래스 템플릿 이용
* > 메모리 관리(new, 포인터, 참조)
*
* - 이후의 생각은 코드를 구상하면서 같이 적어봅시다.
* > 일단 제일 처음인 main 함수부터 만들어 봅시다!
*/

//(참고) 문장으로 보는 객체지향
// 클래스 인스턴스 == 주어(행동의 주체) > 누가?
// public 함수 == 동사(행동 자체) > 무엇을 하는가?
// private 함수 == 명사/목적어(상태/소유물) > 무엇을 가지고 있는가?

// 일단 시작해보자. 필수 구현 기능 체크리스트를 하나씩 해보자.


#include <string>		// std::string을 이용하기 위함(Item의 name_ 멤버 변수 참고).
#include <iostream>		// C++ 출력

// 우선 Item의 private로 가 아이템의 멤버 변수를 만들어봅시다.
class Item
{
public:
	// main 함수에서 보면, 처음 생성할 때 기본 생성자로 만듭니다. 이에 기본 생성자가 필요합니다.
	Item()
		: name_("")
		, price_(0) {
	}
	// 기본 생성자 선언과 동시에 멤버 변수를 초기화시킵니다. 

	// main 함수에서 인벤토리에 아이템을 추가해야 합니다(Inventory에서 AddItem 함수를 통해 아이템을 받음).
	// 이에 생성자를 추가로 만들어야 합니다.
	Item(const std::string& name, int price)
		: name_(name)
		, price_(price) {
	}

	// 이제 getter 함수를 이용하여 아이템 값을 출력하게 합니다.
	const std::string& GetName() const
	{
		return name_;
	}

	int GetPrice() const
	{
		return price_;
	}
	// Item을 만들었습니다! 이제 Inventory도 만들어 봅시다.

private:
	// Item(클래스)를 통해 어떤 것들을 만들 것인가요?
	// 아이템의 특징을 생각해 봅시다. 제일 간단한 것은 *이름과 가격*이 있겠네요.
	// 최근에 배운 std::string을 통해, 이름을 받아봅시다.
	std::string name_;	// #include <string>이 필요합니다.
	// 값은 int 형태로 받습니다.
	int price_;
	// 이제 public으로 가 아이템을 어떻게 생성할 것인지, 값을 어떻게 받을 것인지를 구현해 봅니다.
};



// 1. 데이터 보관함: Inventory<T> 클래스 설계
/* 1) 템플릿 클래스 선언
* > 클래스 이름: Inventory
* > 탬플릿: template <typename T>를 사용 -> 어떤 타입의 데이터든 처리 가능
*/
// 템플릿 클래스(인벤토리) 선언 -> private:로 이동

template <typename T>
// (+) T의 의미를 알기 위해 main함수를 보면, Inventory를 <Item>의 자료형으로 받습니다.
// 이때, Item은 클래스입니다. 이는 T -> class Item의 형태로 바꾸는 것을 의미합니다.
// 따라서 포인터 pItems_의 자료형이 Item의 클래스 형태라는 것을 알 수 있습니다.

class Inventory
{

public:

	// 2. 생성자와 소멸자(탄생과 소멸)
	// 1) 생성자 구현
	// main에서 인벤토리 객체 생성될 때 호출됩니다.
	// 매개변수로 용량(capacity)을 받습니다. 초기화는 int 10으로 설정합니다.
	// 예외 처리: 입력의 값이 0 이하라면, 최소 용량이 1은 되도록 자동으로 보정합니다.
	// new T[capacity] -> 아이템을 저장할 메모리 공간을 할당합니다(힙 메모리).
	Inventory(int capacity = 10)
	{
		if (capacity <= 0)
		{
			capacity = 1;
		}
		// 내부 멤버 변수에 매개변수의 값을 대입합니다.(초기화)
		capacity_ = capacity;
		size_ = 0;	// 탄생 시에는 아이템이 없으므로, 크기가 0입니다.
		// 아이템을 저장할 공간 할당하기!
		pItems_ = new T[capacity_];	// pItems_ 초기화??
		// (+) 이 코드는 초기화한다고 생각하기에 위험 요소가 있습니다.
		// > 초기화: 빌린 공간에 처음으로 의미 있는 값을 채워 넣는다는 것.
		// > 해당 코드는 값을 채워 넣는 것이 아닌, **공간을 할당하는** 코드입니다!
	}

	// 2) 소멸자 관리
	// 인벤토리 객체가 소멸할 때 자동으로 호출됩니다. 예) main함수에서 생성 후 main함수 종료 시
	// *메모리 누수 방지* : 객체가 사라질 때, delete[] pItems_를 실행합니다 -> 할당받은 메모리 반납합니다.
	// 안정성을 높이기 -> 해체 후 포인터의 주소 값을 nullptr로 초기화하여 안전성을 높입니다.
	~Inventory()
	{
		// 소멸자 생성 시 알림
		std::cout << "인벤토리 메모리 해제!" << std::endl;
		delete[] pItems_;
		pItems_ = nullptr;
	}

	// 3. 인벤토리 조작 기능
	// 1) 아이템 추가
	// (+) 매개변수를 const T& item으로 하는 이유
	// > pItems_[size_]에 아이템을 저장해야 합니다.
	// > 이때 저장할 아이템의 구성 요소는 해당 아이템의 이름과 값 등이 들어있습니다.
	// > 이에, 아이템 구성 요소를 만들기 위해 *Item 클래스*를 따로 만들어야 합니다.
	// > item 클래스를 만들었다면, Item을 어떻게 생성할 것인지 고민해야 합니다.
	// > 그런데, 왜 하필 (const T& item)의 형태일까요?
	// - 참조(&)를 사용한 이유는, Item 클래스의 데이터가 많고, 이를 그냥 대입할 시 데이터가 복사됩니다.
	// 이는 복사 시간과 메모리 소비가 심해질 수 있기에 실제 물건을 새로 만들지 않고, 실제 값의 주소만 알려줍니다.
	// - 읽기 전용(const)를 하는 이유: 참조(&)를 사용하면, 함수 내부에 값을 변경 시 실제 값이 바뀝니다!
	// 실제 값은 읽는 용도로만 사용합니다. 이에 값을 바뀌는 일이 없어야 하므로 const를 사용합니다.
	void AddItem(const T& item)
	{
		// 새로운 아이템을 인벤토리에 추가합니다.
		// 이때 인벤토리에 여유 공간이 있을 때에만 아이템을 저장해야 합니다.
		if (size_ < capacity_)
		{
			// 아이템을 저장한 후에, size_를 1 늘립니다.
			pItems_[size_++] = item;
			std::cout << "아이템" << size_ << "이 추가되었습니다." << std::endl;
		}
		else
		{
			std::cout << "인벤토리가 꽉 찼습니다!" << std::endl;
			std::cout << std::endl;
		}
	}

	// 2) 마지막 아이템 제거
	void RemoveLastItem()
	{
		// 실제 메모리를 지우는 것이 아닌, size_를 1 감소시켜 마지막 아이템에 접근할 수 없도록 합니다.
		// 비어있을 떄를 가정하여, 마지막 아이템을 제거할 지를 결정합니다.
		if (size_ > 0)
		{
			size_--;
		}
		else
		{
			std::cout << "인벤토리가 비어있습니다" << std::endl;
		}

	}
	// 3) 정보 조회 함수
	int GetSize() const
	{
		return size_;
	}
	int GetCapacity() const
	{
		return capacity_;
	}

	// 4) 정보 조회 함수
	void PrintAllItems() const
	{
		// 비어있는지 확인을 합니다.
		if (size_ > 0)
		{
			// size_ 크기만큼 아이템 내부 변수의 값을 호출합니다.
			for (int i = 0; i < size_; i++)
			{
				std::cout << "[이름: " << pItems_[i].GetName()
					<< ", 가격: " << pItems_[i].GetPrice() << "G]" << std::endl;
			}
		}
		else
		{
			std::cout << "비어있습니다!" << std::endl;
		}

	}
	// Inventory를 만들었습니다! 이제 main 함수로 가 실제로 인벤토리에 아이템을 넣어봅시다.

/* 2) 내부 멤버 변수 구성(private)
* > 변수를 선언만 합니다.
* > 생성자를 통해 멤버 변수를 초기화합니다.
* > 내부 멤버 함수를 통해 멤버 변수에 값을 대입, 포인터에는 주소를 대입시킵니다.
*/
private:
	// T -> 타입에 얽매이지 않는다 + 다양한 데이터를 담는다!
	// 포인터로 정의하는 이유: 아이템 객체를 저장할 **동적 배열을 가리키는 포인터**로 만들기 위함
	// > 아이템 객체를 동적 배열로 만드는 이유?
	// > 1) 데이터의 개수가 수시로 변할 때
	// > 2) 데이터의 양이 너무 많음 > 일반 배열로 감당하기 힘들 때
	// > 3) 객체 생성 및	소멸을 직접 조절하고 싶을 때
	T* pItems_;
	// 인벤토리의 최대 크기
	int capacity_;
	// 인벤토리 안에 들어있는 아이템의 갯수
	int size_;

	// Inventory -> public:로 이동(위에 있음)
};


// 0. 메인 함수에서, 무엇이 필요한지를 우선 상상해 봅시다.
// 우리는 메인 함수에서 인벤토리 관련 클래스를 생성하고, 그 내부에 있는 함수들을 이용하여 아이템을 받고, 출력해야합니다.

int main()
{
	// 그렇다면 Inventory 클래스를 생성하는 것을 기반으로, 어떻게 인벤토리 클래스를 만들지,  
	// 생성 시 어떻게 코드를 구성할지를 이해해야 합니다.
	// 인벤토리 클래스는 템플릿 클래스를 이용합니다.
	// 예) Inventory<T(?)> itemInventory;
	// 이때, T의 자료형으로 Item 클래스를 이용합니다. 이렇게 하면 Item 클래스의 기본 생성자를 통해 인벤토리 클래스 내부에서 
	// 아이템 클래스의 public 부분을 사용할 수 있게 됩니다(!)
	// 여기에서 하나 더 생각해야 할 부분이 있습니다. 인벤토리의 크기를 수정할 수 있게 하려면 어떻게 해야 할까요?
	// 일반적인 생성으로는 데이터의 개수를 변경하기는 힘들어 보입니다. 생성하면 데이터의 개수를 수시로 변경하기 힘드니까요.
	// 거기에다가, 추가할 부분이 많아지면 데이터의 양이 커질 수 밖에 없습니다. 따라서 **동적 할당**을 이용합니다.
	Inventory<Item>* itemInventory = new Inventory<Item>(10);
	// 여기에서 숫자 10의 의미는 인벤토리(템플릿 클래스)의 capacity_ 값을 의미합니다.
	// 쉽게 비유하자면, (10의 의미 == 인벤토리의 최대 값)이라고 할 수 있습니다.
	// 나중에 최대 크기를 변경하고자 한다면, vector를 이용해 볼 수 있겠네요. 아직 필수 기능만 생각해 보겠습니다.
	// 위의 코드를 통해, 인벤토리를 생성할 코드를 작성했습니다.

	// 이제 Inventory와 Item을 어떻게 구상할 것인지를 생각해 봐야 합니다.
	// 구상한 다음, 생성된 인벤토리를 통해 어떻게 아이템을 넣을 것인지를 생각해 봐야 합니다.
	// > main 함수에서 나와, Inventory와 Item을 만듭니다.

	// 아이템과 인벤토리를 다 만들었으면, 이제 실제로 인벤토리에 아이템을 넣어봅시다!
	for (int i = 0; i < 11; ++i)
	{
		itemInventory->AddItem(Item("Item" + std::to_string(i), (i + 1) * 100));
		// to_string : 숫자나 객체를 문자열(string) 형태로 변환하는 함수
	}

	// Inventory 내부 멤버 함수 PrintAllItems()를 통해 인벤토리 내부에 있는 아이템을 출력합니다. 

	itemInventory->PrintAllItems();

	// 인벤토리를 다 사용했으니, 이제 소멸시켜줍니다.
	// delete를 이용하여 인벤토리 객체를 소멸합니다. 이때 인벤토리에 있는 소멸자(~Inventory)가 자동으로 호출됩니다.

	delete itemInventory;
	itemInventory = nullptr;
	return 0;

	// 끝!!
}
#endif

#ifdef RUN_SOLUTION_HARD

#include <string>		// std::string을 이용하기 위함(Item의 name_ 멤버 변수 참고).
#include <iostream>		// C++ 출력

// 아이템
class Item
{
public:
	Item()
		: name_("")
		, price_(0) {
	}
	Item(const std::string& name, int price)
		: name_(name)
		, price_(price) {
	}

	const std::string& GetName() const
	{
		return name_;
	}

	int GetPrice() const
	{
		return price_;
	}

private:
	std::string name_;
	int price_;
};



// 1. 데이터 보관함: Inventory<T> 클래스 설계
template <typename T>
class Inventory
{

public:

	// 2. 생성자와 소멸자(탄생과 소멸)
	Inventory(int capacity = 10)
	{
		if (capacity <= 0)
		{
			capacity = 1;
		}
		capacity_ = capacity;
		size_ = 0;	
		pItems_ = new T[capacity_];

	}
	~Inventory()
	{
		// 소멸자 생성 시 알림
		std::cout << "인벤토리 메모리 해제!" << std::endl;
		delete[] pItems_;
		pItems_ = nullptr;
	}

	// H1_1. 복사 생성자 구현
	Inventory(const Inventory<T>& other)
	{
		capacity_ = other.capacity_;
		size_ = other.size_;
		pItems_ = new T[capacity_];
		for (int i = 0; i < size_; ++i)
		{
			pItems_[i] = other.pItems_[i];
		}
		std::cout << "인벤토리 복사 완료..!" << std::endl;
	}

	// H1_2. 대입 함수 구현(Assign)
	void Assign(const Inventory<T>& other)
	{
		// 1. 자기 자신을 대입하려 하는지 체크 (주소 비교)
		if (this == &other)
		{
			return;
		}

		// 2. 기존에 가지고 있던 메모리 해제 (중요!)
		delete[] pItems_;

		// 3. 상대방(other)의 정보 복사
		capacity_ = other.capacity_;
		size_ = other.size_;

		// 4. 새로운 메모리 할당 및 데이터 깊은 복사
		pItems_ = new T[capacity_];
		for (int i = 0; i < size_; ++i)
		{
			pItems_[i] = other.pItems_[i];
		}

		std::cout << "인벤토리 대입(Assign) 완료!" << std::endl;
	}

	// H2_1. Resize 멤버 함수 구현 (Resize)
	void Resize(int newCapacity)
	{
		// 1. 기존 크기와 같거나 너무 작은 값이 들어오면 방어 코드 작성
		if (newCapacity <= 0 || newCapacity == capacity_)
		{
			return;
		}

		// 2. 새로운 크기의 메모리 할당 (임시 포인터 활용)
		T* pNewItems = new T[newCapacity];
		
		// 3. 기존 데이터를 새 메모리로 복사
		// 만약 크기를 줄이는 경우라면, 새로운 capacity만큼만 복사해야 합니다.
		int countToCopy = (size_ < newCapacity) ? size_ : newCapacity;

		for (int i = 0; i < countToCopy; ++i)
		{
			pNewItems[i] = pItems_[i];
		}

		// 4. 기존 메모리 해제
		delete[] pItems_;

		// 5. 멤버 변수 정보 갱신
		pItems_ = pNewItems;
		capacity_ = newCapacity;
		size_ = countToCopy; // 혹시 크기가 줄어들었을 경우를 대비해 size_도 조정

		std::cout << "인벤토리 확장 완료! (용량: " << capacity_ << ")" << std::endl;

	}

	// 3. 인벤토리 조작 기능
	void AddItem(const T& item)
	{
		// 새로운 아이템을 인벤토리에 추가합니다.
		// 이때 인벤토리에 여유 공간이 있을 때에만 아이템을 저장해야 합니다.
		if (size_ < capacity_)
		{
			// 아이템을 저장한 후에, size_를 1 늘립니다.
			pItems_[size_++] = item;
			std::cout << "아이템" << size_ << "이 추가되었습니다." << std::endl;
		}
		else
		{
			std::cout << std::endl;
			std::cout << "인벤토리가 꽉 찼습니다!" << std::endl;
			std::cout << "인벤토리를 확장합니다,," << std::endl;		// AddItem 기능 업그레이드(용량 2배)
			Resize(capacity_ * 2);

			pItems_[size_++] = item;
			std::cout << "아이템" << size_ << "이 추가되었습니다." << std::endl;

			std::cout << std::endl;
		}
	}

	void RemoveLastItem()
	{
		// 실제 메모리를 지우는 것이 아닌, size_를 1 감소시켜 마지막 아이템에 접근할 수 없도록 합니다.
		// 비어있을 떄를 가정하여, 마지막 아이템을 제거할 지를 결정합니다.
		if (size_ > 0)
		{
			size_--;
		}
		else
		{
			std::cout << "인벤토리가 비어있습니다" << std::endl;
		}

	}

	int GetSize() const
	{
		return size_;
	}
	int GetCapacity() const
	{
		return capacity_;
	}

	// 4. 정보 조회 함수
	void PrintAllItems() const
	{
		if (size_ > 0)
		{
			std::cout << std::endl;
			std::cout << "---=[아이템 정보 조회]=---" << std::endl;
			// size_ 크기만큼 아이템 내부 변수의 값을 호출합니다.
			for (int i = 0; i < size_; i++)
			{
				std::cout << "[이름: " << pItems_[i].GetName()
					<< ", 가격: " << pItems_[i].GetPrice() << "G]" << std::endl;
			}
		}
		else
		{
			std::cout << "비어있습니다!" << std::endl;
		}

	}

private:
	T* pItems_;
	int capacity_;
	int size_;
};



int main()
{

	Inventory<Item>* itemInventory = new Inventory<Item>(10);

	for (int i = 0; i < 11; ++i)
	{
		itemInventory->AddItem(Item("Item" + std::to_string(i + 1), (i + 1) * 100));
		// to_string : 숫자나 객체를 문자열(string) 형태로 변환하는 함수
	}
	itemInventory->PrintAllItems();
	std::cout << "아이템 총량: " << itemInventory->GetCapacity() << std::endl;
	std::cout << std::endl;
	
	itemInventory->Resize(25);
	for (int i = 0; i < 14; ++i)
	{
		itemInventory->AddItem(Item("Item" + std::to_string(i), i * 100));
	}

	itemInventory->PrintAllItems();
	std::cout << "아이템 총량: " << itemInventory->GetCapacity() << std::endl;
	std::cout << std::endl;

	// SortItems() 함수 미구현

	// 복사 생성자
	Inventory<Item>* itemInventory2 = new Inventory<Item>(*itemInventory);
	itemInventory2->PrintAllItems();
	std::cout << "아이템 총량: " << itemInventory2->GetCapacity() << std::endl;
	std::cout << std::endl;


	/* 인벤토리 비었을 때 출력
	itemInventory->RemoveLastItem();
	itemInventory->RemoveLastItem();
	itemInventory->RemoveLastItem();
	itemInventory->PrintAllItems();
	std::cout << std::endl;

	itemInventory->RemoveLastItem();
	itemInventory->RemoveLastItem();
	itemInventory->RemoveLastItem();
	itemInventory->PrintAllItems();
	std::cout << std::endl;

	itemInventory->RemoveLastItem();
	itemInventory->RemoveLastItem();
	itemInventory->RemoveLastItem();
	itemInventory->PrintAllItems();
	std::cout << std::endl;

	itemInventory->RemoveLastItem();
	itemInventory->PrintAllItems();
	std::cout << std::endl;

	itemInventory->RemoveLastItem();
	itemInventory->PrintAllItems();
	std::cout << std::endl;
	
	// 삭제 후 아이템 다시 넣어보기
	itemInventory->AddItem(Item("Item" + std::to_string(11), 1200));
	itemInventory->PrintAllItems();
	std::cout << std::endl;
	*/
	delete itemInventory;
	itemInventory = nullptr;
	delete itemInventory2;
	itemInventory2 = nullptr;
	return 0;

	// 끝!!
}
#endif
