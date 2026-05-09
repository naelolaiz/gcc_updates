# C++26 (experimental) examples

_Folder: `features/std/cpp26/`. 3 topic(s). Index of examples; build metadata lives in `CMakeLists.txt` next to each `.cpp`. Update by hand when adding new examples._

## Topics

- [language](#language)
- [stl](#stl)
- [templates](#templates)

## language

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp26_contracts_basic.cpp](cpp26_contracts_basic.cpp) | c++26 | 15 | C++26 contracts: pre/post/contract_assert. Implementation status varies; this file is allowed to fail on partial support. *(experimental)* |

## stl

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp26_saturation_arith.cpp](cpp26_saturation_arith.cpp) | c++26 | 14 | <numeric> in C++26 adds saturation arithmetic (add_sat, sub_sat, mul_sat, div_sat) and saturate_cast. |

## templates

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp26_reflection_basic.cpp](cpp26_reflection_basic.cpp) | c++26 | 16 | C++26 static reflection (P2996). API is still settling -- file is experimental. |
