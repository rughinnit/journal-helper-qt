#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Refresh_clicked();
    void handleJournalOutput();
    void on_pushButton_Final_clicked();

    // void on_checkBox_checkStateChanged(const Qt::CheckState &arg1);

    void on_nextsearch_clicked();

    void on_prevsearch_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
