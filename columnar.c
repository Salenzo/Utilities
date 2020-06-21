#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

// 读入一个整数，可能有正负号。遇到非数字字符即停止。返回成功读入的数字个数（1或0）
int read_int(int* result) {
	// sgn用于存放符号（1或-1）
	// read（过去分词）是读成功与否的标志
	int a = 0, sgn = 1, read = 0;
	for (;;) {
		int c = getchar();
		if (!read && (c == '+' || c == '-')) {
			if (c == '-') sgn = -sgn;
		} else if (c >= '0' && c <= '9') {
			a = a * 10 + (c - '0');
			read = 1;
		} else {
			if (read) {
				*result = a * sgn;
			}
			ungetc(c, stdin);
			return read;
		}
	}
}

// 显示乘法竖式
void show_mul(int a, int b) {
	int w = 12, ans;
	a = abs(a);
	b = abs(b);
	ans = a * b;
	printf("%12d\nx%11d\n------------\n", a, b);
	while (b) {
		printf("%*d\n", w, a * (b % 10));
		w--;
		b /= 10;
	}
	printf("------------\n%12d\n\n", ans);
}

// 显示除法竖式
void show_div(int a, int b) {
	// k用于存放剩余待处理的答案位数，kp是10^(k - 1)
	int ans, tmp, i, k, kp;
	a = abs(a);
	b = abs(b);
	ans = a / b;
	// 计算答案的位数
	tmp = ans;
	k = 0;
	kp = 1;
	while (tmp) {
		k++;
		kp *= 10;
		tmp /= 10;
	}
	kp /= 10;
	if (ans == 0) {
		k = 1;
		kp = 1;
	}
	// 显示竖式
	printf("%25d\n             ------------\n%12d)%12d\n", ans, b, a);
	while (k) {
		printf("%*d\n            -------------\n", 26 - k, (ans / kp) % 10 * b);
		if (k == 1) {
			printf("%*d\n\n", 26 - k, a % b);
		} else {
			printf("%*d\n", 27 - k, (a / kp) % b * 10 + (a * 10 / kp) % 10);
		}
		k--;
		kp /= 10;
	}
}

int main() {
	char c;
	// 保存输入的数
	int array[1000];
	size_t n = 0;
	// muldiv = 1表示有待做的乘法运算，2表示有待做的除法运算
	int muldiv = 0;
	// 整个式子的和
	int sum = 0;
	// 刚输入的数
	int a;
	fprintf(stderr, "Input formula: ");
	for (;;) {
		if (read_int(&a) == 1) {
			// 加号与减号作为正负号已经读入到a中
			if (muldiv == 0) {
				// 没有待做的乘除法，则将数字存放好备用，因为其后仍可能遇到需要先做的乘除法
				array[n++] = a;
			} else if (muldiv == 1) {
				show_mul(array[n - 1], a);
				array[n - 1] *= a;
			} else {
				if (a == 0) {
					fprintf(stderr, "Division by zero encountered!\n");
					return 1;
				}
				show_div(array[n - 1], a);
				array[n - 1] /= a;
			}
			muldiv = 0;
		} else {
			// 没有数字，则一定是运算符或输入结束
			do {
				c = getchar();
			} while (!strchr("*/\r\n", c));
			if (c == '*') {
				muldiv = 1;
			} else if (c == '/') {
				muldiv = 2;
			} else {
				break;
			}
		}
	}
	if (n > 1) {
		// 有需要相加减的数，则显示加法竖式
		for (size_t i = 0; i < n; i++) {
			sum += array[i];
			printf("%c%11d\n", array[i] < 0 ? '-' : i ? '+' : ' ', array[i]);
		}
		printf("------------\n%12d\n", sum);
	} else {
		sum = array[0];
	}
	printf("The answer is %d.\n", sum);
	return 0;
}
