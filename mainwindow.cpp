#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QProcess"
#include "QObject"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox_options->addItem("Emergency");
    ui->comboBox_options->addItem("Alert");
    ui->comboBox_options->addItem("Critical");
    ui->comboBox_options->addItem("Error");
    ui->comboBox_options->addItem("Warning");
    ui->comboBox_options->addItem("Notice");
    ui->comboBox_options->addItem("Informational");
    ui->comboBox_options->addItem("Debug");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Refresh_clicked()
{
    QProcess *journal = new QProcess(this);

    // Connect the readyReadStandardOutput signal to a custom slot to handle the output
    connect(journal, &QProcess::readyReadStandardOutput, this, &MainWindow::handleJournalOutput);

    // Start the journalctl command
    QStringList arguments;
    arguments.append("--list-boots");
    // arguments << "--list-boots";
    journal->start("journalctl", arguments);
}

// Custom slot to handle the journalctl output
void MainWindow::handleJournalOutput()
{
    // Get the sender (the QProcess)
    QProcess *process = qobject_cast<QProcess*>(sender());

    // // Read the output and append it to the QPlainTextEdit
    // ui->plainTextEdit->append(process->readAllStandardOutput());

    // Read the output from journalctl --list-boots
    QString output = process->readAllStandardOutput();

    // Clear the combo box before adding new items
    ui->comboBox_list_boot->clear();

    // Split the output by lines
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    // Iterate through each line, parse it, and add it to the combo box
    for (const QString &line : lines) {
        // The format of each line is: <index> <boot-id> <timestamp>
        // You may want to display the index and timestamp in the combo box
        QStringList columns = line.split(' ', Qt::SkipEmptyParts);
        if (columns.size() >= 3) {
            QString index = columns[0];
            QString timestamp = columns.mid(2).join(' '); // Join the rest as the timestamp may contain spaces
            QString comboBoxItem = QString("%1 - %2").arg(index, timestamp);
            ui->comboBox_list_boot->addItem(comboBoxItem);
        }
    }
    // ui->plainTextEdit->append(output);
}
// void MainWindow::FinalOutput()
// {
//     QString selected_options = ui->comboBox_options->currentText();
//     ui->plainTextEdit->append(selected_options);
//     // ui->plainTextEdit->append()
// }

void MainWindow::on_pushButton_Final_clicked()
{
    ui->textEdit->clear();
    QString selected_bootlist = ui->comboBox_list_boot->currentText();
    QString selected_options = ui->comboBox_options->currentText();
    QStringList arguments;
    if (!selected_bootlist.isEmpty()) {
        QStringList bootParts = selected_bootlist.split(' ', Qt::SkipEmptyParts);
        if (!bootParts.isEmpty()) {
            arguments << "-b" << bootParts[0];  // Use the full boot ID
        }
    }
    if (ui->checkBox->isChecked()) {
        arguments << "-x";
    }
    if (selected_options == "Emergency") {
        arguments << "-p0";
    }
    else if (selected_options == "Alert") {
        arguments << "-p1";
    }
    else if (selected_options == "Critical") {
        arguments << "-p2";
    }
    else if (selected_options == "Error") {
        arguments << "-p3";
    }
    else if (selected_options == "Warning") {
        arguments << "-p4";
    }
    else if (selected_options == "Notice") {
        arguments << "-p5";
    }
    else if (selected_options == "Informational") {
        arguments << "-p6";
    }
    else if (selected_options == "Debug") {
        arguments << "-p7";
    }
    QProcess *options = new QProcess(this);
    // options->start("source /home/arch/.bashrc");
    options->start("journalctl", arguments);
    options->waitForFinished();
    QString output = options->readAllStandardOutput();
    QString errors = options->readAllStandardError();
    ui->textEdit->append(output);
    if (!errors.isEmpty()) {
        ui->textEdit->append("Errors:\n" + errors);
    }
    // connect(options, &QProcess::readyReadStandardOutput, [=]() {
    //     ui->plainTextEdit->append(options->readAllStandardOutput());
    // });
    // ui->plainTextEdit->append(select);
}

// void MainWindow::on_checkBox_checkStateChanged(const Qt::CheckState &arg1)
// {

// }


// void MainWindow::on_nextsearch_clicked()
// {
//     ui->textEdit->find(ui->lineEdit->text());
// }
void MainWindow::on_nextsearch_clicked()
{
    QString searchText = ui->lineEdit->text();
    if (!searchText.isEmpty()) {
        bool found = ui->textEdit->find(searchText);

        if (!found) {
            qDebug() << "Text not found: " << searchText;
            ui->textEdit->moveCursor(QTextCursor::Start); // Reset cursor to start for next search
        } else {
            qDebug() << "Text found: " << searchText;
        }
    }
}


void MainWindow::on_prevsearch_clicked()
{
    QString searchText = ui->lineEdit->text();
    if (!searchText.isEmpty()) {
        bool found = ui->textEdit->find(searchText,QTextDocument::FindBackward);

        if (!found) {
            qDebug() << "Text not found: " << searchText;
            ui->textEdit->moveCursor(QTextCursor::Start); // Reset cursor to start for next search
        } else {
            qDebug() << "Text found: " << searchText;
        }
    }
}

