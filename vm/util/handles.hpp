#ifndef HANDLES_HPP
#define HANDLES_HPP

class HiObject;
class OopClosure;
class HiList;
class HiDict;

template<typename T>
class ArrayList;

class LinkedList {
friend class HandleMark;
private:
    LinkedList* _next;
    LinkedList* _prev;

public:
    LinkedList();

    virtual void oops_do(OopClosure* f) = 0;
};

template <typename T>
class Handle : public LinkedList {
protected:
    T _value;

public:
    Handle(T t);
    Handle(const Handle<T>& t);
    ~Handle();

    virtual void oops_do(OopClosure* f);
    T operator ->();
    T operator ()() { return _value; }
    operator T()    { return _value; }
    void operator =(T v)  { _value = v; }
    void operator =(Handle<T>& v)  { _value = v(); }
    bool operator ==(Handle<T>& v) { return _value == v(); }
    bool operator ==(T v)         { return _value == v; }
    bool operator !=(Handle<T>& v) { return _value != v(); }
    bool operator !=(T v)         { return _value != v; }
};

class HandleMark {
private:
    static HandleMark* instance;
    LinkedList* _head;

public:
    HandleMark();

    static HandleMark* get_instance();
    void oops_do(OopClosure* f);
    LinkedList* head()           { return _head; }
    void set_head(LinkedList* x);
    void del_handle(LinkedList* x);
};

#endif

