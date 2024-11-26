#include "loginscreen.h"
#include <QMessageBox>
#include "restaurant.h"
#include "ui_loginscreen.h"
#include "usersignup.h"
#include "home.h"

LoginScreen::LoginScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginScreen)
{
    ui->setupUi(this);
    //Update local username and password stores with DB
    localCredentials.append("test_customer|1234|customer");
    localCredentials.append("test_manager|1234|manager");
    localCredentials.append("Adam|1234|customer");
}

LoginScreen::~LoginScreen()
{
    delete ui;
}

QString LoginScreen::validateCredentials(const QString& enteredUsername, const QString& enteredPassword){
    QString storedUsername;
    QString storedPassword;
    QString storedPermissions;
    QString toReturn = "";

    for(const QString& entry : LoginScreen::localCredentials){
        QStringList creds = entry.split("|");
        if(creds.size()==3){
            storedUsername = creds.at(0);
            storedPassword = creds.at(1);
            if(storedUsername==enteredUsername && storedPassword==enteredPassword){
                storedPermissions = creds.at(2);
                toReturn = enteredUsername+"|"+storedPermissions;
                return toReturn;
            } else if(storedUsername==enteredUsername){
                return "PASSWORD";
            }
        }
    }
    return "INVALID";
}


void LoginScreen::on_pushButton_login_clicked()
{
    QString username, password;

    username = ui->lineEdit_username->text();
    password = ui->lineEdit_password->text();

    QString loginStatus = validateCredentials(username, password);

    if(loginStatus=="INVALID"){
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setWindowTitle("Login Error");
        messageBox.setText("Invalid credentials");
        messageBox.exec();

    } else if(loginStatus=="PASSWORD"){
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setWindowTitle("Login Error");
        messageBox.setText("Incorrect password");
        messageBox.exec();

    } else {
        QStringList userToLogIn = loginStatus.split("|");
        QString validUsername = userToLogIn.at(0);
        QString userPermissions = userToLogIn.at(1);

        auto res = new Home(nullptr, userPermissions, validUsername);
        res->show();
        this->close();

    }

}

void LoginScreen::on_pushButton_signup_clicked()
{
    signup = new usersignup(nullptr);
    connect(signup, &usersignup::backToLogin, this, &LoginScreen::showLoginScreen);
    connect(signup, &usersignup::accountCreated, this, &LoginScreen::createAccount);

    signup->show();
    this->hide();
}

void LoginScreen::showLoginScreen()
{
    this->show();
}

void LoginScreen::createAccount(QString Username, QString Password, QString Permission){

    QString credentials = Username+"|"+Password+"|"+Permission;
    LoginScreen::localCredentials.append(credentials);
    this->show();

}
