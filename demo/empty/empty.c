#include <libgccjit.h>

int main() {
  gcc_jit_context *ctxt;
  gcc_jit_result *result;

  ctxt = gcc_jit_context_acquire ();

  gcc_jit_type *void_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_VOID);

  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt, NULL,
                                  GCC_JIT_FUNCTION_EXPORTED,
                                  void_type,
                                  "empty",
                                  0, NULL,
                                  0);

  gcc_jit_block *curblock =
    gcc_jit_function_new_block (func, NULL);

  gcc_jit_block_end_with_void_return (
    curblock,
    NULL);

  result = gcc_jit_context_compile (ctxt);

  typedef void (*fn_type) ();
  fn_type empty =
    (fn_type)gcc_jit_result_get_code (result, "empty");

  empty();

  gcc_jit_context_release (ctxt);
  gcc_jit_result_release (result);

  return 0;
}
