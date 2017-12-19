#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <functional>

using namespace std;

template<class T, class U>
ostream &operator<<(ostream &os, const pair<T, U> &p)
{
    os << "(" << p.first << "," << p.second << ")";
    return os;
}

template<class T>
ostream &operator<<(ostream &os, const vector<T> &v)
{
    os << "[";
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin()) {
            os << ",";
        }
        os << *it;
    }
    os << "]";
    return os;
}

struct MyPair {
    int a;
    int b;
    MyPair(int a, int b) : a(a), b(b) {}
    friend ostream &operator<<(ostream &os, const MyPair &p);
};

ostream &operator<<(ostream &os, const MyPair &p)
{
    os << "(" << p.a << "," << p.b << ")";
}

/******************************************************************************/
/* http://clang.llvm.org/extra/clang-tidy/checks/modernize-loop-convert.html  */
/******************************************************************************/
void run_loop_convert()
{
    cout << __FUNCTION__ << endl;

    const int N = 5;
    int arr[] = {1, 2, 3, 4, 5};
    vector<string> v;
    v.push_back("aa");
    v.push_back("bb");
    v.push_back("cc");

    // safe conversion
    cout << "    iterate over array: [";
    for (int i = 0; i < N; i++) {
        cout << arr[i] << ",";
    }
    cout << "]" << endl;

    // reasonable conversion
    cout << "    iterate over vector using iterator: [";
    for (vector<string>::iterator it = v.begin(); it != v.end(); ++it) {
        cout << *it << ",";
    }
    cout << "]" << endl;

    // reasonable conversion
    cout << "    iterate over vector using simple for loop: [";
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << ",";
    }
    cout << "]" << endl;

    // risky conversion
    cout << "    iterate over vector using iterator (modify vector; evaluate end() only once): [";
    bool flag = false;
    for (vector<string>::iterator it = v.begin(); it != v.end(); ++it) {
        if (!flag) {
            // This line makes this transformation 'risky'.
            v.push_back(*it);
            flag = true;
        }
        cout << *it << ",";
    }
    cout << "]" << endl;
}

/******************************************************************************/
/* http://clang.llvm.org/extra/clang-tidy/checks/modernize-make-shared.html   */
/* http://clang.llvm.org/extra/clang-tidy/checks/modernize-make-unique.html   */
/******************************************************************************/
void run_make_shared_and_make_unique()
{
    cout << __FUNCTION__ << endl;

    auto my_shared_ptr = shared_ptr<MyPair>(new MyPair(1, 2));
    cout << "    my shared pair: " << *my_shared_ptr << endl;

    my_shared_ptr.reset(new MyPair(3, 4));
    cout << "    my other shared pair: " << *my_shared_ptr << endl;

    auto my_unique_ptr = unique_ptr<MyPair>(new MyPair(5, 6));
    cout << "    my unique pair: " << *my_unique_ptr << endl;

    my_unique_ptr.reset(new MyPair(7, 8));
    cout << "    my other unique pair: " << *my_unique_ptr << endl;
}

/******************************************************************************/
/* http://clang.llvm.org/extra/clang-tidy/checks/modernize-pass-by-value.html */
/******************************************************************************/
void run_pass_by_value()
{
    cout << __FUNCTION__ << endl;

    class Foo
    {
    public:
        Foo(const string &copied, const string &read_only) :
            _copied(copied), _read_only(read_only)
        {
            cout << "  Foo constructor" << endl
                 << "    copied=" << copied << endl
                 << "    read_only=" << read_only << endl;
        }
    private:
        string _copied;
        const string &_read_only;
    };

    string cwd = "/current/working/dir";
    const string path = "/some/path";
    Foo foo(cwd, path);
}

/***********************************************************************************/
/* http://clang.llvm.org/extra/clang-tidy/checks/modernize-raw-string-literal.html */
/***********************************************************************************/
void run_raw_string_literal()
{
    cout << __FUNCTION__ << endl;

    const char *const quotes{"embedded \"quotes\""};
    const char *const paragraph{"[Line one.\nLine two.\nLine three.\n]"};
    const char *const single_line{"[Single line.\n]"};
    const char *const trailing_space{"[Look here -> \n]"};
    const char *const tab{"[One\tTwo\n]"};
    const char *const bell{"Hello!\a  And welcome!"};
    const char *const path{"C:\\Program Files\\Vendor\\Application.exe"};
    const char *const regex{"\\w\\([a-z]\\)"};

    cout << "  quotes: " << quotes << endl
         << "  paragraph: " << paragraph << endl
         << "  single_line: " << single_line << endl
         << "  trailing_space: " << trailing_space << endl
         << "  tab: " << tab << endl
         << "  bell: " << bell << endl
         << "  path: " << path << endl
         << "  regex: " << regex << endl;
}

/******************************************************************************/
/* http://clang.llvm.org/extra/clang-tidy/checks/modernize-shrink-to-fit.html */
/******************************************************************************/
void run_shrink_to_fit()
{
    cout << __FUNCTION__ << endl;

    vector<string> v(1000, "item");
    cout << "Original vector" << endl
         << "  size: " << v.size() << endl
         << "  capacity: " << v.capacity() << endl;

    // Removing half of the elements
    for (int i = v.size(); i > 0; i -= 2) {
        v.pop_back();
    }

    cout << "Modified vector" << endl
         << "  size: " << v.size() << endl
         << "  capacity: " << v.capacity() << endl;

    // Swap with empty vector to match capacity with size
    vector<string>(v).swap(v);

    cout << "Resized vector" << endl
         << "  size: " << v.size() << endl
         << "  capacity: " << v.capacity() << endl;
}

/******************************************************************************/
/* http://clang.llvm.org/extra/clang-tidy/checks/modernize-use-auto.html      */
/******************************************************************************/
void run_use_auto()
{
    cout << __FUNCTION__ << endl;

    // Simple iterator
    {
        vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);

        cout << "  vector: ";
        vector<int>::iterator it = v.begin();
        while(it != v.end()) {
            cout << *it << ", ";
            ++it;
        }
        cout << endl;
    }

    // New expressions
    {
        MyPair *my_ptr = new MyPair(1, 2);
        cout << "  new pair: " << *my_ptr << endl;
        delete my_ptr;
    };

    // Cast expressions
    {
        bool foo = true;
        int bar = static_cast<int>(foo);
        cout << "  foo=" << foo << ", bar=" << bar << endl;
    }
}

/****************************************************************************************/
/* http://clang.llvm.org/extra/clang-tidy/checks/modernize-use-default-member-init.html */
/****************************************************************************************/
void run_use_default_member_init()
{
    cout << __FUNCTION__ << endl;

    struct A {
        A() : i(5), j(10.0) {}
        A(int i) : i(i), j(10.0) {}
        int i;
        double j;
    };

    A foo;
    A bar(2);
    cout << "  foo.i=" << foo.i << ", foo.j=" << foo.j << endl
         << "  bar.i=" << bar.i << ", bar.j=" << bar.j << endl;
}

/******************************************************************************/
/* http://clang.llvm.org/extra/clang-tidy/checks/modernize-use-emplace.html   */
/******************************************************************************/
void run_use_emplace()
{
    cout << __FUNCTION__ << endl;

    vector<string> v;
    v.push_back(string(10, 'a'));
    cout << "  vector v: " << v << endl;

    vector<pair<int, int>> w;
    w.push_back(pair<int, int>(21, 37));
    w.push_back(make_pair(21L, 37L));
    cout << "  vector w: " << w << endl;
}

/******************************************************************************/
/* http://clang.llvm.org/extra/clang-tidy/checks/modernize-use-emplace.html   */
/******************************************************************************/

int main(int argc, char* argv[])
{
    cout << "Hello, BeCPP!" << endl
         << "Let's go over the examples:" << endl;

    run_loop_convert();
    run_make_shared_and_make_unique();
    run_pass_by_value();
    run_raw_string_literal();
    run_shrink_to_fit();
    run_use_auto();
    run_use_default_member_init();
    run_use_emplace();

    return 0;
}
