#include <iostream>
#include <vector>
#include "managers/ParamsManager.h"
#include "managers/FileManager.h"
#include "utils/CLog.h"
#include "managers/DBManager.h"
#include "managers/ConsoleDisplayManager.h"
#include "managers/FileOutPutManager.h"

#define  DEBUG false
using namespace std;


int main(int argc, char *argv[]) {
    fprintf(stdout,  "Обработка запроса начата\n" );
    auto *u = new ParamsManager(argc, argv);
    DisplayManager *dpm;
    CLog::SetLevel(CLog::Debug);

    if (!u ->isParamValid()) {
        fprintf(stderr, "Ошибочные пераметры. Работа программы прекращена\n" );
        return 1;
    }
    vector<string> instructions;
    if (u ->checkParamValue("-f")) {
        // набор команд пришел из файла;
        auto fname = u ->getParamValue("-f");
        FileManager *fm = new FileManager(fname);
        if (!fm->isValid()) {
            throw "Ошибка при работе с файлом " + fname;
        }

        instructions = fm->readInstructions();
        delete fm;
    } else if (u ->checkParamValue("-q")) {
        // набор комманд пришел из строки запроса
        instructions.push_back(u ->getParamValue("-q"));
    }

    string outValue =u ->getParamValue("-o");
    if (outValue == "FILE") {
        string fname = u ->getParamValue("-of");
        dpm = new FileOutPutManager(fname);
//        free( &fname);
    } else if (outValue == "CONSOLE") {
        dpm = new ConsoleDisplayManager();
    } else {
        throw "Выбран не верныф формат вывода результатов";
    }

    string dbName = u ->getParamValue("-d");

    DBManager *dbm = new DBManager(dpm, dbName, true);

    dbm -> executeQuery(instructions);

    fprintf(stdout, "Успешное завершение работы программы\n");

    delete u;
    delete dbm;
    delete dpm;
    return 0;
}



