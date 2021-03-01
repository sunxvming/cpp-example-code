#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>

#define __INTRINSIC_NAME(LBL, SUF) LBL##SUF
#define _INTRINSIC_NAME(LBL, SUF) __INTRINSIC_NAME(LBL, SUF)
#define REGISTER_WASM_VM_INTRINSIC(CLS, MOD, METHOD, NAME) \
   vm::registered_function<wasm::wasm_context_interface, CLS, &CLS::METHOD> _INTRINSIC_NAME(__wasm_vm_intrinsic_fn, __COUNTER__)(std::string(#MOD), std::string(#NAME));





REGISTER_WASM_VM_INTRINSIC(wasm_host_methods, env, abort,                   abort)
REGISTER_WASM_VM_INTRINSIC(wasm_host_methods, env, wasm_exit,               wasm_exit)
REGISTER_WASM_VM_INTRINSIC(wasm_host_methods, env, wasm_assert,             wasm_assert)
REGISTER_WASM_VM_INTRINSIC(wasm_host_methods, env, wasm_assert_code,        wasm_assert_code)
REGISTER_WASM_VM_INTRINSIC(wasm_host_methods, env, current_time,            current_time)