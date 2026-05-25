#include "MainWindow.h"

#include <QAction>
#include <QApplication>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QShortcut>
#include <QSplitter>
#include <QStatusBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextBrowser>
#include <QTextCursor>
#include <QToolBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QWidget>

namespace pc {

namespace {

QString runShortcutLabel() {
#ifdef Q_OS_MAC
    return QStringLiteral("⌘⏎");
#else
    return QStringLiteral("Ctrl+⏎");
#endif
}

QWidget* makeSectionHeader(const QString& title, QWidget* parent) {
    auto* w   = new QWidget(parent);
    auto* lay = new QHBoxLayout(w);
    lay->setContentsMargins(4, 8, 4, 4);
    lay->setSpacing(0);

    auto* lbl = new QLabel(title, w);
    lbl->setObjectName("sectionHeader");
    lay->addWidget(lbl, 1);
    return w;
}

QFrame* makeSeparator(QWidget* parent) {
    auto* line = new QFrame(parent);
    line->setFrameShape(QFrame::HLine);
    line->setObjectName("separator");
    line->setFixedHeight(1);
    return line;
}

}

// =========================================================================
//  Light theme
// =========================================================================
void MainWindow::applyTheme() {
    const QString qss = R"(

/* ---- Global ---- */
QMainWindow, QWidget {
    background-color: #f8f9fc;
    color: #2e3440;
}

/* ---- Section headers ---- */
QLabel#sectionHeader {
    color: #5e6687;
    font-size: 11px;
    font-weight: 700;
    letter-spacing: 1px;
}

/* ---- Separator ---- */
QFrame#separator {
    background-color: #dcdee4;
}

/* ---- Input editor ---- */
QPlainTextEdit#inputEditor {
    background-color: #ffffff;
    color: #2e3440;
    border: 1px solid #d0d3dc;
    border-radius: 8px;
    padding: 10px;
    selection-background-color: #cce5ff;
}
QPlainTextEdit#inputEditor:focus {
    border-color: #4a90d9;
}

/* ---- History browser ---- */
QTextBrowser#historyBrowser {
    background-color: #ffffff;
    color: #2e3440;
    border: 1px solid #dcdee4;
    border-radius: 8px;
    padding: 10px;
    selection-background-color: #cce5ff;
}

/* ---- Run button ---- */
QPushButton#runBtn {
    background-color: #4a90d9;
    color: #ffffff;
    border: none;
    border-radius: 8px;
    padding: 10px 24px;
    font-weight: 700;
    font-size: 14px;
}
QPushButton#runBtn:hover {
    background-color: #5da0e8;
}
QPushButton#runBtn:pressed {
    background-color: #3a7bc8;
}

/* ---- Toolbar ---- */
QToolBar {
    background-color: #f0f1f5;
    border-bottom: 1px solid #dcdee4;
    spacing: 4px;
    padding: 4px 8px;
}
QToolBar QToolButton {
    background-color: #e4e6ec;
    color: #4c566a;
    border: none;
    border-radius: 6px;
    padding: 6px 14px;
    font-size: 12px;
}
QToolBar QToolButton:hover {
    background-color: #d8dae2;
    color: #2e3440;
}

/* ---- Tree widget ---- */
QTreeWidget {
    background-color: #ffffff;
    color: #2e3440;
    border: 1px solid #dcdee4;
    border-radius: 8px;
    padding: 4px;
    outline: none;
}
QTreeWidget::item {
    padding: 3px 6px;
    border-radius: 4px;
}
QTreeWidget::item:hover {
    background-color: #eef0f5;
}
QTreeWidget::item:selected {
    background-color: #dce8f7;
    color: #2e3440;
}
QTreeWidget::branch {
    background-color: transparent;
}
QHeaderView::section {
    background-color: #eef0f5;
    color: #5e6687;
    border: none;
    padding: 4px 8px;
    font-size: 11px;
    font-weight: 600;
}

/* ---- Table widget ---- */
QTableWidget {
    background-color: #ffffff;
    color: #2e3440;
    border: 1px solid #dcdee4;
    border-radius: 8px;
    gridline-color: #eef0f5;
    outline: none;
}
QTableWidget::item {
    padding: 4px 8px;
}
QTableWidget::item:selected {
    background-color: #dce8f7;
}

/* ---- Scrollbars ---- */
QScrollBar:vertical {
    background-color: #f8f9fc;
    width: 10px;
    border-radius: 5px;
    margin: 0;
}
QScrollBar::handle:vertical {
    background-color: #c8cad2;
    border-radius: 5px;
    min-height: 30px;
}
QScrollBar::handle:vertical:hover {
    background-color: #a8aab4;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
    height: 0; background: none;
}
QScrollBar:horizontal {
    background-color: #f8f9fc;
    height: 10px;
    border-radius: 5px;
    margin: 0;
}
QScrollBar::handle:horizontal {
    background-color: #c8cad2;
    border-radius: 5px;
    min-width: 30px;
}
QScrollBar::handle:horizontal:hover {
    background-color: #a8aab4;
}
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
    width: 0; background: none;
}

/* ---- Splitter handles ---- */
QSplitter::handle {
    background-color: #dcdee4;
}
QSplitter::handle:vertical {
    height: 3px;
    margin: 4px 40px;
    border-radius: 1px;
}
QSplitter::handle:horizontal {
    width: 3px;
    margin: 40px 4px;
    border-radius: 1px;
}
QSplitter::handle:hover {
    background-color: #4a90d9;
}

/* ---- Status bar ---- */
QStatusBar {
    background-color: #f0f1f5;
    color: #7a7f8e;
    border-top: 1px solid #dcdee4;
    font-size: 12px;
    padding: 2px 8px;
}
QStatusBar QLabel {
    color: #7a7f8e;
}
)";
    setStyleSheet(qss);
}

// =========================================================================
//  Build
// =========================================================================
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Physics Calculator");
    resize(1200, 780);
    applyTheme();
    buildUi();
    refreshReference();
    refreshVariables();
    statusBar()->showMessage(
        "Ready  —  Enter expressions, then press " + runShortcutLabel() +
        " or click Run");
}

void MainWindow::buildUi() {
    QFont mono("Menlo", 13);
    mono.setStyleHint(QFont::Monospace);

    auto* central = new QWidget(this);
    auto* root    = new QVBoxLayout(central);
    root->setContentsMargins(12, 8, 12, 0);
    root->setSpacing(0);

    // =====================================================================
    //  INPUT pane
    // =====================================================================
    auto* inputPane = new QWidget(central);
    auto* ilay      = new QVBoxLayout(inputPane);
    ilay->setContentsMargins(0, 0, 0, 0);
    ilay->setSpacing(6);

    auto* inputHeader = new QHBoxLayout();
    inputHeader->addWidget(makeSectionHeader("INPUT", inputPane));
    auto* commentHint = new QLabel("lines with # are comments", inputPane);
    commentHint->setStyleSheet("color: #9a9eb0; font-size: 11px;");
    inputHeader->addWidget(commentHint);
    inputHeader->addStretch();

    evalBtn_ = new QPushButton("  Run  " + runShortcutLabel() + "  ", inputPane);
    evalBtn_->setObjectName("runBtn");
    evalBtn_->setDefault(true);
    evalBtn_->setCursor(Qt::PointingHandCursor);
    inputHeader->addWidget(evalBtn_);
    ilay->addLayout(inputHeader);

    input_ = new QPlainTextEdit(inputPane);
    input_->setObjectName("inputEditor");
    input_->setFont(mono);
    input_->setPlaceholderText(
        "# example: compute gravitational force\n"
        "mass  = 70 kg\n"
        "accel = 9.8 m/s^2\n"
        "force = mass * accel");
    input_->setTabChangesFocus(true);
    input_->setMinimumHeight(120);
    ilay->addWidget(input_);

    // =====================================================================
    //  Bottom: history + sidebar
    // =====================================================================
    auto* hSplit = new QSplitter(Qt::Horizontal, central);

    // ---- History pane ----
    auto* histPane = new QWidget(hSplit);
    auto* hlay     = new QVBoxLayout(histPane);
    hlay->setContentsMargins(0, 0, 0, 0);
    hlay->setSpacing(4);
    hlay->addWidget(makeSectionHeader("HISTORY", histPane));

    history_ = new QTextBrowser(histPane);
    history_->setObjectName("historyBrowser");
    history_->setFont(mono);
    history_->setOpenLinks(false);
    hlay->addWidget(history_, 1);

    // ---- Sidebar ----
    auto* sidePane = new QWidget(hSplit);
    auto* slay     = new QVBoxLayout(sidePane);
    slay->setContentsMargins(0, 0, 0, 0);
    slay->setSpacing(4);

    slay->addWidget(makeSectionHeader("UNITS & CONSTANTS", sidePane));
    auto* refHint = new QLabel("double-click to insert into input", sidePane);
    refHint->setStyleSheet(
        "color: #9a9eb0; font-size: 11px; margin-left: 4px;");
    slay->addWidget(refHint);

    reference_ = new QTreeWidget(sidePane);
    reference_->setHeaderLabels({"Symbol", "Description"});
    reference_->setRootIsDecorated(true);
    reference_->setColumnWidth(0, 100);
    reference_->setAnimated(true);
    slay->addWidget(reference_, 3);

    slay->addWidget(makeSeparator(sidePane));
    slay->addWidget(makeSectionHeader("YOUR VARIABLES", sidePane));

    variables_ = new QTableWidget(0, 2, sidePane);
    variables_->setHorizontalHeaderLabels({"Name", "Value"});
    variables_->verticalHeader()->setVisible(false);
    variables_->horizontalHeader()->setStretchLastSection(true);
    variables_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    variables_->setSelectionBehavior(QAbstractItemView::SelectRows);
    variables_->setShowGrid(false);
    variables_->setAlternatingRowColors(true);
    variables_->setStyleSheet(
        "QTableWidget { alternate-background-color: #f2f3f7; }");
    slay->addWidget(variables_, 2);

    hSplit->addWidget(histPane);
    hSplit->addWidget(sidePane);
    hSplit->setStretchFactor(0, 3);
    hSplit->setStretchFactor(1, 2);

    auto* vSplit = new QSplitter(Qt::Vertical, central);
    vSplit->addWidget(inputPane);
    vSplit->addWidget(hSplit);
    vSplit->setStretchFactor(0, 0);
    vSplit->setStretchFactor(1, 1);
    root->addWidget(vSplit, 1);

    setCentralWidget(central);

    // =====================================================================
    //  Toolbar
    // =====================================================================
    auto* tb = addToolBar("Main");
    tb->setMovable(false);
    tb->setToolButtonStyle(Qt::ToolButtonTextOnly);

    auto* clearInput = tb->addAction("Clear input");
    auto* clearHist  = tb->addAction("Clear history");
    auto* clearVars  = tb->addAction("Reset variables");
    tb->addSeparator();
    auto* help       = tb->addAction("Help");

    // =====================================================================
    //  Shortcuts
    // =====================================================================
    auto* runSc1 = new QShortcut(
        QKeySequence(Qt::CTRL | Qt::Key_Return), this);
    auto* runSc2 = new QShortcut(
        QKeySequence(Qt::CTRL | Qt::Key_Enter),  this);

    // =====================================================================
    //  Connections
    // =====================================================================
    connect(evalBtn_,   &QPushButton::clicked,
            this, &MainWindow::onEvaluate);
    connect(runSc1,     &QShortcut::activated,
            this, &MainWindow::onEvaluate);
    connect(runSc2,     &QShortcut::activated,
            this, &MainWindow::onEvaluate);
    connect(clearInput, &QAction::triggered,
            this, [this] { input_->clear(); input_->setFocus(); });
    connect(clearHist,  &QAction::triggered,
            this, &MainWindow::onClearHistory);
    connect(clearVars,  &QAction::triggered,
            this, &MainWindow::onClearVariables);
    connect(help,       &QAction::triggered,
            this, &MainWindow::onShowHelp);
    connect(reference_, &QTreeWidget::itemDoubleClicked,
            this, &MainWindow::onReferenceDoubleClicked);

    input_->setFocus();
}

// =========================================================================
//  Reference tree
// =========================================================================
void MainWindow::refreshReference() {
    reference_->clear();
    const QColor symColor("#2a6fdb");
    const QColor descColor("#5e6687");

    for (const auto& sec : calc_.units().sections()) {
        auto* group = new QTreeWidgetItem(reference_);
        group->setText(0, QString::fromStdString(sec.title));
        QFont gf = group->font(0);
        gf.setBold(true);
        gf.setPointSize(12);
        group->setFont(0, gf);
        group->setForeground(0, QBrush(QColor("#4a4a6a")));

        for (const auto& e : sec.entries) {
            auto* it = new QTreeWidgetItem(group);
            it->setText(0, QString::fromStdString(e.symbol));
            it->setText(1, QString::fromStdString(e.description));
            it->setForeground(0, QBrush(symColor));
            it->setForeground(1, QBrush(descColor));
            QFont sf = it->font(0);
            sf.setBold(true);
            it->setFont(0, sf);
        }
        group->setExpanded(true);
    }
}

// =========================================================================
//  Variables table
// =========================================================================
void MainWindow::refreshVariables() {
    const auto& v = calc_.variables();
    variables_->setRowCount(static_cast<int>(v.size()));
    int row = 0;
    for (const auto& [name, q] : v) {
        const bool shadows = calc_.units().has(name);
        QString displayName = QString::fromStdString(name);
        if (shadows) displayName += " ⚠";

        auto* nameItem = new QTableWidgetItem(displayName);
        auto* valItem  = new QTableWidgetItem(
            QString::fromStdString(q.toString()));

        if (shadows) {
            nameItem->setForeground(QBrush(QColor("#c4384b")));
            nameItem->setToolTip("This name shadows a built-in unit/constant");
            valItem->setToolTip("This name shadows a built-in unit/constant");
        } else {
            nameItem->setForeground(QBrush(QColor("#8b5e3c")));
        }
        valItem->setForeground(QBrush(QColor("#2a7e4f")));

        QFont nf = nameItem->font();
        nf.setBold(true);
        nameItem->setFont(nf);

        variables_->setItem(row, 0, nameItem);
        variables_->setItem(row, 1, valItem);
        ++row;
    }
}

// =========================================================================
//  History (HTML)
// =========================================================================
void MainWindow::appendHistory(const Calculator::HistoryEntry& e) {
    const QString inputColor  = "#2a6fdb";
    const QString okColor     = "#2a7e4f";
    const QString errColor    = "#c4384b";
    const QString dimColor    = "#9a9eb0";

    const QString in  = QString::fromStdString(e.input).toHtmlEscaped();
    const QString out = QString::fromStdString(e.output).toHtmlEscaped();

    QString html;
    html += QStringLiteral(
        "<div style='margin-bottom:10px; font-family:Menlo,monospace; "
        "font-size:13px;'>");

    html += QStringLiteral(
        "<span style='color:%1;'>&gt;</span> "
        "<span style='color:%2;'>%3</span><br>")
        .arg(dimColor, inputColor, in);

    if (e.isError) {
        html += QStringLiteral(
            "&nbsp;&nbsp;<span style='color:%1; font-weight:600;'>error:</span> "
            "<span style='color:%2;'>%3</span>")
            .arg(errColor, errColor, out);
    } else {
        html += QStringLiteral(
            "&nbsp;&nbsp;<span style='color:%1;'>=</span> "
            "<span style='color:%2; font-weight:600;'>%3</span>")
            .arg(dimColor, okColor, out);
    }

    html += QStringLiteral("</div>");

    if (!e.note.empty()) {
        const QString note = QString::fromStdString(e.note).toHtmlEscaped();
        html += QStringLiteral(
            "<div style='margin-left:20px; margin-bottom:10px; "
            "color:%1; font-size:12px; font-style:italic;'>"
            "%2"
            "</div>").arg("#9a9eb0", note);
    }

    history_->append(html);

    if (auto* bar = history_->verticalScrollBar())
        bar->setValue(bar->maximum());
}

// =========================================================================
//  Slots
// =========================================================================
void MainWindow::onEvaluate() {
    const QString full = input_->toPlainText();
    if (full.trimmed().isEmpty()) return;

    const auto lines = full.split('\n');
    int ran = 0, errors = 0;
    for (const QString& raw : lines) {
        QString line = raw;
        int hashAt = line.indexOf('#');
        if (hashAt >= 0) line = line.left(hashAt);
        line = line.trimmed();
        if (line.isEmpty()) continue;

        const auto entry = calc_.evaluate(line.toStdString());
        appendHistory(entry);
        if (entry.isError) ++errors;
        ++ran;
    }
    refreshVariables();
    input_->setFocus();

    QString msg = QStringLiteral("%1 expression(s) evaluated").arg(ran);
    if (errors > 0)
        msg += QStringLiteral(",  %1 error(s)").arg(errors);
    statusBar()->showMessage(msg, 5000);
}

void MainWindow::onClearHistory() {
    calc_.clearHistory();
    history_->clear();
    statusBar()->showMessage("History cleared", 3000);
}

void MainWindow::onClearVariables() {
    calc_.clearVariables();
    refreshVariables();
    statusBar()->showMessage("Variables reset to defaults", 3000);
}

void MainWindow::onReferenceDoubleClicked(QTreeWidgetItem* item, int) {
    if (!item || item->childCount() > 0) return;
    const QString sym = item->text(0);
    if (sym.isEmpty()) return;

    QTextCursor cur = input_->textCursor();
    const QString before = input_->toPlainText().left(cur.position());
    QString prefix;
    if (!before.isEmpty()) {
        QChar last = before.back();
        if (!last.isSpace() && last != '(' && last != '*' && last != '/' &&
            last != '+' && last != '-' && last != '^' && last != '=') {
            prefix = " ";
        }
    }
    cur.insertText(prefix + sym);
    input_->setTextCursor(cur);
    input_->setFocus();
}

void MainWindow::onShowHelp() {
    QMessageBox help(this);
    help.setWindowTitle("Physics Calculator — Help");
    help.setTextFormat(Qt::RichText);
    help.setText(QStringLiteral(
        "<h3>Physics Calculator</h3>"
        "<p><b>Multi-line input:</b></p>"
        "<ul>"
        "<li><b>Enter</b> — new line</li>"
        "<li><b>%1</b> or <b>Run</b> button — evaluate all lines</li>"
        "<li>Lines with <code>#</code> are comments</li>"
        "</ul>"
        "<p><b>Expressions:</b></p>"
        "<pre style='background:#eef0f5; padding:8px; border-radius:6px; "
        "color:#2a7e4f;'>"
        "9.8 m / s^2\n"
        "70 kg * 9.8 m/s^2\n"
        "(3 V) * (2 A)         =  6 W</pre>"
        "<p><b>Variables:</b></p>"
        "<pre style='background:#eef0f5; padding:8px; border-radius:6px; "
        "color:#8b5e3c;'>"
        "accel = 9.8 m/s^2\n"
        "mass  = 70 kg\n"
        "force = mass * accel</pre>"
        "<p><b>Operators:</b> <code>+ - * / ^</code> "
        "(powers are right-associative)</p>"
        "<p>Implicit multiplication: "
        "<code>9.8 m/s^2</code> = <code>9.8 * m / s^2</code></p>"
        "<p>Double-click sidebar items to insert them.</p>"
    ).arg(runShortcutLabel()));
    help.exec();
}

}
