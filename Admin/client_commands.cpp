// Client_commands.cpp
#include "client_commands.h"
#include "ui_client_commands.h"
#include <QDebug>

Client_commands::Client_commands(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client_commands),
    mainLayout(new QVBoxLayout)  // Initialize mainLayout
{
    ui->setupUi(this);
    setLayout(mainLayout);  // Set mainLayout as the layout for the main window


}

Client_commands::~Client_commands()
{
    delete ui;
}

void Client_commands::start_client_box(QSharedPointer<Client> client)
{
    if (set_client(client)) {
        setLabelText();
        add_buttons();
        add_history();
        add_last_message();
        update_status(client->status);
    } else {
        qInfo() << "Client doesn't exist";
        return;
    }
}

void Client_commands::setLabelText()
{
    ui->lb_slave_name->setText(_client->name);
}


void Client_commands::update_status(QString status){
    if(status == "NONE"){
        return;
    }
    QList<QString> status_parts = status.split('-');
    if(status_parts[0].toLower() == "automat"){
        ui->btn_automat->setChecked(true);
        ui->btn_manual->setChecked(false);
        disableLayout(ui->verticalLayout);
        enableLayout(ui->verticalLayout_a);
    }else if(status_parts[0].toLower() == "manual"){
        ui->btn_manual->setChecked(true);
        ui->btn_automat->setChecked(false);
        disableLayout(ui->verticalLayout_a);
        enableLayout(ui->verticalLayout);
    }
}


void Client_commands::add_buttons()
{
    if(_client->manual.size() == 0 ||  _client->automat.size() == 0 ){
        qInfo()<<"Buttons error, auto or manual empty";
        return;
    }
    ui->btn_automat->setCheckable(true);
    ui->btn_manual->setCheckable(true);
    // Get the existing vertical layout from the UI
    QVBoxLayout* existingLayout_manual = ui->verticalLayout;
    QVBoxLayout* existingLayout_automat = ui->verticalLayout_a;

    QList<QString>manual_names = _client->manual;
    if(!manual_names.isEmpty() && manual_names[0]!="NONE"){
        // Create a new horizontal layout for each manual row
        QHBoxLayout* rowLayout_m = new QHBoxLayout;
        rowLayout_m->setAlignment(Qt::AlignTop);
        // Add 3 buttons to each row
        for (int j = 0; j < manual_names.size(); ++j) {
            QPushButton* btn = new QPushButton(manual_names[j], this);
            btn->setStyleSheet("QPushButton {"
                               "background-color: rgb(181, 181, 181);"
                               "color: rgb(255, 255, 255);"
                               "border-radius: 8px;"
                               "padding: 8px 10px;"
                               "}"

                               "QPushButton:enabled {"
                               "background-color: rgb(85, 85, 85);"
                               "color: rgb(255, 255, 255);"
                               "}"

                               "QPushButton:disabled {"
                               "background-color: rgb(181, 181, 181);"
                               "color: rgb(255, 255, 255);"
                               "}"
                               "QPushButton:pressed {"
                               "background-color: rgb(50, 50, 50);"
                               "color: rgb(255, 255, 255);"
                               "}");
            btn->setCursor(Qt::PointingHandCursor);
            // Connect the clicked signal to the slot function
            connect(btn, &QPushButton::clicked, this, &Client_commands::onButtonClicked);
            rowLayout_m->addWidget(btn);
        }

        // Add the row layout to the existing vertical layout
        existingLayout_manual->addLayout(rowLayout_m);


    }


    QList<QString>auto_names  = _client->automat;

    if(!auto_names.isEmpty() && auto_names[0]!="NONE"){
        // Create a new horizontal layout for each manual row
        QHBoxLayout* rowLayout_a = new QHBoxLayout;
        rowLayout_a->setAlignment(Qt::AlignTop);

        // Add 3 buttons to each row
        for (int j = 0; j < auto_names.size(); ++j) {
            QPushButton* btn = new QPushButton(auto_names[j], this);
            btn->setStyleSheet("QPushButton {"
                               "background-color: rgb(181, 181, 181);"
                               "color: rgb(255, 255, 255);"
                               "border-radius: 8px;"
                               "padding: 8px 10px;"
                               "}"

                               "QPushButton:enabled {"
                               "background-color: rgb(85, 85, 85);"
                               "color: rgb(255, 255, 255);"
                               "}"

                               "QPushButton:disabled {"
                               "background-color: rgb(181, 181, 181);"
                               "color: rgb(255, 255, 255);"
                               "}"
                               "QPushButton:pressed {"
                               "background-color: rgb(50, 50, 50);"
                               "color: rgb(255, 255, 255);"
                               "}");
            btn->setCursor(Qt::PointingHandCursor);
            rowLayout_a->addWidget(btn);
        }

        // Add the row layout to the existing vertical layout
        existingLayout_automat->addLayout(rowLayout_a);
    }




}

bool Client_commands::set_client(QSharedPointer<Client> client)
{
    if (client) {
        _client = client;
        return true;
    }
    return false;
}

void Client_commands::add_history()
{
    ui->chat_history->addItems(_client->message_history);
}

void Client_commands::add_last_message()
{
    if (!_client->message_history.isEmpty()) {
        const QString& lastMessage = _client->message_history.last();
        ui->lstConsole->addItem(lastMessage);
    }
}

Ui::Client_commands *Client_commands::getUI() const
{
    return ui;
}


// Helper function to disable all widgets in a layout
void Client_commands::disableLayout(QLayout *layout)
{
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem *item = layout->itemAt(i);
        if (item->widget()) {
            item->widget()->setEnabled(false);
        } else if (item->layout()) {
            disableLayout(item->layout());
        }
    }
}

// Helper function to disable all widgets in a layout
void Client_commands::enableLayout(QLayout *layout)
{
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem *item = layout->itemAt(i);
        if (item->widget()) {
            item->widget()->setEnabled(true);
        } else if (item->layout()) {
            enableLayout(item->layout());
        }
    }
}

void Client_commands::on_btn_automat_clicked()
{

    ui->btn_automat->setChecked(true);
    ui->btn_manual->setChecked(false);
    disableLayout(ui->verticalLayout);
    enableLayout(ui->verticalLayout_a);
    emit send_to_client(_client->name + ":automat");

}



void Client_commands::on_btn_manual_clicked()
{
    ui->btn_manual->setChecked(true);
     ui->btn_automat->setChecked(false);
    disableLayout(ui->verticalLayout_a);
     enableLayout(ui->verticalLayout);
    emit send_to_client(_client->name + ":manual");
}

void Client_commands::onButtonClicked()
{
     QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
     if (senderButton) {
        QString buttonName = senderButton->text();
        emit send_to_client(_client->name + ":" + buttonName);

     }
}


void Client_commands::on_btn_start_clicked()
{
    emit send_to_client(_client->name + ":start");
}


void Client_commands::on_btn_status_clicked()
{
    emit send_to_client(_client->name + ":status");
}


void Client_commands::on_btn_stop_clicked()
{
    emit send_to_client(_client->name + ":stop");
}

