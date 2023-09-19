#include <iostream>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<string, string> pss;
#define rep(i, a, n) for (int i = a; i < n; ++i)

/*
** ö�����ͣ�CHECK_TYPE
** ö��˵������������rawInput���������
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
    // �ж�����ĺ����߼�
    if (year % 400 == 0 || year % 4 == 0 && year % 100 != 0) {
        return true;
    }
    return false;
}


int main(int argc, char *argv[]) {
    cout << ("======= ��������� ==========\n");

    string rawInput;
    int parseNum;
    while (cin >> rawInput) {
        // ����
        int ans = checkInput(rawInput);
        switch (ans) {
            case CHECK_TYPE::NO_NUM:
                cout << "��Ч���" << endl;
                break;
            case CHECK_TYPE::NEGATIVE_NUM:
                cout << "���������Ҫ����0" << endl;
                break;
            case CHECK_TYPE::POSITIVE_NUM:
                parseNum = atoi(rawInput.c_str());
                if (checkLeapYear(parseNum))
                    cout << parseNum << " ������" << endl;
                else
                    cout << parseNum << "��ƽ��" << endl;
                break;
            default:
                break;
        }
    }

    cout << "==============================================" << endl;
    return 0;
}