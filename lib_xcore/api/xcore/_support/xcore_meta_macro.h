// Copyright (c) 2019-2020, XMOS Ltd, All rights reserved
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define _XCORE_SHIM(_FLM, ...) _FLM(__VA_ARGS__)
#define _XCORE_UNIQUE_LABEL_I(_BNAME, _CNT) _BNAME ## _CNT
#define _XCORE_UNIQUE_LABEL(_BNAME) _XCORE_SHIM(_XCORE_UNIQUE_LABEL_I, _BNAME, __COUNTER__)
#define _XCORE_STRINGIFY_I(...) #__VA_ARGS__
#define _XCORE_STRINGIFY(_S) _XCORE_STRINGIFY_I(_S)

#define _XCORE_SEP__XCORE_SEP_COMMA ,
#define _XCORE_SEP__XCORE_SEP_SEMICOLON ;
#define _XCORE_SEP__XCORE_SEP_NONE

#define _XCORE_APPLY00(_F, _SEP)
#define _XCORE_APPLY01(_F, _SEP, _A0, ...) _F(_A0)                   _XCORE_APPLY00(_F, _SEP, ## __VA_ARGS__)
#define _XCORE_APPLY02(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY01(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY03(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY02(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY04(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY03(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY05(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY04(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY06(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY05(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY07(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY06(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY08(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY07(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY09(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY08(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY10(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY09(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY11(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY10(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY12(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY11(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY13(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY12(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY14(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY13(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY15(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY14(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY16(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY15(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY17(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY16(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY18(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY17(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY19(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY18(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY20(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY19(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY21(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY20(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY22(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY21(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY23(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY22(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY24(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY23(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY25(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY24(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY26(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY25(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY27(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY26(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY28(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY27(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY29(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY28(_F, _SEP, __VA_ARGS__)
#define _XCORE_APPLY30(_F, _SEP, _A0, ...) _F(_A0) _XCORE_SEP_##_SEP _XCORE_APPLY29(_F, _SEP, __VA_ARGS__)

#define _XCORE_GET30(_I00_,  _I01_,  _I02_,  _I03_,  _I04_,  _T05_,  _T06_,  _T07_,  _T08_,  _T09_, \
                     _I10_,  _I11_,  _I12_,  _I13_,  _I14_,  _T15_,  _T16_,  _T17_,  _T18_,  _T19_, \
                     _I20_,  _I21_,  _I22_,  _I23_,  _I24_,  _T25_,  _T26_,  _T27_,  _T28_,  _T29_, \
                     _N, ...) _N

#define _XCORE_APPLICATOR(...) \
  _XCORE_GET30(__VA_ARGS__, _XCORE_APPLY30, \
    _XCORE_APPLY29, _XCORE_APPLY28, _XCORE_APPLY27, _XCORE_APPLY26, _XCORE_APPLY25, \
    _XCORE_APPLY24, _XCORE_APPLY23, _XCORE_APPLY22, _XCORE_APPLY21, _XCORE_APPLY20, \
    _XCORE_APPLY19, _XCORE_APPLY18, _XCORE_APPLY17, _XCORE_APPLY16, _XCORE_APPLY15, \
    _XCORE_APPLY14, _XCORE_APPLY13, _XCORE_APPLY12, _XCORE_APPLY11, _XCORE_APPLY10, \
    _XCORE_APPLY09, _XCORE_APPLY08, _XCORE_APPLY07, _XCORE_APPLY06, _XCORE_APPLY05, \
    _XCORE_APPLY04, _XCORE_APPLY03, _XCORE_APPLY02, _XCORE_APPLY01, _XCORE_APPLY00)

#define _XCORE_APPLY_II(_F, _SEP, ...) _XCORE_APPLICATOR(__VA_ARGS__)(_F, _SEP, ## __VA_ARGS__)
#define _XCORE_APPLY_I(...) _XCORE_APPLY_II(__VA_ARGS__)
#define _XCORE_APPLY(_F, ...) _XCORE_APPLY_I(_F, _XCORE_SEP_COMMA, ## __VA_ARGS__)
#define _XCORE_APPLY_NOSEP(_F, ...) _XCORE_APPLY_I(_F, _XCORE_SEP_NONE, ## __VA_ARGS__)
#define _XCORE_LABELADDR(_LABEL) &&_LABEL

#define _XCORE_DEPAREN(...) __VA_ARGS__
#define _XCORE_UNPACK(_PACK) _XCORE_DEPAREN _PACK
#define _XCORE_PSHIM_I(_FLM, _PACK, ...) _XCORE_SHIM(_FLM, _XCORE_UNPACK(_PACK), ## __VA_ARGS__)
#define _XCORE_PSHIM(...) _XCORE_PSHIM_I(__VA_ARGS__)
#define _XCORE_PSHIM_NV(_FLM, _PACK) _XCORE_SHIM(_XCORE_SHIM, _FLM, _XCORE_UNPACK(_PACK))

#define _XCORE_TAIL(_H_, ...) __VA_ARGS__
#define _XCORE_HEAD(_H, ...) _H

#define _XCORE_I(...) __VA_ARGS__
#define _XCORE_PACK_JOIN(_LHS, _RHS) (_XCORE_DEPAREN _LHS, _XCORE_DEPAREN _RHS)



#define _XCORE_TAG00(_T_)
#define _XCORE_TAG01(_T, _V, ...) (_T,_V)  _XCORE_TAG00(_T, ## __VA_ARGS__)
#define _XCORE_TAG02(_T, _V, ...) (_T,_V), _XCORE_TAG01(_T, __VA_ARGS__)
#define _XCORE_TAG03(_T, _V, ...) (_T,_V), _XCORE_TAG02(_T, __VA_ARGS__)
#define _XCORE_TAG04(_T, _V, ...) (_T,_V), _XCORE_TAG03(_T, __VA_ARGS__)
#define _XCORE_TAG05(_T, _V, ...) (_T,_V), _XCORE_TAG04(_T, __VA_ARGS__)
#define _XCORE_TAG06(_T, _V, ...) (_T,_V), _XCORE_TAG05(_T, __VA_ARGS__)
#define _XCORE_TAG07(_T, _V, ...) (_T,_V), _XCORE_TAG06(_T, __VA_ARGS__)
#define _XCORE_TAG08(_T, _V, ...) (_T,_V), _XCORE_TAG07(_T, __VA_ARGS__)
#define _XCORE_TAG09(_T, _V, ...) (_T,_V), _XCORE_TAG08(_T, __VA_ARGS__)
#define _XCORE_TAG10(_T, _V, ...) (_T,_V), _XCORE_TAG09(_T, __VA_ARGS__)
#define _XCORE_TAG11(_T, _V, ...) (_T,_V), _XCORE_TAG10(_T, __VA_ARGS__)
#define _XCORE_TAG12(_T, _V, ...) (_T,_V), _XCORE_TAG11(_T, __VA_ARGS__)
#define _XCORE_TAG13(_T, _V, ...) (_T,_V), _XCORE_TAG12(_T, __VA_ARGS__)
#define _XCORE_TAG14(_T, _V, ...) (_T,_V), _XCORE_TAG13(_T, __VA_ARGS__)
#define _XCORE_TAG15(_T, _V, ...) (_T,_V), _XCORE_TAG14(_T, __VA_ARGS__)
#define _XCORE_TAG16(_T, _V, ...) (_T,_V), _XCORE_TAG15(_T, __VA_ARGS__)
#define _XCORE_TAG17(_T, _V, ...) (_T,_V), _XCORE_TAG16(_T, __VA_ARGS__)
#define _XCORE_TAG18(_T, _V, ...) (_T,_V), _XCORE_TAG17(_T, __VA_ARGS__)
#define _XCORE_TAG19(_T, _V, ...) (_T,_V), _XCORE_TAG18(_T, __VA_ARGS__)
#define _XCORE_TAG20(_T, _V, ...) (_T,_V), _XCORE_TAG19(_T, __VA_ARGS__)
#define _XCORE_TAG21(_T, _V, ...) (_T,_V), _XCORE_TAG20(_T, __VA_ARGS__)
#define _XCORE_TAG22(_T, _V, ...) (_T,_V), _XCORE_TAG21(_T, __VA_ARGS__)
#define _XCORE_TAG23(_T, _V, ...) (_T,_V), _XCORE_TAG22(_T, __VA_ARGS__)
#define _XCORE_TAG24(_T, _V, ...) (_T,_V), _XCORE_TAG23(_T, __VA_ARGS__)
#define _XCORE_TAG25(_T, _V, ...) (_T,_V), _XCORE_TAG24(_T, __VA_ARGS__)
#define _XCORE_TAG26(_T, _V, ...) (_T,_V), _XCORE_TAG25(_T, __VA_ARGS__)
#define _XCORE_TAG27(_T, _V, ...) (_T,_V), _XCORE_TAG26(_T, __VA_ARGS__)
#define _XCORE_TAG28(_T, _V, ...) (_T,_V), _XCORE_TAG27(_T, __VA_ARGS__)
#define _XCORE_TAG29(_T, _V, ...) (_T,_V), _XCORE_TAG28(_T, __VA_ARGS__)
#define _XCORE_TAG30(_T, _V, ...) (_T,_V), _XCORE_TAG29(_T, __VA_ARGS__)

#define _XCORE_TAGGER(...) \
  _XCORE_GET30(__VA_ARGS__, _XCORE_TAG30, \
    _XCORE_TAG29, _XCORE_TAG28, _XCORE_TAG27, _XCORE_TAG26, _XCORE_TAG25, \
    _XCORE_TAG24, _XCORE_TAG23, _XCORE_TAG22, _XCORE_TAG21, _XCORE_TAG20, \
    _XCORE_TAG19, _XCORE_TAG18, _XCORE_TAG17, _XCORE_TAG16, _XCORE_TAG15, \
    _XCORE_TAG14, _XCORE_TAG13, _XCORE_TAG12, _XCORE_TAG11, _XCORE_TAG10, \
    _XCORE_TAG09, _XCORE_TAG08, _XCORE_TAG07, _XCORE_TAG06, _XCORE_TAG05, \
    _XCORE_TAG04, _XCORE_TAG03, _XCORE_TAG02, _XCORE_TAG01, _XCORE_TAG00)

#define _XCORE_TAG_I(_T, ...)  _XCORE_TAGGER(__VA_ARGS__)(_T, ## __VA_ARGS__)
#define _XCORE_TAG(...) _XCORE_TAG_I(__VA_ARGS__)

#define _XCORE_PACK(...) (__VA_ARGS__)

#define _XCORE_COUNT30(...) \
  _XCORE_GET30(__VA_ARGS__, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9,  8,  7,  6,  5,  4,  3,  2,  1,  0)

#ifdef __cplusplus
}
#endif

