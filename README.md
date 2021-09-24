Sketch library
==============

单源文件的短小程序和脚本。大多数文件按照初学该编程语言时的编译方法就能编译和运行。

<dl>
	<dt>echop</dt>
	<dd>
		<p>具有和<code>echo</code>类似的作用。主要是为了方便检视程序接收到的ARGV。Windows上的<code>echo</code>作为命令提示符的内置功能，语法和调用外部命令不一致；*nix上则可以避免参数意外匹配<code>-n</code>和<code>-e</code>选项。</p>
		<p>在Windows上，不同的编译器还会产生行为不同的程序。Visual C++产生的程序默认不展开通配符，与setargv.obj链接以展开（参照Microsoft文档<a href="https://docs.microsoft.com/zh-cn/cpp/c-language/expanding-wildcard-arguments">《扩展通配符自变量》</a>）；MinGW-w64产生的程序默认展开通配符，与CRT_noglob.o链接以不展开（参照<a href="https://sourceforge.net/p/mingw-w64/mingw-w64/ci/master/tree/mingw-w64-crt/crt/CRT_noglob.c">CRT_noglob.c</a>）。</p>
		<p>区区一个回显命令，怎么到处都有怪问题？批处理与外壳编程就是这么变态。</p>
	</dd>
	<dt>slzpdf</dt>
	<dd>以仅次于复制文件的速度对命令行参数中指定的所有JPEG图像文件作为每一页合并生成一个PDF文件。页面大小为A4，无边距，图像保持比例最大化居中。此外，还支持将文本文件导出为PDF，不过这并不是主要功能。</dd>
	<dt>columnar</dt>
	<dd>标准C语言课程作业之整数竖式表达式计算。输入一行含有加、减、乘、除的整数表达式，按照优先级计算，输出中间计算过程竖式和结果。不支持括号。</dd>
	<dt>bat</dt>
	<dd>（仅限Windows。）执行一句批处理命令。很奇怪，但有时候用cmd.exe就是搞不定。</dd>
	<dt>cpro</dt>
	<dd>（仅限Windows。）如《编程珠玑（续）》之第1章所述，用一晚上的时间实现的性能监视工具，作用是记录C语句执行次数。对代码风格有要求：大括号皆位于关键字同行末。不太明确能否被计数的块则依次交由<code>gcc -fsyntax-only</code>判别。</dd>
</dl>

还有一些单文件网页。

<dl>
	<dt>cppedit</dt>
	<dd>非常简陋的C++程序输入法，主要用于在手机上写数据结构课的作业。</dd>
	<dt>sqrtpi</dt>
	<dd>重度残废的单目录列表脚本，放哪读哪。需要文件系统编码为UTF-8。上传文件的功能可能存在权限问题。</dd>
</dl>

库的名字“Sketch library”是用谷歌翻译机翻译自“小品程序库”的结果。
