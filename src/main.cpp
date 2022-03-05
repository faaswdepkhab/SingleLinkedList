#include <cassert>
#include <cstddef>
//#include <string>
#include <utility>
#include <iterator>
#include <stdexcept>	
//#include <string>
#include <vector>
//#include <iostream>
#include <cassert>

template <typename Type>
class SingleLinkedList {
private:
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Node(const Type& val)
            : value(val)
            , next_node(nullptr) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;
        
    public:
        // Категория итератора - forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;
        
        BasicIterator() = default;
        
        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            _p = other._p;
        }
        
        BasicIterator(const BasicIterator<const Type>& other) noexcept {
            _p = other._p;
        }
        
        //BasicIterator& operator=(const BasicIterator& rhs) = default;
        BasicIterator& operator=(const BasicIterator<Type>& rhs){
            this->_p = rhs._p;
            return *this;
        }
        
        BasicIterator& operator=(const BasicIterator<const Type>& rhs){
            this->_p = rhs._p;
            return *this;
        }
        
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept{
            return _p == rhs._p;
        }
            
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return _p != rhs._p;
        }
            
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return _p == rhs._p;
        }
            
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return _p != rhs._p;
        }
            
        [[nodiscard]] reference operator*() const noexcept {
			assert(_p != nullptr);
            return (_p->value);
        }
        
        [[nodiscard]] pointer operator->() const noexcept {
			assert(_p != nullptr);
            return &(_p->value);
        }
        
        BasicIterator& operator++() noexcept {
			assert(_p != nullptr);
           _p = _p -> next_node;
            return *this;
        }
            
        BasicIterator operator++(int) noexcept {
            auto old_value(this->_p); // Сохраняем прежнее значение объекта для последующего возврата
            ++(*this); // используем логику префиксной формы инкремента
            return old_value;
        }
    private:
        BasicIterator(Node* p):_p(p){
        }
        Node* _p;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;
    
    SingleLinkedList(){
        head_.next_node = nullptr;
        size_ = 0;
        phead_ = &head_;
    }
    
    SingleLinkedList(std::initializer_list<Type> values) {
        auto rb = rbegin(values);
        auto re = rend(values);
        size_ = 0;
        head_.next_node = nullptr;
        while (rb != re){
            PushFront(*rb);
            rb++;
        }
        phead_ = &head_;
    }
    
    SingleLinkedList(const SingleLinkedList& other) {
        size_ = 0;
        head_.next_node = nullptr;
        phead_ = &head_;
        Node* p = nullptr;
        Node*  t = nullptr;
        try {
            for (auto &item:other){
                t = new Node(item, nullptr);
                if (p){
                    p->next_node = t;
                } else{
                    head_.next_node =t;
                }
                p = t;
                t = nullptr;
                size_++;
            }
        }
        catch(...){
            delete t;
        	Clear();
            size_ = 0;
            throw;
        }
    }
    
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs){
            auto rhs_copy(rhs);
            swap(rhs_copy);
        }
        return *this;
    }
    
    ~SingleLinkedList(){
        Clear();
    }
    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }
    
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    
    void Clear() {
        Node* first_node = head_.next_node;
        while (first_node != nullptr){
            head_.next_node = head_.next_node->next_node;
            delete first_node;
            first_node = head_.next_node;;
        }
        size_ = 0;
    }
    
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }
    
    [[nodiscard]] BasicIterator<Type> end() noexcept {
        return Iterator(nullptr);
    }
    
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }
    
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }
    
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }
    
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
    
    template <typename V>    
    friend void swap(SingleLinkedList<V>& lhs, SingleLinkedList<V>& rhs) noexcept;
    
    void swap(SingleLinkedList& rhs) noexcept {
    	if (this == &rhs){
    		return;
    	}
    	std::swap(this->head_.next_node, rhs.head_.next_node);
    	std::swap(this->size_, rhs.size_); 
    }
    
    bool operator==(const SingleLinkedList<Type>& rhs) const{
        return std::equal(this->begin(), this->end(), rhs.begin());
    }
    
    bool operator!=(const SingleLinkedList& rhs) const{
        return !(*this == rhs);
    }
    
    bool operator<(const SingleLinkedList& rhs) const{
        return std::lexicographical_compare(this->begin(), this->end(), rhs.begin(), rhs.end());
    }
    
    bool operator<=(const SingleLinkedList& rhs) const{
        return !(*this > rhs);
    }
    
    bool operator>(const SingleLinkedList& rhs) const{
        return (rhs < *this);
    }
    
    bool operator>=(const SingleLinkedList& rhs) const{
        return !(rhs > *this);
    }
    
    void PopFront(){
        Node *p = head_.next_node;
        if (p){
            head_.next_node = p->next_node;
            delete p;
            size_--;
        }
    }
    
    Iterator InsertAfter(const Iterator &pos, const Type value){
        if (!(pos._p)){
            return Iterator(nullptr);
        }
        Node *p = new Node(value, pos._p->next_node);
        pos._p->next_node = p;
        size_++;
        return Iterator(p);
    }
    
    Iterator EraseAfter(const Iterator &pos){
        if (!(pos._p)){
            return Iterator(nullptr);
        }
        Node *p = pos._p->next_node;
        pos._p->next_node = p->next_node;
        delete p;
        size_--;
        return Iterator(pos._p->next_node);
    }
    
    [[nodiscard]] Iterator before_begin() const noexcept {
        return Iterator(phead_);
    }
    
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(phead_);
    }
    
private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    Node* phead_;
    size_t size_;
    
    template <typename Iter>
    void Init(Iter begin, Iter end){
        Node *p = &head_;
        for (auto it = begin; it != end; ++it) {
            p->next_node = new Node(*it);
            p = p->next_node;
            size_++;
        }
    }
	
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    if (&lhs == &rhs){
    	return;
    }	
    lhs.swap(rhs);
}


void Test0() {
    using namespace std;
    {
        const SingleLinkedList<int> empty_int_list;
        assert(empty_int_list.GetSize() == 0u);
        assert(empty_int_list.IsEmpty());
    }

    {
        const SingleLinkedList<string> empty_string_list;
        assert(empty_string_list.GetSize() == 0u);
        assert(empty_string_list.IsEmpty());
    }
    
    {
        SingleLinkedList<string> string_list;
        string s="sfsfd";
        string_list.PushFront(s);
        s="ffghfgh";
        string_list.PushFront(s);
        assert(string_list.GetSize() == 2u);
        string_list.Clear();
        assert(string_list.GetSize() == 0u);
        assert(string_list.IsEmpty());
    }
    
    {
        SingleLinkedList<string> string_list;
        assert(string_list.begin() == string_list.end());
    }
}

// Эта функция тестирует работу SingleLinkedList
void Test2() {
    // Итерирование по пустому списку
    {
        SingleLinkedList<int> list;
        // Константная ссылка для доступа к константным версиям begin()/end()
        const auto& const_list = list;

        // Итераторы begine и end у пустого диапазона равны друг другу
        assert(list.begin() == list.end());
        assert(const_list.begin() == const_list.end());
        assert(list.cbegin() == list.cend());
        assert(list.cbegin() == const_list.begin());
        assert(list.cend() == const_list.end());
    }

    // Итерирование по непустому списку
    {
        SingleLinkedList<int> list;
        const auto& const_list = list;

        list.PushFront(1);
        assert(list.GetSize() == 1u);
        assert(!list.IsEmpty());

        assert(const_list.begin() != const_list.end());
        assert(const_list.cbegin() != const_list.cend());
        assert(list.begin() != list.end());

        assert(const_list.begin() == const_list.cbegin());

        assert(*list.cbegin() == 1);
        *list.begin() = -1;
        assert(*list.cbegin() == -1);

        const auto old_begin = list.cbegin();
        list.PushFront(2);
        assert(list.GetSize() == 2);

        const auto new_begin = list.cbegin();
        assert(new_begin != old_begin);
        // Проверка прединкремента
        {
            auto new_begin_copy(new_begin);
            assert((++(new_begin_copy)) == old_begin);
        }
        // Проверка постинкремента
        {
            auto new_begin_copy(new_begin);
            assert(((new_begin_copy)++) == new_begin);
            assert(new_begin_copy == old_begin);
        }
        // Итератор, указывающий на позицию после последнего элемента равен итератору end()
        {
            auto old_begin_copy(old_begin);
            assert((++old_begin_copy) == list.end());
        }
    }
    // Преобразование итераторов
    {
        SingleLinkedList<int> list;
        list.PushFront(1);
        // Конструирование ConstItrator из Iterator
        SingleLinkedList<int>::ConstIterator const_it(list.begin());
        assert(const_it == list.cbegin());
        assert(*const_it == *list.cbegin());

        SingleLinkedList<int>::ConstIterator const_it1;
        // Присваивание ConstIterator-у значения Iterator
        const_it1 = list.begin();
        assert(const_it1 == const_it);
    }
    // Проверка оператора ->
    {
        using namespace std;
        SingleLinkedList<std::string> string_list;

        string_list.PushFront("one"s);
        assert(string_list.cbegin()->length() == 3u);
        string_list.begin()->push_back('!');
        assert(*string_list.begin() == "one!"s);
    }
}

void Test3() {
    // Проверка списков на равенство и неравенство
    {
        SingleLinkedList<int> list_1;
        list_1.PushFront(1);
        list_1.PushFront(2);

        SingleLinkedList<int> list_2;
        list_2.PushFront(1);
        list_2.PushFront(2);
        list_2.PushFront(3);

        SingleLinkedList<int> list_1_copy;
        list_1_copy.PushFront(1);
        list_1_copy.PushFront(2);

        SingleLinkedList<int> empty_list;
        SingleLinkedList<int> another_empty_list;

        // Список равен самому себе
        assert(list_1 == list_1);
        assert(empty_list == empty_list);

        // Списки с одинаковым содержимым равны, а с разным - не равны
        assert(list_1 == list_1_copy);
        assert(list_1 != list_2);
        assert(list_2 != list_1);
        assert(empty_list == another_empty_list);
    }

    // Обмен содержимого списков
    {
        SingleLinkedList<int> first;
        first.PushFront(1);
        first.PushFront(2);

        SingleLinkedList<int> second;
        second.PushFront(10);
        second.PushFront(11);
        second.PushFront(15);

        const auto old_first_begin = first.begin();
        const auto old_second_begin = second.begin();
        const auto old_first_size = first.GetSize();
        const auto old_second_size = second.GetSize();

        first.swap(second);

        assert(second.begin() == old_first_begin);
        assert(first.begin() == old_second_begin);
        assert(second.GetSize() == old_first_size);
        assert(first.GetSize() == old_second_size);

        // Обмен при помощи функции swap
        {
            using std::swap;

            // В отсутствие пользовательской перегрузки будет вызвана функция std::swap, которая
            // выполнит обмен через создание временной копии
            swap(first, second);

            // Убеждаемся, что используется не std::swap, а пользовательская перегрузка

            // Если бы обмен был выполнен с созданием временной копии,
            // то итератор first.begin() не будет равен ранее сохранённому значению,
            // так как копия будет хранить свои узлы по иным адресам
            assert(first.begin() == old_first_begin);
            assert(second.begin() == old_second_begin);
            assert(first.GetSize() == old_first_size);
            assert(second.GetSize() == old_second_size);
        }
    }
    // Инициализация списка при помощи std::initializer_list
    {
        SingleLinkedList<int> list{1, 2, 3, 4, 5};
        assert(list.GetSize() == 5);
        assert(!list.IsEmpty());
        assert(std::equal(list.begin(), list.end(), std::begin({1, 2, 3, 4, 5})));
    }

    // Лексикографическое сравнение списков
    {
        using IntList = SingleLinkedList<int>;

        assert((IntList{1, 2, 3} < IntList{1, 2, 3, 1}));
        assert((IntList{1, 2, 3} <= IntList{1, 2, 3}));
        assert((IntList{1, 2, 4} > IntList{1, 2, 3}));
        assert((IntList{1, 2, 3} >= IntList{1, 2, 3}));
    }

    // Копирование списков
    {
        const SingleLinkedList<int> empty_list{};
        // Копирование пустого списка
        {
            auto list_copy(empty_list);
            assert(list_copy.IsEmpty());
        }

        SingleLinkedList<int> non_empty_list{1, 2, 3, 4};
        // Копирование непустого списка
        {
            auto list_copy(non_empty_list);

            assert(non_empty_list.begin() != list_copy.begin());
            assert(list_copy == non_empty_list);
        }
    }
    // Присваивание списков
    {
        const SingleLinkedList<int> source_list{1, 2, 3, 4};

        SingleLinkedList<int> receiver{5, 4, 3, 2, 1};
        receiver = source_list;
        assert(receiver.begin() != source_list.begin());
        assert(receiver == source_list);
    }

    // Вспомогательный класс, бросающий исключение после создания N-копии
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                } else {
                    --(*countdown_ptr);
                }
            }
        }
        // Присваивание элементов этого типа не требуется
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
        // Как только обнулится, конструктор копирования выбросит исключение
        int* countdown_ptr = nullptr;
    };

    // Безопасное присваивание списков
    {
        SingleLinkedList<ThrowOnCopy> src_list;
        src_list.PushFront(ThrowOnCopy{});
        src_list.PushFront(ThrowOnCopy{});
        auto thrower = src_list.begin();
        src_list.PushFront(ThrowOnCopy{});

        int copy_counter = 0;  // при первом же копировании будет выброшего исключение
        thrower->countdown_ptr = &copy_counter;

        SingleLinkedList<ThrowOnCopy> dst_list;
        dst_list.PushFront(ThrowOnCopy{});
        int dst_counter = 10;
        dst_list.begin()->countdown_ptr = &dst_counter;
        dst_list.PushFront(ThrowOnCopy{});
		
        try {
            dst_list = src_list;
            // Ожидается исключение при присваивании
            assert(false);
        } catch (const std::bad_alloc&) {
            // Проверяем, что состояние списка-приёмника не изменилось
            // при выбрасывании исключений
            assert(dst_list.GetSize() == 2);
            auto it = dst_list.begin();
            assert(it != dst_list.end());
            assert(it->countdown_ptr == nullptr);
            ++it;
            assert(it != dst_list.end());
            assert(it->countdown_ptr == &dst_counter);
            assert(dst_counter == 10);
        } catch (...) {
            // Других типов исключений не ожидается
            assert(false);
        }
    }
}

int main() {
    Test0();
    Test2();
    Test3();
}
