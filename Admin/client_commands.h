// Client_commands.h
#ifndef CLIENT_COMMANDS_H
#define CLIENT_COMMANDS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "client.h"
#include <QSharedPointer>


namespace Ui {
class Client_commands;
}

class Client_commands : public QDialog
{
    Q_OBJECT

public:
    explicit Client_commands(QWidget *parent = nullptr);
    ~Client_commands();
    void start_client_box(QSharedPointer<Client> client);
    void setLabelText();
    void add_buttons();
    bool set_client(QSharedPointer<Client> client);
    void add_history();
    void add_last_message();
    void create_row_buttons(int number,QList<QString>names);
    // Public method to access the UI
    Ui::Client_commands *getUI() const;
    void disableLayout(QLayout *layout);
    void enableLayout(QLayout *layout);
    void update_status(QString status);

signals:
    void window_opend(QSharedPointer<Client> client);
    void send_to_client(QString message);
private slots:

    void on_btn_manual_2_clicked();

    void on_btn_automat_clicked();

    void on_btn_manual_clicked();
    void on_btn_start_clicked();

    void on_btn_status_clicked();

    void on_btn_stop_clicked();

public slots:
    void onButtonClicked();
private:
    Ui::Client_commands *ui;
    QSharedPointer<Client> _client;
    QVBoxLayout* mainLayout;  // Added mainLayout member
};

#endif // CLIENT_COMMANDS_H
