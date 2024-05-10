#include "runtime/stringTable.hpp"
#include "runtime/universe.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/frameObject.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/cellObject.hpp"
#include "object/arrayList.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "util/map.hpp"

#include <string.h>

#define PUSH(x)       _frame->stack()->append((x))
#define POP()         _frame->stack()->pop()
#define TOP()         _frame->stack()->top()

#define HI_TRUE       Universe::HiTrue
#define HI_FALSE      Universe::HiFalse

Interpreter::Interpreter() {
    _builtins = new HiDict();

    _builtins->put(new HiString("True"),     Universe::HiTrue);
    _builtins->put(new HiString("False"),    Universe::HiFalse);
    _builtins->put(new HiString("None"),     Universe::HiNone);

    _builtins->put(new HiString("print"),    new FunctionObject(object_print));
    _builtins->put(new HiString("len"),      new FunctionObject(len));
}

void Interpreter::build_frame(HiObject* callable, HiList* args, HiList* kwargs) {
    if (callable->klass() == NativeFunctionKlass::get_instance()) {
        PUSH(((FunctionObject*)callable)->call(args));
    }
    else if (MethodObject::is_method(callable)) {
        MethodObject* method = (MethodObject*) callable;
        // return value is ignored here, because they are handled
        // by other pathes.
        if (!args) {
            args = new HiList();
        }
        args->insert(0, method->owner());
        build_frame(method->func(), args, kwargs);
    }
    else if (callable->klass() == FunctionKlass::get_instance()) {
        FrameObject* frame = new FrameObject((FunctionObject*) callable, args, kwargs);
        frame->set_sender(_frame);
        _frame = frame;
    }
}

void Interpreter::leave_frame(HiObject* return_value) {
    if (!_frame->sender()) {
        delete _frame;
        _frame = NULL;
        return;
    }

    FrameObject* temp = _frame;
    _frame         = _frame->sender();
    PUSH(return_value);

    delete temp;
}

void Interpreter::run(CodeObject* codes) {
    _frame = new FrameObject(codes);

    while (_frame->has_more_codes()) {
        unsigned char op_code = _frame->get_op_code();
        int op_arg = _frame->get_op_arg();

        FunctionObject* fo;
        HiList* args = nullptr;
        HiList* kwargs = nullptr;
        HiList* lst;
        HiInteger* lhs, * rhs;
        HiObject* v, * w, * u, * attr;
        int arg_cnt = 0;

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

                w = _builtins->get(v);
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
                    PUSH(v->subscr(new HiInteger(op_arg)));
                }
                break;

            case ByteCode::POP_TOP:
                POP();
                break;

            case ByteCode::INPLACE_ADD:
            case ByteCode::BINARY_ADD:
                v = POP();
                w = POP();
                PUSH(w->add(v));
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
                    args = new HiList();
                    for (int i = 0; i < t->length(); i++) {
                        args->append(t->get(i));
                    }
                }
                fo->set_default(args);
                PUSH(fo);

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

            case ByteCode::CALL_METHOD:
            case ByteCode::CALL_FUNCTION:
                if (op_arg > 0) {
                    args = new HiList();
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }

                fo = static_cast<FunctionObject*>(POP());
                build_frame(fo, args);

                break;

            case ByteCode::CALL_FUNCTION_KW:
                assert(op_arg > 0);
                arg_cnt = op_arg;
                kwargs = POP()->as<HiList>();
                args = new HiList();
                while (arg_cnt--) {
                    args->set(arg_cnt, POP());
                }

                fo = static_cast<FunctionObject*>(POP());
                build_frame(fo, args, kwargs);

                break;

            case ByteCode::RETURN_VALUE:
                leave_frame(POP());
                if (!_frame)
                    return;
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
                lst = new HiList();
                while (op_arg--) {
                    lst->set(op_arg, POP());
                }
                PUSH(lst);
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
                w = v->getattr(StringTable::get_instance()->next_str);
                build_frame(w, NULL);

                if (TOP() == NULL) {
                    _frame->_pc += op_arg;
                    POP();
                }
                break;

            case ByteCode::BUILD_MAP:
                v = new HiDict();
                for (int i = 0; i < op_arg; i++) {
                    ((HiDict*)v)->put(POP(), POP());
                }
                PUSH(v);
                break;
                
            case ByteCode::BUILD_CONST_KEY_MAP:
                lst = (HiList*)POP();

                v = new HiDict();
                for (int i = 0; i < op_arg; i++) {
                    ((HiDict*)v)->put(lst->get(op_arg - i - 1), POP());
                }

                PUSH(v);
                break;

            default:
                printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
}
