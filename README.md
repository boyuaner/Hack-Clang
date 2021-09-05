usage:
```
clang  ~/llvm-project/build/mytest.c -fsyntax-only  -Xclang -load -Xclang ~/llvm-project/build/lib/MuteUndefined.so -Xclang -add-plugin -Xclang MuteUndefined
```