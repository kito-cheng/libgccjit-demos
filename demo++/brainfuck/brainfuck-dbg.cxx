#include <libgccjit++.h>
#include <assert.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>

struct loop_t {
  gccjit::block cond;
  gccjit::block body;
  gccjit::block exit;
};

int main(int argc, const char *argv[]) {
  gccjit::context ctxt;
  gccjit::location loc;
  gcc_jit_result *result;
  int ch, line;

  struct loop_t loops[1024];
  int loop_level = 0;

  FILE *fp = fopen(argv[1], "r");

  ctxt = gccjit::context::acquire ();

  ctxt.set_bool_option (GCC_JIT_BOOL_OPTION_DEBUGINFO, 1);

  /* Declare type for `int`.  */
  gccjit::type int_type =
    ctxt.get_type (GCC_JIT_TYPE_INT);

  /* Declare type for `int[30000]`.  */
  gccjit::type int_arr_type =
    ctxt.new_array_type (int_type, 30000);

  /* Declare type for void.  */
  gccjit::type void_type =
    ctxt.get_type (GCC_JIT_TYPE_VOID);

  std::vector<gccjit::param> bf_params;
  /* Declare a function `void brainfuck()`.  */
  gccjit::function func =
    ctxt.new_function (
      GCC_JIT_FUNCTION_EXPORTED,
      void_type,
      "brainfuck",
      bf_params,
      0);

  /* Declare a static global variable `data` of type int[30000].  */
  gccjit::lvalue data =
    ctxt.new_global (GCC_JIT_GLOBAL_INTERNAL, int_arr_type, "data");

  /* Declare a static global variable `idx` of type int.  */
  gccjit::lvalue idx =
    ctxt.new_global (GCC_JIT_GLOBAL_INTERNAL, int_type, "idx");

  gccjit::block curblock =
    func.new_block ();

  /* Create a constant value 1 with type int.  */
  gccjit::rvalue int_one = ctxt.new_rvalue (int_type, 1);

  /* Create a constant value 0 with type int.  */
  gccjit::rvalue int_zero = ctxt.new_rvalue (int_type, 0);

  /* Declare a function `int putchar(int c)`.  */
  gccjit::param param_c =
    ctxt.new_param (int_type, "c");
  std::vector<gccjit::param> putchar_params;
  putchar_params.push_back (param_c);
  gccjit::function func_putchar =
    ctxt.new_function (
      GCC_JIT_FUNCTION_IMPORTED,
      int_type,
      "putchar",
      putchar_params,
      0);

  /* Declare a function `int getchar()`.  */
  std::vector<gccjit::param> getchar_params;
  gccjit::function func_getchar =
    ctxt.new_function (
      GCC_JIT_FUNCTION_IMPORTED,
      int_type,
      "getchar",
      getchar_params,
      0);

  loc = ctxt.new_location(argv[1], 1, 0);
  line = 0;
  while ((ch = fgetc(fp)) != EOF) {
    switch (ch) {
      case '>':
        /* Append `idx += 1` to curblock.  */
        curblock.add_assignment_op (idx, GCC_JIT_BINARY_OP_PLUS, int_one, loc);
        break;
      case '<':
        /* Append `idx -= 1` to curblock.  */
        curblock.add_assignment_op (idx, GCC_JIT_BINARY_OP_MINUS, int_one, loc);
        break;
      case '+':
        /* Append `data[idx] += 1` to curblock.  */
        curblock.add_assignment_op (
          ctxt.new_array_access (data, idx),
          GCC_JIT_BINARY_OP_PLUS,
          int_one,
          loc);
        break;
      case '-':
        /* Append `data[idx] -= 1` to curblock.  */
        curblock.add_assignment_op (
          ctxt.new_array_access (data, idx),
          GCC_JIT_BINARY_OP_MINUS,
          int_one,
          loc);
        break;
      case '.':
        {
          /* Append `putchar(data[idx])` to curblock.  */
          gccjit::rvalue arg =
            ctxt.new_array_access (data, idx);
          std::vector<gccjit::rvalue> args;
          args.push_back (arg);
          gccjit::rvalue call =
            ctxt.new_call (func_putchar, args);
          curblock.add_eval (call, loc);
        }
        break;
      case ',':
        {
          /* Append `data[idx] = getchar()` to curblock.  */
          std::vector<gccjit::rvalue> args;
          gccjit::rvalue call =
            ctxt.new_call (func_getchar, args);
          curblock.add_assignment (
            ctxt.new_array_access (data, idx),
            call,
            loc);
        }
        break;
      case '[':
        {
          gccjit::block cond = func.new_block();
          gccjit::block body = func.new_block();
          gccjit::block exit = func.new_block();

          curblock.end_with_jump (cond);

          cond.end_with_conditional(
            ctxt.new_eq(
              ctxt.new_array_access (data, idx),
              int_zero),
            exit,
            body
          );

          loops[loop_level].cond = cond;
          loops[loop_level].body = body;
          loops[loop_level].exit = exit;
          loop_level++;
          curblock = body;
        }
        break;
      case ']':
        {
          loop_level--;
          curblock.end_with_jump (loops[loop_level].cond);
          curblock = loops[loop_level].exit;
        }
        break;
      case '\n':
        line++;
        loc = ctxt.new_location(argv[1], line, 0);
        break;;
      default:
        break;
    }
  }
  /* Append `return` to curblock.  */
  curblock.end_with_return ();

  /* Let's compile!  */
  result = ctxt.compile();

  if (!result) {
    fprintf (stderr, "NULL result");
    exit (1);
  }

  /* Get brainf*ck just like dlsym.  */
  typedef void (*fn_type) (void);
  fn_type brainfuck =
    (fn_type)gcc_jit_result_get_code (result, "brainfuck");

  /* brainf*ck!  */
  brainfuck ();

  /* Release resource.  */
  fclose (fp);
  ctxt.release();
  gcc_jit_result_release (result);

  return 0;
}
