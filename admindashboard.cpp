#include "admindashboard.h"
#include "ui_admindashboard.h"
using namespace std;

AdminDashboard::AdminDashboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminDashboard)
{
    ui->setupUi(this);
    //ui->stackedWidget->setCurrentIndex(0);
    //ui->comboBox->setItemData(0, 0, Qt::UserRole - 1);

    on_dashboardBtn_clicked();
    ui->date->setDate(QDate::currentDate());
    ui->bookDate->setDate(QDate::currentDate());

    this->setStyleSheet("QMessageBox::QLabel{font-size: 18px; color: #fff; font-weight: 400; font-family: 'Poppins';} QMessageBox::QPushButton{color: #fff; font-family: 'Poppins' } QCalendarWidget { background: #777; }");

    QValidator* intValidator = new QIntValidator(this);
    ui->phone_number->setValidator(intValidator);
    ui->updatephone_number->setValidator(intValidator);


    QString path_to_database = QCoreApplication::applicationDirPath() + QDir::separator() + "database" + QDir::separator() + "database.db";

    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(path_to_database);

    if(DB.open()) {
        qDebug() << "Database connected.";
    }
    else {
        qDebug() << "Database not connected.";
        qDebug() << "Error: " << DB.lastError();
    }
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
}

void AdminDashboard::refreshTable() {
    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM user_details");
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(1,200);
    ui->tableWidget->setColumnWidth(2,120);
    ui->tableWidget->setColumnWidth(3,200);
    ui->tableWidget->setColumnWidth(4,180);
    ui->tableWidget->setColumnWidth(5,125);
    ui->tableWidget->setColumnWidth(6,125);

    if (query.exec()) {
        int RowNumber = 0;

        while(query.next())
        {
            ui->tableWidget->insertRow(RowNumber);
            ui->tableWidget->setRowHeight(RowNumber,40);
            ui->tableWidget->setItem(RowNumber, 0, new QTableWidgetItem(QString("%1-%2").arg(QDate::currentDate().year()).arg(query.value("id").toString())));
            ui->tableWidget->setItem(RowNumber, 1, new QTableWidgetItem(QString(query.value("name").toString())));
            ui->tableWidget->setItem(RowNumber, 2, new QTableWidgetItem(QString(query.value("gender").toString())));
            ui->tableWidget->setItem(RowNumber, 3, new QTableWidgetItem(QString(query.value("phone_number").toString())));
            ui->tableWidget->setItem(RowNumber, 4, new QTableWidgetItem(QString(query.value("address").toString())));
            ui->tableWidget->setItem(RowNumber, 5, new QTableWidgetItem(QString(query.value("service").toString())));
            ui->tableWidget->setItem(RowNumber, 6, new QTableWidgetItem(QString(query.value("date").toString())));
            ui->tableWidget->setItem(RowNumber, 7, new QTableWidgetItem(QString(query.value("time").toString())));
            RowNumber++;
        }
    } else {
        // Login failed, show error message
        qDebug() << "Data loading to table error.";
    }

    DB.close();
}

void AdminDashboard::on_logoutBtn_clicked()
{
    deselectedPushButton(ui->dashboardBtn);
    deselectedPushButton(ui->bookingBtn);
    deselectedPushButton(ui->membersBtn);
    deselectedPushButton(ui->notificationsBtn);
    selectedPushButton(ui->logoutBtn);
    LoginPage *loginpage = new LoginPage();
    loginpage->showMaximized();
    this->close();
}


void AdminDashboard::on_dashboardBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    selectedPushButton(ui->dashboardBtn);
    deselectedPushButton(ui->bookingBtn);
    deselectedPushButton(ui->membersBtn);
    deselectedPushButton(ui->notificationsBtn);
    deselectedPushButton(ui->logoutBtn);
}


void AdminDashboard::on_membersBtn_clicked()
{
    refreshTable();
    ui->stackedWidget->setCurrentIndex(3);
    deselectedPushButton(ui->dashboardBtn);
    deselectedPushButton(ui->bookingBtn);
    selectedPushButton(ui->membersBtn);
    deselectedPushButton(ui->notificationsBtn);
    deselectedPushButton(ui->logoutBtn);
}


void AdminDashboard::on_bookingBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    on_showBookingsBtn_clicked();
    deselectedPushButton(ui->dashboardBtn);
    selectedPushButton(ui->bookingBtn);
    deselectedPushButton(ui->membersBtn);
    deselectedPushButton(ui->notificationsBtn);
    deselectedPushButton(ui->logoutBtn);
}


void AdminDashboard::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void AdminDashboard::on_pushButton_updateInfo_clicked()
{
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "No row selected.";
        QMessageBox::warning(this, "Invalid selection","No row is selected.",QMessageBox::Ok);
        return;
    }
    ui->stackedWidget->setCurrentIndex(2);
    int selectedRow = selectedIndexes.first().row();
    string id = ui->tableWidget->item(selectedRow, 0)->text().toStdString();
    id = id.substr(id.find("-") + 1);

    ui->updateid->setText(QString::fromStdString(id));
    ui->updatename->setText(ui->tableWidget->item(selectedRow, 1)->text());
    ui->updatephone_number->setText(ui->tableWidget->item(selectedRow, 3)->text());
    ui->updateaddress->setText(ui->tableWidget->item(selectedRow, 4)->text());

    if(ui->tableWidget->item(selectedRow, 2)->text() == "Male") {
        ui->maleradioButton1->setChecked(true);
    }
    else if(ui->tableWidget->item(selectedRow, 2)->text() == "Female") {
        ui->femaleradioButton1->setChecked(true);
    }
    else if(ui->tableWidget->item(selectedRow, 2)->text() == "Others") {
        ui->othersradioButton1->setChecked(true);
    }
    ui->updateservice->setCurrentText(ui->tableWidget->item(selectedRow, 5)->text());
    ui->updateDate->setDate(QDate::fromString(ui->tableWidget->item(selectedRow, 6)->text(), "yyyy/M/d"));
    ui->updatetimeSlot->setCurrentText(ui->tableWidget->item(selectedRow, 7)->text());
}

void AdminDashboard::on_pushButton_addBooking_clicked()
{

    QString name, gender, phone_number, address, service, date, time;

    name = ui->name->text();
    if(ui->femaleradioButton->isChecked()) {
        gender = ui->femaleradioButton->text();
    }
    else if(ui->maleradioButton->isChecked()) {
        gender = ui->maleradioButton->text();
    }
    else if(ui->othersradioButton->isChecked()) {
        gender = ui->othersradioButton->text();
    }

    phone_number = ui->phone_number->text();
    address = ui->address->text();
    service = ui->service->currentText();
    date = ui->date->text();
    time = ui->timeSlot->currentText();


    qDebug() << name << gender << phone_number << address << service << date << time;

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }


    if(!name.isEmpty() && !gender.isEmpty() && !phone_number.isEmpty() && !address.isEmpty() && !service.isEmpty() && !date.isEmpty() && !time.isEmpty() && phone_number.length()==10) {
        if(checkCapacity(service, date, time)) {
            QSqlQuery query;
            query.prepare("INSERT INTO user_details (name, gender, phone_number, address, service, date, time) VALUES (:name, :gender, "
                          ":phone_number, :address, :service, :date, :time)");
            query.bindValue(":name", name);
            query.bindValue(":gender", gender);
            query.bindValue(":phone_number", phone_number);
            query.bindValue(":address", address);
            query.bindValue(":service", service);
            query.bindValue(":date", date);
            query.bindValue(":time", time);

            if (query.exec()) {
                qDebug() << "Data inserted successfully";
                QMessageBox::information(this,"Successful", "User inserted successfully.", QMessageBox::Ok);
                ui->name->clear();
                ui->phone_number->clear();
                ui->address->clear();
                ui->maleradioButton->setChecked(false);
                ui->femaleradioButton->setChecked(false);
                ui->othersradioButton->setChecked(false);
                ui->service->setCurrentIndex(-1);
                ui->date->setDate(QDate::currentDate());
                ui->timeSlot->setCurrentIndex(-1);
            } else {
                qDebug() << "Error: " << query.lastError().text();
            }
        } else {
            QMessageBox::warning(this, "Booking error", "The selected time slot is full for the chosen service.", QMessageBox::Ok);
        }
    } else {
        QMessageBox::warning(this,"Booking error", "All fields are required and cannot be empty.", QMessageBox::Ok);
    }

    DB.close();

}


bool AdminDashboard::checkCapacity(const QString &service, const QString &date, const QString &time) {
    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
    }

    // Query to get the counts for each service, date, and time
    QSqlQuery query;
    query.prepare("SELECT * FROM user_details WHERE service = :service AND date = :date AND time = :time");
    query.bindValue(":service", service);
    query.bindValue(":date", date);
    query.bindValue(":time", time);

    int count = 0;
    if (query.exec()) {
        while(query.next())
        {
            count++;
        }
    } else {
        qDebug() << "Error: " << query.lastError().text();
        return false; // Error in querying the database
    }

    // Set the maximum capacity based on service type
    int maxCapacity = 0;
    if (service == "Swimming") {
        maxCapacity = swimmingCapacity;
    } else if (service == "Sauna") {
        maxCapacity = saunaCapacity;
    } else if (service == "Spa") {
        maxCapacity = spaCapacity;
    }

    // checking the capacity is full or not.
    if(count < maxCapacity) {
        return true;
    } else {
        return false;
    }
}


void AdminDashboard::selectedPushButton(QPushButton *button)
{
    button->setStyleSheet("QPushButton { background: #307970; color:#fff; text-align: left; padding-left:20; border: none; border-left:6px solid #2fc5b3; outline: none; } QPushButton:hover { background: #307970; }");
}

void AdminDashboard::deselectedPushButton(QPushButton *button)
{
    button->setStyleSheet("QPushButton { background: #2fc5b3; color:#fff; text-align: left; padding-left:20; border: none; outline: none; } QPushButton:hover { background: #307970; }");
}






void AdminDashboard::on_refreshtableBtn_clicked()
{
    refreshTable();
}

//delete record
void AdminDashboard::on_pushButton_clicked()
{
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "No row selected.";
        QMessageBox::warning(this, "Invalid selection","No row is selected.",QMessageBox::Ok);
        return;
    }

    int selectedRow = selectedIndexes.first().row();

    string id = ui->tableWidget->item(selectedRow, 0)->text().toStdString();
    id = id.substr(id.find("-") + 1);
    QString idToDelete = QString::fromStdString(id);
    qDebug() << idToDelete;

    if (QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this record?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        // Perform the deletion here using the ID or other identifying information
        if (!DB.open()) {
            qDebug() << "Failed to open the database.";
            return;
        }

        QSqlQuery query;
        query.prepare("DELETE FROM user_details WHERE id = :id");
        query.bindValue(":id", idToDelete);

        if (!query.exec()) {
            qDebug() << "Error deleting record:" << query.lastError().text();
        }

        DB.close();

        // Refresh the table after deletion
        refreshTable();
    }
}


void AdminDashboard::on_pushButton_updateBooking_clicked()
{
    QString name, gender, phone_number, address, service, date, id, time;

    name = ui->updatename->text();
    if(ui->femaleradioButton1->isChecked()) {
        gender = ui->femaleradioButton1->text();
    }
    else if(ui->maleradioButton1->isChecked()) {
        gender = ui->maleradioButton1->text();
    }
    else if(ui->othersradioButton1->isChecked()) {
        gender = ui->othersradioButton1->text();
    }

    phone_number = ui->updatephone_number->text();
    address = ui->updateaddress->text();
    service = ui->updateservice->currentText();
    date = ui->updateDate->text();
    id = ui->updateid->text();
    time = ui->updatetimeSlot->currentText();

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }


    if(!name.isEmpty() && !gender.isEmpty() && !phone_number.isEmpty() && !address.isEmpty() && !service.isEmpty() && !date.isEmpty() && !time.isEmpty() && phone_number.length()==10) {
        if(checkCapacity(service,date,time)) {
            QSqlQuery query;

            query.prepare("UPDATE user_details SET name = :name, gender = :gender, phone_number = :phone_number, address = :address, service = :service, date = :date, time = :time WHERE id = :id");
            query.bindValue(":name", name);
            query.bindValue(":gender", gender);
            query.bindValue(":phone_number", phone_number);
            query.bindValue(":address", address);
            query.bindValue(":service", service);
            query.bindValue(":date", date);
            query.bindValue(":id",id);
            query.bindValue(":time",time);

            if (query.exec()) {
                qDebug() << "Data updated successfully";
                QMessageBox::information(this,"Successful", "User information updated successfully.", QMessageBox::Ok);
                ui->updatename->clear();
                ui->updateid->clear();
                ui->updatephone_number->clear();
                ui->updateaddress->clear();
                ui->maleradioButton1->setChecked(false);
                ui->femaleradioButton1->setChecked(false);
                ui->othersradioButton1->setChecked(false);
                ui->updateservice->setCurrentIndex(-1);
                ui->updateDate->setDate(QDate::currentDate());
                ui->updatetimeSlot->setCurrentIndex(-1);
                refreshTable();
                ui->stackedWidget->setCurrentIndex(3);

            } else {
                qDebug() << "Error: " << query.lastError().text();
            }
        } else {
            QMessageBox::warning(this, "Booking error", "The selected time slot is full for the chosen service.", QMessageBox::Ok);
        }
    } else {
        QMessageBox::warning(this,"Updating error", "All fields are required and cannot be empty.", QMessageBox::Ok);
    }

    DB.close();
}

void AdminDashboard::on_showBookingsBtn_clicked()
{
    QString date, time;
    date = ui->bookDate->text();
    time = ui->bookTimeSlot->currentText();
    int count_Swimming = 0, count_Sauna = 0, count_Spa = 0;

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM user_details WHERE date = :date AND time = :time");
    query.bindValue(":date", date);
    query.bindValue(":time", time);

    if (query.exec()) {

        while(query.next())
        {
            if (query.value("service").toString() == "Swimming") {
                count_Swimming++;
            }
            else if(query.value("service").toString() == "Sauna") {
                count_Sauna++;
            }
            else if(query.value("service").toString() == "Spa") {
                count_Spa++;
            }
        }
        ui->countSwimming->setText("Booked: " + QString::number(count_Swimming));
        ui->countSauna->setText("Booked: " + QString::number(count_Sauna));
        ui->countSpa->setText("Booked: " + QString::number(count_Spa));
        ui->remainingSwimming->setText("Remaining: " + QString::number(swimmingCapacity - count_Swimming));
        ui->remainingSauna->setText("Remaining: " + QString::number(saunaCapacity - count_Sauna));
        ui->remainingSpa->setText("Remaining: " + QString::number(spaCapacity - count_Spa));

    } else {
        // Login failed, show error message
        qDebug() << "Data loading to bookings error.";
    }

    DB.close();
}







