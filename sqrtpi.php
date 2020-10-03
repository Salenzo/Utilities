<?php
if ($_GET["phpinfo"] === "123") {
	phpinfo();
	return;
}

function pi_htmlformat($template, $text) {
	return str_replace("★", htmlspecialchars($text, ENT_HTML5 | ENT_DISALLOWED, "UTF-8"), $template);
}

if ($_FILES["insertpi"]["error"] > 0) {
	echo pi_htmlformat("上传文件时发生错误。<br>★", $_FILES["insertpi"]["error"]);
	return;
} else {
	if (file_exists("upload/" . $_FILES["insertpi"]["name"])) {
		echo pi_htmlformat("★ 已存在。", $_FILES["insertpi"]["name"]);
		return;
	} else {
		move_uploaded_file($_FILES["insertpi"]["tmp_name"], $_FILES["insertpi"]["name"]);
	}
}

$depth = 0;

function pi_list($dir, $link) {
	$depth++;
	if ($depth > 100) {
		echo "<div>目录嵌套太深。</div>";
		return;
	}
	$filenames = scandir($dir, SCANDIR_SORT_ASCENDING);
	foreach ($filenames as $filename) {
		if ($filename === "." || $filename === ".." || $filename === basename(__FILE__)) continue;
		if (is_dir($dir . DIRECTORY_SEPARATOR . $filename)) {
			echo pi_htmlformat("<details><summary>★</summary>", $filename);
			pi_list($dir . DIRECTORY_SEPARATOR . $filename, $link . "/" . urlencode($filename));
			echo "</details>";
		} else {
			echo "<div><a href=\"" . $link . "/" . urlencode($filename) . "\">";
			echo pi_htmlformat("★</a></div>", $filename);
		}
	}
	$depth--;
}
?>
<!DOCTYPE html>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
<meta http-equiv="X-UA-Compatible" content="ie=edge">
<title>√(π)</title>
<style>
details {
	padding-left: 1em;
}
details > summary {
	margin-left: -1em;
}
</style>
<h1 style="float: right;">√<var style="text-decoration: overline;">π</var></h1>
<form action="<?php echo urlencode(basename(__FILE__)); ?>" method="post" enctype="multipart/form-data">
<button onclick="Array.prototype.forEach.call(document.getElementsByTagName('details'), function (el) { el.open = true; });">全部展开</button>
<button onclick="Array.prototype.forEach.call(document.getElementsByTagName('details'), function (el) { el.open = false; });">全部收起</button>
<input type="file" name="insertpi">
<input type="submit" value="添加">
</form>
<hr>
<main>
<?php
$dir = "/home/satgo/sqrtpi";
pi_list($dir, ".");
if (is_readable($dir . DIRECTORY_SEPARATOR . "README.txt")) {
	echo pi_htmlformat("<hr><h2>读我.文本文档</h2><pre>★</pre>", file_get_contents($dir . DIRECTORY_SEPARATOR . "README.txt"));
}
?>
</main>
