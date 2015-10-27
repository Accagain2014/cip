# cip
  A Compressing File Tool Using Huffman Coding 

# run step
1 编译
```
g++ -o cip main.cpp
```
2 压缩
```
  ./cip sourcefile compressedfile
  eg ./cip Aesop_Fables.txt Aesop_Fables_compress.txt
```

3 解压
```
  ./cip compressedfile targetfile
  eg ./main Aesop_Fables_compress.txt Aesop_Fables_copy.txt
```

# compression ratio
  For the Aesop_Fables.txt file is 109KB/190KB = 57.37%

  For the graph_compress.txt file is 931KB/2.1M = 43.25%

