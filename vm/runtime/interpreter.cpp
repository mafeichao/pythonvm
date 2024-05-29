#include "runtime/stringTable.hpp"
#include "runtime/universe.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/frameObject.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/cellObject.hpp"
#include "runtime/module.hpp"
#include "object/arrayList.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "object/typeObject.hpp"
#include "memory/oopClosure.hpp"
#include "util/map.hpp"
#include "util/handles.hpp"

#include <string.h>

#define POP()         _frame->stack()->pop()
#define TOP()         _frame->stack()->top()
#define STACK_LEVEL() _frame->stack()->size()
#define PEEK(x)       _frame->stack()->get((x))
#define EMPTY()       (_frame->stack()->size() == 0)

#define HI_TRUE       Universe::HiTrue
#define HI_FALSE      Universe::HiFalse

Interpreter* Interpreter::_instance = NULL;

Interpreter* Interpreter::get_instance() {
    if (_instance == NULL) {
        _instance = new Interpreter();
    }

    return _instance;
}

Interpreter::Interpreter() {
}

void Interpreter::initialize() {
    _search_path = HiList::new_instance();
    _search_path->append(ST(lib));

    _builtins = ModuleObject::import_module(HiString::new_instance("builtins"));
    _modules  = HiDict::new_instance();

    _builtins->put(HiString::new_instance("True"),     Universe::HiTrue);
    _builtins->put(HiString::new_instance("False"),    Universe::HiFalse);
    _builtins->put(HiString::new_instance("None"),     Universe::HiNone);

    _builtins->put(HiString::new_instance("type"),     TypeKlass::get_instance()->type_object());
    _builtins->put(HiString::new_instance("object"),   ObjectKlass::get_instance()->type_object());
    _builtins->put(HiString::new_instance("int"),      IntegerKlass::get_instance()->type_object());
    _builtins->put(HiString::new_instance("str"),      StringKlass::get_instance()->type_object());
    _builtins->put(HiString::new_instance("list"),     ListKlass::get_instance()->type_object());
    _builtins->put(HiString::new_instance("dict"),     DictKlass::get_instance()->type_object());

    Handle<HiString*> name = HiString::new_instance("print");
    _builtins->put(name,         new FunctionObject(object_print, name));
    name = HiString::new_instance("len");
    _builtins->put(name,         new FunctionObject(object_len, name));
    name = HiString::new_instance("repr");
    _builtins->put(name,         new FunctionObject(object_repr, name));
    name = HiString::new_instance("isinstance");
    _builtins->put(name,         new FunctionObject(isinstance, name));
    name = HiString::new_instance("sysgc");
    _builtins->put(name,         new FunctionObject(sysgc, name));
    _builtins->put(ST(build_class), new FunctionObject(build_type_object, ST(build_class)));

    _modules->put(HiString::new_instance("__builtins__"), _builtins);
}

void Interpreter::add_search_path(const char* x) {
    Handle<HiList*> args = HiList::new_instance();
    args->append(HiString::new_instance(x));
    args->append(HiString::new_instance("/"));

    Handle<HiList*> t = string_rpartition(args, nullptr)->as<HiList>();
    _search_path->append(t->get(0)->add(t->get(1)));
}

void Interpreter::PUSH(Handle<HiObject*> x) {
    _frame->_stack->append(x);
}

void Interpreter::destroy() {
    delete _instance;
    _instance = nullptr;
}

void Interpreter::build_frame(Handle<HiObject*> callable, 
    Handle<HiList*> args, Handle<HiList*> kwargs) {
    if (MethodObject::is_method(callable)) {
        Handle<MethodObject*> method = callable->as<MethodObject>();
        // return value is ignored here, because they are handled
        // by other pathes.
        if (!args) {
            args = HiList::new_instance();
        }
        args->insert(0, method->owner());
        build_frame(method->func(), args, kwargs);
    }
    else if (callable->klass() == FunctionKlass::get_instance()) {
        FrameObject* frame = new FrameObject();
        frame->set_sender(_frame);
        _frame = frame;
        _frame->initialize(callable->as<FunctionObject>(), args, kwargs);
    }
    else {
        Handle<HiObject*> result = callable->call(args, nullptr);
        PUSH(result);
    }
}

HiObject* Interpreter::call_virtual(Handle<HiObject*> func, Handle<HiList*> args) {
    if (MethodObject::is_native(func)) {
        // we do not create a virtual frame, but native frame.
        return func->call(args, nullptr);
    }
    else if (MethodObject::is_method(func)) {
        MethodObject* method = func->as<MethodObject>();
        // return value is ignored here, because they are handled
        // by other pathes.
        if (!args) {
            args = HiList::new_instance();
        }
        args->insert(0, method->owner());
        return call_virtual(method->func(), args);
    }
    else if (MethodObject::is_function(func)) {
        int size = args ? args->length() : 0;

        FrameObject* frame = new FrameObject();
        enter_frame(frame);
        _frame->initialize(func->as<FunctionObject>(), args, nullptr);
        _frame->set_entry_frame(true);
        eval_frame();
        destroy_frame();

        return _ret_value;
    }

    return Universe::HiNone;
}

void Interpreter::leave_frame() {
    destroy_frame();
    PUSH(_ret_value);
}

void Interpreter::destroy_frame() {
    FrameObject* temp = _frame;
    _frame = _frame->sender();

    delete temp;
}

void Interpreter::enter_frame(FrameObject* frame) {
    frame->set_sender(_frame);
    _frame         = frame;
}

void Interpreter::run(CodeObject* codes) {
    _frame = new FrameObject();
    _frame->initialize(codes);
    _frame->locals()->put(ST(name), HiString::new_instance("__main__"));
    eval_frame();

    destroy_frame();
}

HiDict* Interpreter::run_mod(Handle<CodeObject*> codes, Handle<HiString*> mod_name) {
    FrameObject* frame = new FrameObject();
    enter_frame(frame);
    frame->initialize(codes);
    frame->set_entry_frame(true);
    frame->locals()->put(ST(name), mod_name);

    eval_frame();
    HiDict* result = frame->locals();
    destroy_frame();
    return result;
}

void Interpreter::eval_frame() {
    Handle<FunctionObject*> fo = nullptr;
    Handle<HiList*> args = nullptr;
    Handle<HiList*> kwargs = nullptr;
    Handle<HiList*> lst = nullptr;
    Handle<HiInteger*> lhs = nullptr;
    Handle<HiInteger*> rhs = nullptr;
    Handle<HiObject*> v = nullptr;
    Handle<HiObject*> w = nullptr;
    Handle<HiObject*> u = nullptr;
    Handle<HiObject*> attr = nullptr;
    int arg_cnt = 0;

    while (_frame->has_more_codes()) {
        unsigned char op_code = _frame->get_op_code();
        int op_arg = _frame->get_op_arg();

        switch (op_code) {
            case ByteCode::LOAD_CONST:
                PUSH(_frame->consts()->get(op_arg));
                break;

            case ByteCode::LOAD_NAME:
                v = _frame->names()->get(op_arg);
                w = _frame->locals()->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = _frame->globals()->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = Interpreter::get_instance()->_builtins->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                PUSH(Universe::HiNone);
                break;

            case ByteCode::LOAD_FAST:
                PUSH(_frame->fast_locals()->get(op_arg));
                break;

            case ByteCode::LOAD_GLOBAL:
                v = _frame->names()->get(op_arg);
                w = _frame->globals()->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = _builtins->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                PUSH(Universe::HiNone);
                break;

            case ByteCode::STORE_ATTR:
                u = POP();
                v = _frame->_names->get(op_arg);
                w = POP();
                u->setattr(v, w);
                break;

            case ByteCode::LOAD_ATTR:
            case ByteCode::LOAD_METHOD:
                v = POP();
                w = _frame->_names->get(op_arg);
                PUSH(v->getattr(w));
                break;

            case ByteCode::STORE_FAST:
                _frame->_fast_locals->set(op_arg, POP());
                break;

            case ByteCode::STORE_NAME:
                v = _frame->names()->get(op_arg);
                _frame->locals()->put(v, POP());
                break;

            case ByteCode::STORE_GLOBAL:
                v = _frame->names()->get(op_arg);
                _frame->globals()->put(v, POP());
                break;

            case ByteCode::UNPACK_SEQUENCE:
                v = POP();

                while (op_arg--) {
                    PUSH(v->as<HiList>()->get(op_arg));
                }
                break;

            case ByteCode::POP_TOP:
                POP();
                break;

            case ByteCode::POP_BLOCK:
                _frame->pop_block();
                break;

            case ByteCode::INPLACE_ADD:
            case ByteCode::BINARY_ADD:
                v = POP();
                w = POP();
                PUSH(w->add(v));
                break;

            case ByteCode::INPLACE_SUBSTRACT:
            case ByteCode::BINARY_SUBSTRACT:
                v = POP();
                w = POP();
                PUSH(w->sub(v));
                break;

            case ByteCode::BINARY_MULTIPLY:
                v = POP();
                w = POP();
                PUSH(w->mul(v));
                break;

            case ByteCode::BINARY_TRUE_DIVIDE:
                v = POP();
                w = POP();
                PUSH(w->true_div(v));
                break;

            case ByteCode::BINARY_SUBSCR:
                v = POP();
                w = POP();
                PUSH(w->subscr(v));
                break;

            case ByteCode::STORE_SUBSCR:
                u = POP();
                v = POP();
                w = POP();
                v->store_subscr(u, w);
                break;

            case ByteCode::MAKE_FUNCTION:
                w = POP(); // function name
                v = POP();
                fo = new FunctionObject(v);
                fo->set_globals(_frame->globals());
                if (op_arg & 0x8) {
                    fo->set_closure(POP()->as<HiList>());
                }

                op_arg &= 0x7;

                if (op_arg == 1) {
                    HiList* t = POP()->as<HiList>();
                    args = HiList::new_instance();
                    for (int i = 0; i < t->length(); i++) {
                        args->append(t->get(i));
                    }
                }
                fo->set_default(args);
                PUSH(fo());

                break;

            case ByteCode::LOAD_CLOSURE:
                v = _frame->closure()->get(op_arg);
                if (v == NULL) {
                    _frame->closure()->set(op_arg, (_frame->get_cell_from_parameter(op_arg)));
                }

                v = _frame->closure()->get(op_arg);
                if (v->klass() == CellKlass::get_instance()) {
                    PUSH(v);
                }
                else
                    PUSH(new CellObject(_frame->closure(), op_arg));

                break;

            case ByteCode::LOAD_DEREF:
                v = _frame->closure()->get(op_arg);
                if (v->klass() == CellKlass::get_instance()) {
                    v = v->as<CellObject>()->value();
                }
                PUSH(v);
                break;

            case ByteCode::STORE_DEREF:
                v = _frame->closure()->get(op_arg);
                w = POP();

                if (v == nullptr || v->klass() != CellKlass::get_instance()) {
                    _frame->closure()->set(op_arg, w);
                }
                else {
                    v->as<CellObject>()->set_value(w);
                }

                break;

            case ByteCode::LOAD_BUILD_CLASS:
                PUSH(_builtins->get(ST(build_class)));
                break;

            case ByteCode::CALL_METHOD:
            case ByteCode::CALL_FUNCTION:
                if (op_arg > 0) {
                    args = HiList::new_instance();
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }

                fo = (FunctionObject*)POP();
                build_frame(fo(), args);

                if (args) {
                    args->clear();
                }

                break;

            case ByteCode::CALL_FUNCTION_KW:
                assert(op_arg > 0);
                arg_cnt = op_arg;
                kwargs = POP()->as<HiList>();
                args = HiList::new_instance();
                while (arg_cnt--) {
                    args->set(arg_cnt, POP());
                }

                fo = static_cast<FunctionObject*>(POP());
                build_frame(fo(), args, kwargs);

                if (args) {
                    args->clear();
                }

                break;

            case ByteCode::CALL_FUNCTION_EX:
                assert(op_arg == 0);
                args = POP()->as<HiList>();
                fo = POP()->as<FunctionObject>();
                build_frame(fo(), args, kwargs);

                if (args) {
                    args->clear();
                }

                break;

            case ByteCode::CALL_FINALLY:
                PUSH((HiObject*)((long long)(_frame->get_pc() << 1) | 0x1));
                _frame->set_pc(_frame->get_pc() + op_arg);
                break;

            case ByteCode::RETURN_VALUE:
                _ret_value = POP();
                if (_frame->is_first_frame() ||
                        _frame->is_entry_frame())
                    return;
                leave_frame();
                break;

            case ByteCode::END_FINALLY:
                v = POP();
                if (v == nullptr) {
                    // do nothing.
                }
                else if (((long long)v()) & 0x1) {
                    _frame->set_pc(((long long)v()) >> 1);
                }
                else {
                    // do nothing
                }
                break;

            case ByteCode::COMPARE_OP:
                w = POP();
                v = POP();

                switch(op_arg) {
                case ByteCode::GREATER:
                    PUSH(v->greater(w));
                    break;

                case ByteCode::LESS:
                    PUSH(v->less(w));
                    break;

                case ByteCode::EQUAL:
                    PUSH(v->equal(w));
                    break;

                case ByteCode::NOT_EQUAL:
                    PUSH(v->not_equal(w));
                    break;

                case ByteCode::GREATER_EQUAL:
                    PUSH(v->ge(w));
                    break;

                case ByteCode::LESS_EQUAL:
                    PUSH(v->le(w));
                    break;

                case ByteCode::IS:
                    if (v == w)
                        PUSH(HI_TRUE);
                    else
                        PUSH(HI_FALSE);
                    break;

                case ByteCode::IS_NOT:
                    if (v == w)
                        PUSH(HI_TRUE);
                    else
                        PUSH(HI_FALSE);
                    break;

                case ByteCode::IN:
                    PUSH(w->contains(v));
                    break;

                default:
                    printf("Error: Unrecognized compare op %d\n", op_arg);
                }
                break;

            case ByteCode::POP_JUMP_IF_FALSE:
                v = POP();
                if (v == Universe::HiFalse || v == Universe::HiNone)
                    _frame->set_pc(op_arg);
                break;

            case ByteCode::JUMP_FORWARD:
                _frame->set_pc(op_arg + _frame->get_pc());
                break;

            case ByteCode::JUMP_ABSOLUTE:
                _frame->set_pc(op_arg);
                break;

            case ByteCode::BUILD_LIST:
            case ByteCode::BUILD_TUPLE:
                lst = HiList::new_instance();
                while (op_arg--) {
                    lst->set(op_arg, POP());
                }
                PUSH(lst());
                break;

            case ByteCode::DELETE_SUBSCR:
                w = POP();
                v = POP();
                v->del_subscr(w);
                break;

            case ByteCode::GET_ITER:
                v = POP();
                PUSH(v->iter());
                break;

            case ByteCode::FOR_ITER:
                v = TOP();
                w = v->getattr(ST(next));
                build_frame(w, NULL);

                if (TOP() == NULL) {
                    _frame->_pc += op_arg;
                    POP();
                }
                break;

            case ByteCode::BUILD_MAP:
                v = HiDict::new_instance();
                for (int i = 0; i < op_arg; i++) {
                    v->as<HiDict>()->put(POP(), POP());
                }
                PUSH(v);
                break;
                
            case ByteCode::BUILD_CONST_KEY_MAP:
                lst = POP()->as<HiList>();

                v = HiDict::new_instance();
                for (int i = 0; i < op_arg; i++) {
                    v->as<HiDict>()->put(lst->get(op_arg - i - 1), POP());
                }

                PUSH(v);
                break;

            case ByteCode::BUILD_TUPLE_UNPACK_WITH_CALL:
                v = POP();
                args = HiList::new_instance();
                op_arg--;

                while (op_arg--) {
                    w = POP();
                    args->append(w);
                    args->append(v);
                    list_extend(args, nullptr);
                    v = args->get(0);
                    args->clear();
                }
                PUSH(w);

                break;

            case ByteCode::IMPORT_NAME:
                v = _frame->names()->get(op_arg);
                w = _modules->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }
                w = ModuleObject::import_module(v);
                _modules->put(v, w);
                PUSH(w);
                break;

            case ByteCode::IMPORT_FROM:
                v = _frame->names()->get(op_arg);
                w = TOP();
                u = w->getattr(v);
                PUSH(u);
                break;

            case ByteCode::SETUP_FINALLY:
                _frame->setup_block(ByteCode::SETUP_FINALLY, 
                    _frame->get_pc() + op_arg, STACK_LEVEL());
                break;

            case ByteCode::POP_FINALLY:
                v = POP();
                if (op_arg)
                    w = POP();
                if (v == nullptr || ((long long)v()) & 0x1) {
                    // do nothing.
                }
                else {
                    // do nothing
                }
                if (op_arg)
                    PUSH(w);
                break;

            default:
                printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
}

void Interpreter::oops_do(OopClosure* f) {
    f->do_oop((HiObject**)&_search_path);
    f->do_oop((HiObject**)&_modules);
    f->do_oop((HiObject**)&_builtins);
    f->do_oop((HiObject**)&_ret_value);

    if (_frame)
        _frame->oops_do(f);
}

