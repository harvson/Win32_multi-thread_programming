#include <iostream>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<string, string> pss;
#define rep(i, a, n) for (int i = a; i < n; ++i)

/*
** 枚举类型：CHECK_TYPE
** 枚举说明：用来表征rawInput检测后的类型
*/
enum CHECK_TYPE {
    NO_NUM,
    POSITIVE_NUM,
    NEGATIVE_NUM
};


int checkInput(string s) {
    if (s == "")
        return CHECK_TYPE::NO_NUM;

    bool flag1 = false;
    if (s[0] == '-') {
        flag1 = true;
        rep(i, 1, s.size()) {
            if (s[i] < '0' || s[i] > '9') {
                return CHECK_TYPE::NO_NUM;
            }
        }
    } else {
        rep(i, 0, s.size()) {
            if (s[i] < '0' || s[i] > '9') {
                return CHECK_TYPE::NO_NUM;
            }
        }
    }
    if (flag1)
        return CHECK_TYPE::NEGATIVE_NUM;
    else
        return CHECK_TYPE::POSITIVE_NUM;
}


bool checkLeapYear(int year) {
    // 判断闰年的核心逻辑
    if (year % 400 == 0 || year % 4 == 0 && year % 100 != 0) {
        return true;
    }
    return false;
}


int main(int argc, char *argv[]) {
    cout << ("======= 请输入年份 ==========\n");

    string rawInput;
    int parseNum;
    while (cin >> rawInput) {
        // 调用
        int ans = checkInput(rawInput);
        switch (ans) {
            case CHECK_TYPE::NO_NUM:
                cout << "无效年份" << endl;
                break;
            case CHECK_TYPE::NEGATIVE_NUM:
                cout << "输入年份需要大于0" << endl;
                break;
            case CHECK_TYPE::POSITIVE_NUM:
                parseNum = atoi(rawInput.c_str());
                if (checkLeapYear(parseNum))
                    cout << parseNum << " 是闰年" << endl;
                else
                    cout << parseNum << "是平年" << endl;
                break;
            default:
                break;
        }
    }

    cout << "==============================================" << endl;
    return 0;
}