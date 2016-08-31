重複行を削除（ジェネレータに通す前の辞書のメンテ向け）
・ソートはしない
・オンメモリの為、巨大なファイルの処理は不能

UniqLineFilter

  usage: worddictgen [option]
  
  required
    none
  
  option
    -? -h                        // Help
    -o <output word list file>   // (default stdout)
    -i <input word list file>    // (default stdin)
    -d                           // Delete empty line



今のところVS2015用、しかもマルチバイト専用
Linux BSD 辺りにhogeろうと思ったんだけどUTFどうしようでそのまま
入出力ファイルの指定を無視すると標準入出力を使う
標準出力は Win の仕様上 CRLF、-o で出力すると LF

>>>
hoge
fuga

hoge
piyo
>>>

<<<
hoge
fuga

piyo
<<<

-d があると空行は出力しない

