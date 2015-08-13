#include <libgccjit++.h>
#include <vector>

int main() {
  gccjit::context ctxt;
  gcc_jit_result *result;

  ctxt = gccjit::context::acquire ();

  gccjit::type void_type =
    ctxt.get_type (GCC_JIT_TYPE_VOID);

  std::vector<gccjit::param> params;
  gccjit::function func =
    ctxt.new_function(GCC_JIT_FUNCTION_EXPORTED,
                      void_type,
                      "empty",
                      params,
                      0);

  gccjit::block curblock =
    func.new_block();

  curblock.end_with_return();

  result = ctxt.compile();

  typedef void (*fn_type) ();
  fn_type empty =
    (fn_type)gcc_jit_result_get_code (result, "empty");

  empty();

  ctxt.release ();
  gcc_jit_result_release (result);

  return 0;
}
