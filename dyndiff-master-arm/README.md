# dyndiff

## ida script

Load a binary from `testbin/`

Use IDA 'File -> Script file...'

Running `func_extract.py` , functions list will be written in `testbin/funclist.json`.

## test run

just run ```make && ./dyndiff```

## coding 规范
1. 不要使用printf, puts，使用include/utils.h里面的LOGINFO, LOGDEBUG等
2.  先写c语言header文件，放到include里面，再写源文件，放到src文件里面
3.  push前先测试
4.  除了testdata文件夹里面不要放二进制文件
5.  写注释或者命名时使用有意义的单词
