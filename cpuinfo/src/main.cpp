// Displays /proc/cpuinfo

#include <QMessageBox>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <fstream>
#include <iostream>
#include <vector>

QT_USE_NAMESPACE

using namespace std ;

// Reuse - http://www.sbin.org/doc/HOWTO/C++Programming-HOWTO-7.html
void Tokenize(const string& str,
                      std::vector<string>& tokens,
                      const string& delimiters = ":\t")
{
    if (0 == str.length())
        return ;

    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QVBoxLayout *layout = new QVBoxLayout;

    std::ifstream fileCpuInfo("/proc/cpuinfo");
    std::string s;
    std::string delimiter = "\t:";
    while (std::getline(fileCpuInfo, s))
    {
//        std::cout << str ;
        std::vector<std::string> tokens;

        Tokenize(s, tokens);

        QString line;
        if (0 < tokens.size()) {
            line = QString(tokens[0].c_str()) + " : ";
            if (1 < tokens.size()) {
                line += QString(tokens[tokens.size()-1].c_str()) ;
            }
        }

        QLabel *label = new QLabel(line);
        layout->addWidget(label);
    }

    QWidget *workPage = new QWidget;
    workPage->setLayout(layout);

    QScrollArea area;
    area.setWindowTitle(QObject::tr("CPU Information from /proc/cpuinfo."));
    area.setWidget(workPage);
    area.show();

    return a.exec();
}

