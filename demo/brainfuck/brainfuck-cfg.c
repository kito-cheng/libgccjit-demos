#include <libgccjit.h>
#include <assert.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>

struct loop_t {
  gcc_jit_block *cond;
  gcc_jit_block *exit;
};

int main(int argc, const char *argv[]) {
  gcc_jit_context *ctxt;
  gcc_jit_result *result;
  int ch;

  struct loop_t loops[1024];
  int loop_level = 0;

  FILE *fp = fopen(argv[1], "r");

  ctxt = gcc_jit_context_acquire ();
  if (!ctxt) {
    printf ("can't get gcc context");
    exit (1);
  }

  /* Declare type for `int`.  */
  gcc_jit_type *int_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_INT);

  /* Declare type for `int[30000]`.  */
  gcc_jit_type *int_arr_type =
    gcc_jit_context_new_array_type (ctxt,
                                    NULL,
                                    int_type,
                                    30000);

  /* Declare type for void.  */
  gcc_jit_type *void_type =
    gcc_jit_context_get_type (ctxt, GCC_JIT_TYPE_VOID);

  /* Declare a function `void brainfuck()`.  */
  gcc_jit_function *func =
    gcc_jit_context_new_function (ctxt, NULL,
                                  GCC_JIT_FUNCTION_EXPORTED,
                                  void_type,
                                  "brainfuck",
                                  0, NULL,
                                  0);

  /* Declare a static global variable `data` of type int[30000].  */
  gcc_jit_lvalue *data =
    gcc_jit_context_new_global (
      ctxt,
      NULL,
      GCC_JIT_GLOBAL_INTERNAL,
      int_arr_type,
      "data");

  /* Declare a static global variable `idx` of type int.  */
  gcc_jit_lvalue *idx =
    gcc_jit_context_new_global (
      ctxt,
      NULL,
      GCC_JIT_GLOBAL_INTERNAL,
      int_type,
      "idx");

  gcc_jit_block *curblock =
    gcc_jit_function_new_block (func, NULL);

  /* Create a constant value 1 with type int.  */
  gcc_jit_rvalue *int_one = gcc_jit_context_one (ctxt, int_type);

  /* Create a constant value 0 with type int.  */
  gcc_jit_rvalue *int_zero = gcc_jit_context_zero (ctxt, int_type);

  /* Declare a function `int putchar(int c)`.  */
  gcc_jit_param *param_c =
    gcc_jit_context_new_param (ctxt, NULL, int_type, "c");
  gcc_jit_function *func_putchar =
    gcc_jit_context_new_function (ctxt, NULL,
                                  GCC_JIT_FUNCTION_IMPORTED,
                                  int_type,
                                  "putchar",
                                  1, &param_c,
                                  0);
  /* Declare a function `int getchar()`.  */
  gcc_jit_function *func_getchar =
    gcc_jit_context_new_function (ctxt, NULL,
                                  GCC_JIT_FUNCTION_IMPORTED,
                                  int_type,
                                  "getchar",
                                  0, NULL,
                                  0);

  while ((ch = fgetc(fp)) != EOF) {
    switch (ch) {
      case '>':
        /* Append `idx += 1` to curblock.  */
        gcc_jit_block_add_assignment_op (curblock,
                                         NULL,
                                         idx,
                                         GCC_JIT_BINARY_OP_PLUS,
                                         int_one);

        break;
      case '<':
        /* Append `idx -= 1` to curblock.  */
        gcc_jit_block_add_assignment_op (curblock,
                                         NULL,
                                         idx,
                                         GCC_JIT_BINARY_OP_MINUS,
                                         int_one);
        break;
      case '+':
        /* Append `data[idx] += 1` to curblock.  */
        gcc_jit_block_add_assignment_op (
          curblock,
          NULL,
          gcc_jit_context_new_array_access (
            ctxt,
            NULL,
            gcc_jit_lvalue_as_rvalue (data),
            gcc_jit_lvalue_as_rvalue (idx)),
          GCC_JIT_BINARY_OP_PLUS,
          int_one);
        break;
      case '-':
        /* Append `data[idx] -= 1` to curblock.  */
        gcc_jit_block_add_assignment_op (
          curblock,
          NULL,
          gcc_jit_context_new_array_access (
            ctxt,
            NULL,
            gcc_jit_lvalue_as_rvalue (data),
            gcc_jit_lvalue_as_rvalue (idx)),
          GCC_JIT_BINARY_OP_MINUS,
          int_one);
        break;
      case '.':
        {
          /* Append `putchar(data[idx])` to curblock.  */
          gcc_jit_rvalue *arg =
            gcc_jit_lvalue_as_rvalue (
              gcc_jit_context_new_array_access (
                ctxt,
                NULL,
                gcc_jit_lvalue_as_rvalue (data),
                gcc_jit_lvalue_as_rvalue (idx)));
          gcc_jit_rvalue *call =
            gcc_jit_context_new_call (ctxt,
                                      NULL,
                                      func_putchar,
                                      1, &arg);
          gcc_jit_block_add_eval (curblock, NULL, call);
        }
        break;
      case ',':
        {
          /* Append `data[idx] = getchar()` to curblock.  */
          gcc_jit_rvalue *call =
            gcc_jit_context_new_call (ctxt,
                                      NULL,
                                      func_getchar,
                                      0, NULL);
          gcc_jit_block_add_assignment (
            curblock,
            NULL,
            gcc_jit_context_new_array_access (
              ctxt,
              NULL,
              gcc_jit_lvalue_as_rvalue (data),
              gcc_jit_lvalue_as_rvalue (idx)),
            call
          );
        }
        break;
      case '[':
        {
          gcc_jit_block *cond = gcc_jit_function_new_block (func, NULL);
          gcc_jit_block *body = gcc_jit_function_new_block (func, NULL);
          gcc_jit_block *exit = gcc_jit_function_new_block (func, NULL);

          gcc_jit_block_end_with_jump (
            curblock,
            NULL,
            cond);

          gcc_jit_block_end_with_conditional (
            cond,
            NULL,
            gcc_jit_context_new_comparison (
              ctxt,
              NULL,
              GCC_JIT_COMPARISON_EQ,
              gcc_jit_lvalue_as_rvalue (
                gcc_jit_context_new_array_access (
                  ctxt,
                  NULL,
                  gcc_jit_lvalue_as_rvalue (data),
                  gcc_jit_lvalue_as_rvalue (idx))),
              int_zero),
            exit,
            body
          );

          loops[loop_level].cond = cond;
          loops[loop_level].exit = exit;
          loop_level++;
          curblock = body;
        }
        break;
      case ']':
        {
          loop_level--;
          gcc_jit_block_end_with_jump (
            curblock,
            NULL,
            loops[loop_level].cond);
          curblock = loops[loop_level].exit;
        }
        break;
      default:
        break;
    }
  }
  /* Append `return` to curblock.  */
  gcc_jit_block_end_with_void_return (curblock, NULL);

  gcc_jit_function_dump_to_dot (func, "cfg.dot");

  /* Let's compile!  */
  result = gcc_jit_context_compile (ctxt);

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
  gcc_jit_context_release (ctxt);
  gcc_jit_result_release (result);

  return 0;
}
