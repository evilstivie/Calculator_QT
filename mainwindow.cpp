#include "mainwindow.h"
#include "algo.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QTableWidget>
#include <QTextEdit>
#include <QTime>

static MainWindow* mw;

/* Styles */
QString bg_style = "background-color:#030308;";
QString header_style =
    "font-weight:bold;color:#EAE4F6;font-size:30px;text-align:center;font-"
    "family:Andale Mono;";
QString header_dis_style =
    "color:#EAE4F6;font-size:20px;text-align:center;font-family:Andale Mono;";
QString text_field_style =
    "color:#ffffff;background-color:#242424;font-size:30px;border-radius:15px;";
QString btn_str_sub_style =
    "background-color:#ff7700;font-weight:bold;color:#ffffff;font-size:20px;"
    "border-radius:15px;margin:10px 150px 10px "
    "150px;padding:20px;font-family:Andale Mono;";

QGridLayout* base_table;
QGridLayout* main_grid;
QGridLayout* main_menu;
QGridLayout* input_win;
QTextEdit* result_output;
QTextEdit* input_sample;

bool success = true;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  mw = this;
  setStyleSheet(bg_style);

  main_grid = new QGridLayout;

  /* Main menu, input window, error window */
  main_menu = new QGridLayout;

  // Header
  QLabel* prog_header_main = new QLabel("String-calculator");
  prog_header_main->setStyleSheet(header_style);
  QLabel* prog_header_dis =
      new QLabel("Fill input with string with standart functions and submit!");
  prog_header_dis->setStyleSheet(header_dis_style);

  // Add headers
  main_menu->addWidget(prog_header_main, 0, 0);
  main_menu->addWidget(prog_header_dis, 1, 0);

  // Input window
  input_win = new QGridLayout;
  input_sample = new QTextEdit;
  input_sample->setStyleSheet(text_field_style);
  input_win->addWidget(input_sample, 0, 0);

  // Add input window
  main_menu->addLayout(input_win, 2, 0);

  // Result Output
  result_output = new QTextEdit;
  result_output->setStyleSheet(text_field_style);

  // Add result output
  main_menu->addWidget(result_output, 3, 0);

  // Submit String Button
  QPushButton* btn_sub_string = new QPushButton("Submit string");
  QVariant pb_res("sb_str");
  btn_sub_string->setProperty("op", pb_res);
  btn_sub_string->setStyleSheet(btn_str_sub_style);

  // Add submit string button
  main_menu->addWidget(btn_sub_string, 4, 0);

  // Add main_menu
  main_grid->addLayout(main_menu, 0, 0);

  // Connect
  QObject::connect(btn_sub_string, SIGNAL(pressed()), this, SLOT(get_res()));

  ui->verticalLayout->addLayout(main_grid);
  error = [](std::string s) {
    result_output->setFontPointSize(14);
    result_output->setFontFamily("Consolas");
    result_output->setText("[" + QTime::currentTime().toString() +
                           "] Error occured: " + QString::fromStdString(s));
    success = false;
  };
}

void MainWindow::get_res() {
  success = true;
  result_output->setFontFamily("Andale Mono");

  std::string str = input_sample->toPlainText().toStdString(), polish;
  hashmap<std::string, double, string_hash> vars;
  vars.put("pi", M_PI);
  vars.put("e", M_E);
  stack<std::string> new_vars;

  trim_unary(str, vars, new_vars);
  if (!success) {
    qDebug() << new_vars.size() << endl;
    while (!new_vars.empty()) {
      QString str = QString::fromStdString(new_vars.top());
      new_vars.pop();
      qDebug() << "Variable " << str << endl;
    }
    return;
  }

  qDebug() << "pre-proccess " << QString::fromStdString(str) << endl;

  to_polish(str, polish, vars);
  if (!success)
    return;

  qDebug() << "Польска запшиш " << QString::fromStdString(polish) << endl;

  double res = calc_polish(polish);
  if (!success)
    return;

  result_output->setStyleSheet(text_field_style);
  result_output->setText("Polish notation: " + QString::fromStdString(polish) +
                         "\nResult = " + QString::number(res));
}

MainWindow::~MainWindow() {
  ui->setupUi(this);
  delete ui;
}
