#pragma once

#include "core/Calculator.h"

#include <QMainWindow>

class QPlainTextEdit;
class QPushButton;
class QTextBrowser;
class QTreeWidget;
class QTreeWidgetItem;
class QTableWidget;
class QLabel;

namespace pc {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onEvaluate();
    void onClearHistory();
    void onClearVariables();
    void onReferenceDoubleClicked(QTreeWidgetItem* item, int column);
    void onShowHelp();

private:
    void buildUi();
    void applyTheme();
    void refreshReference();
    void refreshVariables();
    void appendHistory(const Calculator::HistoryEntry& e);

    Calculator      calc_;

    QPlainTextEdit* input_       = nullptr;
    QPushButton*    evalBtn_     = nullptr;
    QTextBrowser*   history_     = nullptr;
    QTreeWidget*    reference_   = nullptr;
    QTableWidget*   variables_   = nullptr;
    QLabel*         statusLabel_ = nullptr;
};

}
