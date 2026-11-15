#include <clang-c/Index.h>
#include <iostream>
using namespace std;

int lambdas = 0;
int loops = 0;

bool is_loop(CXCursorKind k)
{
    return k == CXCursor_ForStmt || k == CXCursor_CXXForRangeStmt ||
           k == CXCursor_WhileStmt || k == CXCursor_DoStmt;
}

int line_of(CXCursor c)
{
    unsigned line;
    clang_getSpellingLocation(clang_getCursorLocation(c), nullptr, &line, nullptr, nullptr);
    return line;
}

CXChildVisitResult count_loops(CXCursor c, CXCursor parent, CXClientData data)
{
    if (c.kind == CXCursor_LambdaExpr)
        return CXChildVisit_Continue;
    if (is_loop(c.kind))
        (*(int *)data)++;
    return CXChildVisit_Recurse;
}

CXChildVisitResult find_lambdas(CXCursor c, CXCursor parent, CXClientData data)
{
    if (!clang_Location_isFromMainFile(clang_getCursorLocation(c)))
        return CXChildVisit_Continue;

    if (c.kind == CXCursor_LambdaExpr) {
        int n = 0;
        clang_visitChildren(c, count_loops, &n);
        lambdas++;
        loops += n;
        cout << "лямбда в рядку " << line_of(c) << ": циклів " << n << endl;
    }
    return CXChildVisit_Recurse;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        cout << "запуск: ./lab7 file.cpp" << endl;
        return 1;
    }

    CXIndex index = clang_createIndex(0, 1);
    CXTranslationUnit unit = clang_parseTranslationUnit(index, argv[1], argv + 2, argc - 2, nullptr, 0, 0);
    if (!unit) {
        cout << "помилка" << endl;
        clang_disposeIndex(index);
        return 1;
    }

    clang_visitChildren(clang_getTranslationUnitCursor(unit), find_lambdas, nullptr);

    cout << "всього лямбд: " << lambdas << endl;
    cout << "всього циклів у лямбдах: " << loops << endl;

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
    return 0;
}
