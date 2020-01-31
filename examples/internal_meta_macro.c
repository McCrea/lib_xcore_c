#include <stdio.h>
#include <xcore/_support/xcore_meta_macro.h>

#define ONE_TO_THIRTY 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
#define STRING_TRAILING_SPACE(X) _XCORE_STRINGIFY(X)" "
#define ALWAYS_AN_INT_FORMATTER(_X) "%d "

#define ADD_TWO(A, B) A + B
#define ADD_PACK_OF_TWO(PACK) _XCORE_PSHIM(ADD_TWO,PACK)


int main(void)
{
  puts(_XCORE_APPLY_NOSEP(_XCORE_STRINGIFY, one, two three));
  puts(_XCORE_APPLY_NOSEP(STRING_TRAILING_SPACE, ONE_TO_THIRTY));
  printf(_XCORE_APPLY_NOSEP(ALWAYS_AN_INT_FORMATTER, ONE_TO_THIRTY) "\n",
         _XCORE_APPLY(ADD_PACK_OF_TWO, _XCORE_TAG(100, ONE_TO_THIRTY)));
}

