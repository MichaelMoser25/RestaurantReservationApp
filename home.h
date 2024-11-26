#ifndef HOME_H
#define HOME_H

#include <QDateTime>
#include <QDialog>
#include <QMap>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QStyle>
#include <QTextEdit>
#include <QLineEdit>
#include <QGridLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QFileDialog>
#include <QTextStream>
#include <functional>
#include <QClipboard>
#include <QMessageBox>

namespace Ui {
class Home;
}

class TableInfo
{
public:
    int seats;
    bool isReserved;
    QDateTime reservationTime;
    QString customerName;
    QList<QDateTime> reservedTimes;
    bool isVIP;
    QString specialNotes;
    double minSpend;

    TableInfo(int s = 4)
        : seats(s)
        , isReserved(false)
        , isVIP(s >= 8)
        , minSpend(0.0)
    {}
};

class Home : public QDialog
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr, const QString &userMode = "", const QString &userName = "");
    ~Home() override;

    void setUserMode(const QString &userMode);
    bool reserveTable(int tableNumber, const std::string &name, const std::string &date, int guests);
    bool cancelReservation(int tableNumber);
    bool checkAvailability(int tableNumber);
    std::string displayTableStatus();

private slots:
    void on_ReservationList_clicked();
    void on_FloorPlan_clicked();
    void on_Reserve_clicked();
    void handleTableClick();
    void on_Logout_clicked();
    void updateTableStatus();
    void on_Dashboard_clicked();
    void on_Orders_clicked();
    void on_Menu_clicked();
    void on_Locations_clicked();
    void on_Table1_list_currentTextChanged(const QString &text);
    void onFilterChanged();  // New slot for filter changes

private:
    // Define the function types for reservation filtering and sorting
    using ReservationFilter = std::function<bool(const TableInfo&, const QDateTime&)>;

    // Member variables
    Ui::Home *ui;
    QString m_userMode;
    QString m_userName;
    QMap<QString, TableInfo> tables;
    QPushButton *selectedTable;
    bool m_sortAscending;

    // UI elements
    QLineEdit* nameInput;
    QLineEdit* phoneInput;
    QTextEdit* specialRequests;
    QWidget* rightPanel;
    QWidget* reservationsPage;
    QWidget* bookingPage;
    QLineEdit* reservationSearchBox;
    QComboBox* reservationStatusFilter;
    QComboBox* reservationDateFilter;
    QComboBox* reservationTypeFilter;
    QComboBox* timeFilter;
    QComboBox* capacityFilter;
    QWidget* contactPage;


    // Basic setup functions
    void setupUI();
    void setupTables();
    void setupConnections();
    void resetTableStyles();
    void updateTableAppearance(QPushButton *table);
    void showReservationPrompt(QPushButton *table);
    void loadReservations();
    void saveReservations();
    bool isTableAvailable(const QString &tableId, const QDateTime &requestedTime);
    void populateTimeSlots();
    void resetTableStyle(QPushButton *table);
    void cleanupNavigation();
\


    // UI enhancement functions
    void setupTableCapacityIndicators();
    void setupFilters();
    void setupLegend();
    void setupQuickStats();
    void showTableDetails(QPushButton* table);
    void setupReservationControls();
    void filterReservations();
    void toggleReservationSort();
    void exportReservations();
    void loadUserReservations(const ReservationFilter& filter = [](const TableInfo&, const QDateTime&) { return true; });

    // Page management
    void setupReservationsPage();
    void setupBookingPage();
    void showReservationsPage();
    void showBookingPage();
    void setupContactPage();
    void showContactPage();


    // Calculation functions
    int calculateActiveReservations();
    int calculateDailyRevenue();
    int countVIPReservations();


    // Other variables
    int totalTables;
    std::map<int, bool> tableStatus;
    std::map<int, std::string> reservationDetails;
    int totalReserved = 0;
};

#endif // HOME_H
