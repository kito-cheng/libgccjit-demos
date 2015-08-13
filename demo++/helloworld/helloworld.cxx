#include <libgccjit++.h>
#include <vector>

#include <assert.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
  gccjit::context ctxt;
  gcc_jit_result *result;
  int ch;

  ctxt = gccjit::context::acquire ();

  gccjit::type void_type =
    ctxt.get_type (GCC_JIT_TYPE_VOID);

  gccjit::type int_type =
    ctxt.get_type (GCC_JIT_TYPE_INT);

  std::vector<gccjit::param> helloworld_params;
  gccjit::function func =
    ctxt.new_function (GCC_JIT_FUNCTION_EXPORTED,
                       void_type,
                       "helloworld",
                       helloworld_params,
                       0);

  gccjit::type const_char_ptr_type =
    ctxt.get_type (GCC_JIT_TYPE_CONST_CHAR_PTR);

  gccjit::param param_format =
    ctxt.new_param (const_char_ptr_type, "format");
  std::vector<gccjit::param> printf_params;
  gccjit::function printf_func =
    ctxt.new_function (GCC_JIT_FUNCTION_IMPORTED,
                       int_type,
                       "printf",
                       printf_params,
                       1);

  gccjit::rvalue arg =
    ctxt.new_rvalue ("Hello World\n");

  gccjit::block curblock =
    func.new_block ();

  std::vector<gccjit::rvalue> args;
  args.push_back (arg);
  gccjit::rvalue call =
    ctxt.new_call (printf_func, args);

  curblock.add_eval (call);

  curblock.end_with_return ();

  result = ctxt.compile();

  if (!result) {
    fprintf (stderr, "NULL result");
    exit (1);
  }


  typedef void (*fn_type) ();
  fn_type helloworld =
    (fn_type)gcc_jit_result_get_code (result, "helloworld");

  helloworld ();

  ctxt.release ();
  gcc_jit_result_release (result);

  return 0;
}
