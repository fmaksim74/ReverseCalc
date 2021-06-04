#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <stack>
#include <vector>
#include <list>
#include <tuple>
#include <chrono>
#include <algorithm>


const int IN_SIZE = 5;
const int X[IN_SIZE] = { 5, 10, 28, 41, 562340 };
std::wstring R[IN_SIZE] = { L"121", L"331", L"3331", L"331221", L"3333312222122213312"};

std::wstring calc1(int value);
std::wstring calc2(int value);
std::wstring calc3(int value);
std::wstring calc4(int value);
std::wstring calc5(int value);
int check(std::wstring value);

const int CALC_CNT = 5;
std::wstring(*calcs[CALC_CNT])(int) = { calc1, calc2, calc3, calc4, calc5 };

const int CALC_NUM = 1;
const int INPUT_SIZE = 6;
const int RESULT_SIZE = 30;
const int CHECK_SIZE = 6;
const int SCMP_SIZE = 4;
const int LCMP_SIZE = 4;
const int TIME_SIZE = 20;

// last +1 for WEOF (\xffff)
const int LINE_SIZE = 1 + CALC_NUM + 1 + INPUT_SIZE + 1 + RESULT_SIZE + 1 + CHECK_SIZE + 1 + LCMP_SIZE + 1 + TIME_SIZE + 1 + 1;
std::wstring line_template = L"";

void print_splitter() {
	wprintf_s(L"\u251C%s\u253C%s\u253C%s\u253C%s\u253C%s\u253C%s\u253C%s\u2524\n",
		std::wstring(CALC_NUM, L'\u2500').c_str(),
		std::wstring(INPUT_SIZE, L'\u2500').c_str(),
		std::wstring(RESULT_SIZE, L'\u2500').c_str(),
		std::wstring(CHECK_SIZE, L'\u2500').c_str(),
		std::wstring(SCMP_SIZE, L'\u2500').c_str(),
		std::wstring(LCMP_SIZE, L'\u2500').c_str(),
		std::wstring(TIME_SIZE, L'\u2500').c_str());
}

void print_header() {

	wprintf_s(L"\u250C%s\u252C%s\u252C%s\u252C%s\u252C%s\u252C%s\u252C%s\u2510\n",
		std::wstring(CALC_NUM, L'\u2500').c_str(),
		std::wstring(INPUT_SIZE, L'\u2500').c_str(),
		std::wstring(RESULT_SIZE, L'\u2500').c_str(),
		std::wstring(CHECK_SIZE, L'\u2500').c_str(),
		std::wstring(SCMP_SIZE, L'\u2500').c_str(),
		std::wstring(LCMP_SIZE, L'\u2500').c_str(),
		std::wstring(TIME_SIZE, L'\u2500').c_str());

	wprintf_s(L"\u2502%-*s\u2502%-*s\u2502%-*s\u2502%-*s\u2502%-*s\u2502%-*s\u2502%-*s\u2502\n",
		CALC_NUM, L"N", INPUT_SIZE, L"Input", RESULT_SIZE, L"Result", CHECK_SIZE, L"Check", SCMP_SIZE, L"SCMP", LCMP_SIZE, L"LCMP", TIME_SIZE, L"TIME");

	print_splitter();
}

void print_line(int calc, int input, const wchar_t* result, int check, bool svalid, bool lvalid, long long duration) {
	wprintf_s(L"\u2502%*d\u2502%-*d\u2502%-*s\u2502%-*d\u2502%-*s\u2502%-*s\u2502%-*I64i\u2502\n",
		CALC_NUM, calc, INPUT_SIZE, input, RESULT_SIZE, result, CHECK_SIZE, check, SCMP_SIZE, (svalid) ? L"OK" : L"ERR", LCMP_SIZE, (lvalid) ? L"OK" : L"ERR", TIME_SIZE, duration);
}

void print_footer() {
	wprintf_s(L"\u2514%s\u2534%s\u2534%s\u2534%s\u2534%s\u2534%s\u2534%s\u2518\n",
		std::wstring(CALC_NUM, L'\u2500').c_str(),
		std::wstring(INPUT_SIZE, L'\u2500').c_str(),
		std::wstring(RESULT_SIZE, L'\u2500').c_str(),
		std::wstring(CHECK_SIZE, L'\u2500').c_str(),
		std::wstring(SCMP_SIZE, L'\u2500').c_str(),
		std::wstring(LCMP_SIZE, L'\u2500').c_str(),
		std::wstring(TIME_SIZE, L'\u2500').c_str());
}

int main(int argc, char* argv[]) {

	int mode = 0;
	mode = _setmode(_fileno(stdout), _O_U8TEXT);
	mode = _setmode(_fileno(stdin) , _O_U8TEXT);
	mode = _setmode(_fileno(stderr), _O_U8TEXT);

	std::wstring result = L"";

	print_header();
	for (int i = 0; i < IN_SIZE; ++i) {
		print_line(0, X[i], R[i].c_str(), check(R[i]), true, true, 0);
	}
	for (int i = 0; i < CALC_CNT; i++) {
		print_splitter();
		for (int j = 0; j < IN_SIZE; j++) {
			auto start = std::chrono::high_resolution_clock::now();
			result = calcs[i](X[j]);
			auto finish = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
			print_line(i + 1, X[j], result.c_str(), check(result), result == R[j], result.size() == R[j].size(), duration);
		}
	}
	print_footer();

	return 0;
}

int check(std::wstring value) {

	int res = 1;

	for (wchar_t c : value) {
		if (c == '3') res *= 3;
		if (c == '2') res *= 2;
		if (c == '1') res += 1;
	}

	return res;
}

std::wstring calc1(int value) {

	if (value <= 1) return L"";

	if (value == 2) {
		value -= 1;
		return L"1";
	}

	int i3 = value - (value / 3) * 3;
	int i2 = value - (value / 2) * 2;

	std::wstring s3 = calc1((value - i3) / 3).append(L"3").append(std::wstring(i3, '1'));

	if (i3 == i2) return s3;

	std::wstring s2 = calc1((value - i2) / 2).append(L"2").append(std::wstring(i2, '1'));

	return (s3.length() <= s2.length()) ? s3 : s2;
}

std::wstring calc2(int value) {
	
	std::wstring result;

	std::tuple<int, std::wstring> tuple;
	std::stack<std::tuple<int, std::wstring>> stack;

	int curr = 0;
	std::wstring buffer = L"";
	stack.push(std::make_tuple(value, buffer));
	while (!stack.empty()) {

		tuple = stack.top();
		stack.pop();

		curr = std::get<0>(tuple);
		buffer = std::get<1>(tuple);

		if (curr == 1) {
			if (result.size() == 0 || result.size() > buffer.size())
				result = buffer;
			continue;
		}

		if (curr > 1) {
			if (curr == 2) {
				curr -= 1;
				stack.push(std::make_tuple(curr, buffer.insert(0, L"1")));
				continue;
			}

			int i3 = curr - (curr / 3) * 3;
			int i2 = curr - (curr / 2) * 2;

			curr = (curr - i3) / 3;
			stack.push(std::make_tuple(curr, buffer.insert(0, L"3" + std::wstring(i3, '1'))));

			curr = std::get<0>(tuple);
			buffer = std::get<1>(tuple);

			curr = (curr - i2) / 2;
			stack.push(std::make_tuple(curr, buffer.insert(0, L"2" + std::wstring(i2, '1'))));
		}
	}

	return result;
}

std::wstring calc3(int value) {
	
	std::wstring result;

	std::tuple<int, std::wstring> tuple;
	std::list<std::tuple<int, std::wstring>> stack;

	int curr = 0;
	std::wstring buffer = L"";
	stack.push_back(std::make_tuple(value, buffer));
	while (!stack.empty()) {

		tuple = stack.front();
		stack.pop_front();

		curr = std::get<0>(tuple);
		buffer = std::get<1>(tuple);

		if (curr == 1) {
			if (result.size() == 0 || result.size() > buffer.size())
				result = buffer;
			continue;
		}

		if (curr > 1) {
			if (curr == 2) {
				curr -= 1;
				stack.push_back(std::make_tuple(curr, buffer.append(L"1")));
				continue;
			}

			int i3 = curr % 3;
			int i2 = curr % 2;

			curr = (curr - i3) / 3;
			buffer.append(std::wstring(i3, '1')).append(L"3");
			stack.push_back(std::make_tuple(curr, buffer));

			curr = std::get<0>(tuple);
			buffer = std::get<1>(tuple);

			curr = (curr - i2) / 2;
			buffer.append(std::wstring(i2, '1')).append(L"2");
			stack.push_back(std::make_tuple(curr, buffer));
		}
	}
	std::reverse(result.begin(), result.end());
	return result;
}

std::wstring calc4(int value) {
	std::wstring result;
	while (value > 1)
	{
		if (value % 3 == 0)
		{
			value = value / 3;
			result.append(L"3");
		}
		else if (value % 2 == 0)
		{
			if ((value - 1) % 3 == 0 && value % 4 != 0)
			{
				value--;
				result.append(L"1");
				continue;
			}
			value = value / 2;
			result.append(L"2");
		}
		else
		{
			value--;
			result.append(L"1");
		}
	}
	std::reverse(result.begin(), result.end());
	return result;
}

std::wstring calc5(int value) {

	size_t sz = value;
	std::vector<int> A;
	A.resize(sz + 1);

	int min;
	for (size_t i = 2; i < sz + 1; i++) {
		min = A[i - 1] + 1;
		if (i % 2 == 0) min = std::min(min, A[i / 2] + 1);
		if (i % 3 == 0) min = std::min(min, A[i / 3] + 1);

		A[i] = min;
	}

	std::wstring result = L"";

	{
		size_t i = value;
		while (i > 1) {
			if (A[i] == A[i - 1] + 1) {
				result.insert(0, L"1");
				i--;
				continue;
			}

			if (i % 2 == 0 && A[i] == A[i / 2] + 1) {
				result.insert(0, L"2");
				i /= 2;
				continue;
			}

			result.insert(0, L"3");
			i /= 3;
		}
	}
	return result;
}