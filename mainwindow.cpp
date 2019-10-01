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
    "font-weight:bold;color:#EAE4F6;"
    "font-size:30px;text-align:center;"
    "font-family:Andale Mono;";

QString header_dis_style =
    "color:#EAE4F6;"
    "font-size:20px;"
    "text-align:center;"
    "font-family:Andale Mono;";

QString text_field_style =
    "color:#ffffff;"
    "background-color:#242424;"
    "font-size:30px;"
    "border-radius:15px;";

QString btn_str_sub_style =
    "background-color:#ff7700;"
    "font-weight:bold;"
    "color:#ffffff;"
    "font-size:20px;"
    "border-radius:15px;"
    "margin:10px 100px 10px 100px;"
    "padding:20px;"
    "font-family:Andale Mono;";

QString var_input_style = "margin: 25px 10px 10px 50px;";

QString var_header_style = "margin: 45px 0px 0px 40px;";

QGridLayout* base_table;
QGridLayout* main_grid;
QGridLayout* main_menu;
QGridLayout* input_win;
QTextEdit* result_output;
QTextEdit* input_sample;
QTextEdit *var_input_field;

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
      new QLabel("Fill input with string with standart\nfunctions and submit!");
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

  // Var field
  QGridLayout *var_input = new QGridLayout;

  // Var Header
  QLabel *var_header = new QLabel("Set variables like 'a = 5' etc.");
  var_header->setStyleSheet(header_dis_style + var_header_style);

  // Add var header
  var_input->addWidget(var_header, 0, 0);

  // Var input filed
  var_input_field = new QTextEdit;
  var_input_field->setStyleSheet(text_field_style + var_input_style);

  // Add var input filed
  var_input->addWidget(var_input_field, 1, 0);

  // Add main_menu
  main_grid->addLayout(main_menu, 0, 0);
  main_grid->addLayout(var_input, 0, 1);

  // Connect
  QObject::connect(btn_sub_string, SIGNAL(pressed()), this, SLOT(get_res()));

  ui->verticalLayout->addLayout(main_grid);
  error = [](std::string s) {
    result_output->setFontPointSize(14);
    result_output->setFontFamily("Consolas");
    result_output->setText("[" + QTime::currentTime().toString() +
                           "] Error occured: " + QString::fromStdString(s));
    success = false; // FIXME log(2,16,,) cos(101,*pi)
  };
}

void fill(hashmap<std::string, double, string_hash> &vars) {
    for (QString str: var_input_field->toPlainText().split("\n")) {
        if (str.size() == 0)
            continue;

        str.replace(" ", "");

        success = true;
        int i = 0;
        std::string name = get_word(str.toStdString(), i);
        hashmap<std::string, double, string_hash> empty;
        empty.put("e", EULER);
        empty.put("pi", PI);
        stack<std::string> lol;

        if (i + 1 >= str.size()) {
            error("Wrong variable (without declaration) \"" + name + "\"");
            return;
        }

        std::string past = str.toStdString().substr(i + 1, str.size() - 1);
        qDebug() << QString::fromStdString(name) << "=" << QString::fromStdString(past) << endl;

        trim_unary(past, empty, lol);
        if (!lol.empty()) {
            error("Variables in variables!");
            return;
        }
        if (!success) {
            return;
        }

        std::string polish;
        to_polish(past, polish, empty);
        if (!success) {
            return;
        }

        vars.put(name, calc_polish(polish));
    }
}

void MainWindow::get_res() {
  success = true;
  result_output->setFontFamily("Andale Mono");

  std::string str = input_sample->toPlainText().toStdString(), polish;
  hashmap<std::string, double, string_hash> vars;
  vars.put("pi", M_PI);
  vars.put("e", M_E);
  fill(vars);

  if (!success) {
      return;
  }

  stack<std::string> new_vars;

  trim_unary(str, vars, new_vars);
  if (!success) {
    while (!new_vars.empty()) {
      QString str = QString::fromStdString(new_vars.top());
      new_vars.pop();
      result_output->setText(result_output->toPlainText() + "\n" + str + "=?");
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
