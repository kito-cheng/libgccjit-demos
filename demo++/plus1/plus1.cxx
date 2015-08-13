#include <libgccjit++.h>
#include <assert.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
  gccjit::context ctxt;
  gcc_jit_result *result;
  int ch;

  ctxt = gccjit::context::acquire ();

  gccjit::type int_type = ctxt.get_type (GCC_JIT_TYPE_INT);

  gccjit::param n =
    ctxt.new_param (int_type, "n");

  std::vector<gccjit::param> plus1_params;
  plus1_params.push_back(n);
  gccjit::function func =
    ctxt.new_function (GCC_JIT_FUNCTION_EXPORTED,
                       int_type,
                       "plus1",
                       plus1_params,
                       0);

  gccjit::block curblock =
    func.new_block ();

  gccjit::rvalue int_one = ctxt.new_rvalue (int_type, 1);

  curblock.end_with_return (
    ctxt.new_plus (int_type, n, int_one));

  result = ctxt.compile ();

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

  ctxt.release ();
  gcc_jit_result_release (result);

  return 0;
}
