#include "usersignup.h"
#include "ui_usersignup.h"
// #include "loginscreen.h"

usersignup::usersignup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::usersignup)
{
    ui->setupUi(this);
}

usersignup::~usersignup()
{
    delete ui;
}

void usersignup::on_pushButton_2_clicked()
{
    emit backToLogin();
    this->close();
}

void usersignup::on_pushButton_clicked()
{
    QString username = ui->usernameEnter->text();
    QString password = ui->passwordEnter->text();
    // A created account will be a customer.
    // Restaurats will have manager accounts created on restaurant creation (in app)
    emit accountCreated(username, password, "customer");
    emit backToLogin();
    this ->close();

}

