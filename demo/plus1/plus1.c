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

  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);

  gcc_jit_param *n =
    gcc_jit_context_new_param (ctxt, NULL, int_type, "n");

  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt, NULL,
                                  GCC_JIT_FUNCTION_EXPORTED,
                                  int_type,
                                  "plus1",
                                  1, &n,
                                  0);

  gcc_jit_block *curblock =
    gcc_jit_function_new_block (func, NULL);

  gcc_jit_rvalue *int_one = gcc_jit_context_new_rvalue_from_int (ctxt, int_type, 1);

  gcc_jit_block_end_with_return (
    curblock,
    NULL,
    gcc_jit_context_new_binary_op (
      ctxt,
      NULL,
      GCC_JIT_BINARY_OP_PLUS,
      int_type,
      gcc_jit_param_as_rvalue (n),
      int_one) );

  result = gcc_jit_context_compile (ctxt);

  if (!result) {
    fprintf (stderr, "NULL result");
    exit (1);
  }


  typedef int (*fn_type) (int);
  fn_type plus1 =
    (fn_type)gcc_jit_result_get_code (result, "plus1");

  int input;
  scanf ("%d", &input);
  int val = plus1 (input);
  printf ("%d\n", val);

  gcc_jit_context_release (ctxt);
  gcc_jit_result_release (result);

  return 0;
}
