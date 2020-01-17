#pragma once

#define _XMM_SHIM(FLM, ...) FLM(__VA_ARGS__)
#define _XMM_UNIQUE_LABEL_I(BNAME, CNT) BNAME ## CNT
#define _XMM_UNIQUE_LABEL(BNAME) _XMM_SHIM(_XMM_UNIQUE_LABEL_I, BNAME, __COUNTER__)
#define _XMM_STRINGIFY_I(S) #S
#define _XMM_STRINGIFY(S) _XMM_STRINGIFY_I(S)

#define _XMM_SEP__XMM_SEP_COMMA ,
#define _XMM_SEP__XMM_SEP_SEMICOLON ;
#define _XMM_SEP__XMM_SEP_NONE

#define _XMM_APPLY0(F, SEP)
#define _XMM_APPLY1(F, SEP, A0, ...) F(A0) _XMM_APPLY0(F, SEP, ## __VA_ARGS__)
#define _XMM_APPLY2(F, SEP, A0, ...) F(A0) _XMM_SEP_##SEP _XMM_APPLY1(F, SEP, __VA_ARGS__)
#define _XMM_APPLY3(F, SEP, A0, ...) F(A0) _XMM_SEP_##SEP _XMM_APPLY2(F, SEP, __VA_ARGS__)
#define _XMM_APPLY4(F, SEP, A0, ...) F(A0) _XMM_SEP_##SEP _XMM_APPLY3(F, SEP, __VA_ARGS__)
#define _XMM_APPLY5(F, SEP, A0, ...) F(A0) _XMM_SEP_##SEP _XMM_APPLY4(F, SEP, __VA_ARGS__)
#define _XMM_APPLY6(F, SEP, A0, ...) F(A0) _XMM_SEP_##SEP _XMM_APPLY5(F, SEP, __VA_ARGS__)
#define _XMM_APPLY7(F, SEP, A0, ...) F(A0) _XMM_SEP_##SEP _XMM_APPLY6(F, SEP, __VA_ARGS__)
#define _XMM_APPLY8(F, SEP, A0, ...) F(A0) _XMM_SEP_##SEP _XMM_APPLY7(F, SEP, __VA_ARGS__)
#define _XMM_APPLY9(F, SEP, A0, ...) F(A0) _XMM_SEP_##SEP _XMM_APPLY8(F, SEP, __VA_ARGS__)
#define _XMM_APPLY10(F, SEP, A0, ...) F(A0) _XMM_SEP_##SEP _XMM_APPLY9(F, SEP, __VA_ARGS__)

#define _XMM_GET10(I0, I1, I2, I3, I4, T5, T6, T7, T8, T9, N, ...) N
#define _XMM_APPLICATOR(...) _XMM_GET10(__VA_ARGS__, _XMM_APPLY10, _XMM_APPLY9, _XMM_APPLY8, _XMM_APPLY7, _XMM_APPLY6, _XMM_APPLY5, _XMM_APPLY4, _XMM_APPLY3, _XMM_APPLY2, _XMM_APPLY1, _XMM_APPLY0)

#define _XMM_APPLY_I(F, SEP, ...) _XMM_APPLICATOR(__VA_ARGS__)(F, SEP, ## __VA_ARGS__)
#define _XMM_APPLY(F, ...) _XMM_APPLY_I(F, _XMM_SEP_COMMA, ## __VA_ARGS__)
#define _XMM_APPLY_NOSEP(F, ...) _XMM_APPLY_I(F, _XMM_SEP_NONE, ## __VA_ARGS__)
#define _XMM_LABELADDR(LABEL) &&LABEL

#define _XMM_DEPAREN(...) __VA_ARGS__
#define _XMM_UNPACK(PACK) _XMM_DEPAREN PACK
#define _XMM_PSHIM(FLM, PACK, ...) _XMM_SHIM(FLM, _XMM_UNPACK(PACK), ## __VA_ARGS__)
#define _XMM_PSHIM_NV(FLM, PACK) _XMM_SHIM(_XMM_SHIM, FLM, _XMM_UNPACK(PACK))

#define _XMM_TAIL(H_, ...) __VA_ARGS__
#define _XMM_HEAD(H, ...) H

#define _XMM_I(...) __VA_ARGS__
#define _XMM_PACK_JOIN(LHS, RHS) (_XMM_DEPAREN LHS, _XMM_DEPAREN RHS)



#define _XMM_TAG0(T_)
#define _XMM_TAG1(T, V, ...) (T,V) _XMM_TAG0(T, ## __VA_ARGS__)
#define _XMM_TAG2(T, V, ...) (T,V), _XMM_TAG1(T, __VA_ARGS__)
#define _XMM_TAG3(T, V, ...) (T,V), _XMM_TAG2(T, __VA_ARGS__)
#define _XMM_TAG4(T, V, ...) (T,V), _XMM_TAG3(T, __VA_ARGS__)
#define _XMM_TAG5(T, V, ...) (T,V), _XMM_TAG4(T, __VA_ARGS__)
#define _XMM_TAG6(T, V, ...) (T,V), _XMM_TAG5(T, __VA_ARGS__)
#define _XMM_TAG7(T, V, ...) (T,V), _XMM_TAG6(T, __VA_ARGS__)
#define _XMM_TAG8(T, V, ...) (T,V), _XMM_TAG7(T, __VA_ARGS__)
#define _XMM_TAG9(T, V, ...) (T,V), _XMM_TAG8(T, __VA_ARGS__)
#define _XMM_TAG10(T, V, ...) (T,V), _XMM_TAG9(T, __VA_ARGS__)

#define _XMM_TAGGER(...) _XMM_GET10(__VA_ARGS__, _XMM_TAG10, _XMM_TAG9, _XMM_TAG8, _XMM_TAG7, _XMM_TAG6, _XMM_TAG5, _XMM_TAG4, _XMM_TAG3, _XMM_TAG2, _XMM_TAG1, _XMM_TAG0)
#define _XMM_TAG(T, ...)  _XMM_TAGGER(__VA_ARGS__)(T, ## __VA_ARGS__)
#define _XMM_PACK(...) (__VA_ARGS__)

#define _XMM_COUNT10(...) _XMM_GET10(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
