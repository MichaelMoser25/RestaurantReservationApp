#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QDialog>

#include "restaurant.h"
#include "usersignup.h"

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QDialog
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr);
    QString validateCredentials(const QString& enteredUsername, const QString& enteredPassword);
    void createAccount(QString Username, QString Password, QString Permission);


    ~LoginScreen();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_signup_clicked();
    void showLoginScreen();

private:
    Ui::LoginScreen *ui;
    Restaurant *res;
    usersignup *signup;
    QStringList localCredentials;
};

#endif // LOGINSCREEN_H
