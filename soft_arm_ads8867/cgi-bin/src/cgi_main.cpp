#include "cgi_main.h"

using namespace std;

// 简单的解析函数，用于解析POST数据
void parseFormData(string &data) {
    string query = getenv("QUERY_STRING");
    string key, value;
    size_t pos = 0;
    while ((pos = query.find("&")) != string::npos) {
        key = query.substr(0, query.find("="));
        value = query.substr(query.find("=") + 1, pos - query.find("=") - 1);
        if (key == "username") {
            data = value; // 只返回用户名作为示例
            break;
        }
        query.erase(0, pos + 1);
    }
}

int main() {
    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html><body>";

    if (string(getenv("REQUEST_METHOD")) == "POST") {
        string username, password, data;
        parseFormData(data);

        // 这里仅为示例，实际应用中需要更安全的验证方式
        if (data == "admin") {
            cout << "<h2>Welcome back, admin!</h2>";
        } else {
            cout << "<h2>Login failed: Invalid credentials</h2>";
        }
    } else {
        cout << "<h2>Unsupported HTTP Method</h2>";
    }

    cout << "</body></html>";

    return 0;
}

