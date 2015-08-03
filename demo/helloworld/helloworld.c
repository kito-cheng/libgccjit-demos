#include <libgccjit.h>
#include <assert.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
  gcc_jit_context *ctxt;
  gcc_jit_result *result;
  int ch;

  ctxt = gcc_jit_context_acquire ();
  if (!ctxt) {
    printf ("can't get gcc context");
    exit (1);
  }

  gcc_jit_type *void_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_VOID);

  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt, NULL,
                                  GCC_JIT_FUNCTION_EXPORTED,
                                  void_type,
                                  "helloworld",
                                  0, NULL,
                                  0);

  gcc_jit_type *const_char_ptr_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_CONST_CHAR_PTR);

  gcc_jit_param *param_format =
    gcc_jit_context_new_param (ctxt, NULL, const_char_ptr_type, "format");
  gcc_jit_function *printf_func =
    gcc_jit_context_new_function (ctxt, NULL,
				  GCC_JIT_FUNCTION_IMPORTED,
				  gcc_jit_context_get_type (
				     ctxt, GCC_JIT_TYPE_INT),
				  "printf",
				  1, &param_format,
				  1);

  gcc_jit_rvalue *args =
    gcc_jit_context_new_string_literal (ctxt, "Hello World\n");

  gcc_jit_block *curblock =
    gcc_jit_function_new_block (func, NULL);

  gcc_jit_rvalue *call =
    gcc_jit_context_new_call (ctxt,
                              NULL,
                              printf_func,
                              1, &args);

  gcc_jit_block_add_eval (
    curblock, NULL, call);

  gcc_jit_block_end_with_void_return (
    curblock,
    NULL);

  result = gcc_jit_context_compile (ctxt);

  if (!result) {
    fprintf (stderr, "NULL result");
    exit (1);
  }


  typedef void (*fn_type) ();
  fn_type helloworld =
    (fn_type)gcc_jit_result_get_code (result, "helloworld");

  helloworld ();

  gcc_jit_context_release (ctxt);
  gcc_jit_result_release (result);

  return 0;
}
