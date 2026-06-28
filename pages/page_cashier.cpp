#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "restaurantapp.h"

// 5. KASIR PAGE
void RestaurantApp::createCashierPage() {
    cashierPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(cashierPage);
    layout->setContentsMargins(30, 20, 30, 20);
    layout->setSpacing(15);

    QLabel *title = new QLabel("💳  Dashboard Kasir — Antrean Pembayaran");
    title->setObjectName("pageHeader");

    // Panel dua kolom: kiri = daftar pesanan, kanan = detail
    QHBoxLayout *panelLayout = new QHBoxLayout;
    panelLayout->setSpacing(20);

    // --- Kiri: Daftar antrian pesanan ---
    QVBoxLayout *leftCol = new QVBoxLayout;
    leftCol->setSpacing(8);

    QLabel *lblList = new QLabel("Daftar Pesanan Masuk");
    lblList->setObjectName("sectionHeader");
    cashierListWidget = new QListWidget;
    cashierListWidget->setMinimumWidth(260);

    leftCol->addWidget(lblList);
    leftCol->addWidget(cashierListWidget);

    // --- Kanan: Detail pesanan yang dipilih ---
    QVBoxLayout *rightCol = new QVBoxLayout;
    rightCol->setSpacing(8);

    QLabel *lblDetail = new QLabel("Detail Pesanan");
    lblDetail->setObjectName("sectionHeader");

    // ScrollArea untuk detail
    QScrollArea *detailScroll = new QScrollArea;
    detailScroll->setWidgetResizable(true);
    detailScroll->setObjectName("detailScroll");

    QWidget *detailContainer = new QWidget;
    detailContainer->setObjectName("detailContainer");
    QVBoxLayout *detailLayout = new QVBoxLayout(detailContainer);
    detailLayout->setContentsMargins(16, 16, 16, 16);
    detailLayout->setSpacing(8);
    detailLayout->setAlignment(Qt::AlignTop);

    cashierDetailLabel = new QLabel(
        "← Pilih pesanan dari daftar\nuntuk melihat detail.");
    cashierDetailLabel->setObjectName("detailPlaceholder");
    cashierDetailLabel->setAlignment(Qt::AlignCenter);
    cashierDetailLabel->setWordWrap(true);

    detailLayout->addWidget(cashierDetailLabel);
    detailScroll->setWidget(detailContainer);

    rightCol->addWidget(lblDetail);
    rightCol->addWidget(detailScroll);

    panelLayout->addLayout(leftCol, 1);
    panelLayout->addLayout(rightCol, 2);

    // --- Tombol ---
    QPushButton *btnPay  = new QPushButton("✔  Proses Pembayaran → Kirim ke Dapur");
    QPushButton *btnHome = new QPushButton("← Beranda");

    btnPay->setObjectName("primaryBtn");
    btnHome->setObjectName("secondaryBtn");

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(15);
    btnLayout->addWidget(btnHome);
    btnLayout->addStretch();
    btnLayout->addWidget(btnPay);

    layout->addWidget(title);
    layout->addLayout(panelLayout, 1);
    layout->addLayout(btnLayout);

    // Saat item dipilih di daftar → tampilkan detail di kanan
    connect(cashierListWidget, &QListWidget::currentRowChanged, this,
            [=](int row) {
        // Bersihkan detail lama
        QLayoutItem *child;
        while ((child = detailLayout->takeAt(0)) != nullptr) {
            if (child->widget()) child->widget()->deleteLater();
            delete child;
        }

        if (row < 0 || row >= cashierOrders.size()) {
            QLabel *ph = new QLabel(
                "← Pilih pesanan dari daftar\nuntuk melihat detail.");
            ph->setObjectName("detailPlaceholder");
            ph->setAlignment(Qt::AlignCenter);
            ph->setWordWrap(true);
            detailLayout->addWidget(ph);
            return;
        }

        const Order &ord = cashierOrders[row];

        // Header info customer
        QFrame *customerCard = new QFrame;
        customerCard->setObjectName("detailCard");
        QVBoxLayout *ccLayout = new QVBoxLayout(customerCard);
        ccLayout->setContentsMargins(12, 12, 12, 12);
        ccLayout->setSpacing(4);

        QString tableStr = (ord.tableNumber == 0)
            ? "Takeaway"
            : QString("Meja %1").arg(ord.tableNumber);

        auto mkRow = [&](const QString &label, const QString &val,
                         const QString &objName = "detailText") {
            QHBoxLayout *row = new QHBoxLayout;
            QLabel *lbl = new QLabel(label);
            lbl->setObjectName("detailLabel");
            lbl->setFixedWidth(120);
            QLabel *vlb = new QLabel(val);
            vlb->setObjectName(objName);
            vlb->setWordWrap(true);
            row->addWidget(lbl);
            row->addWidget(vlb, 1);
            ccLayout->addLayout(row);
        };

        mkRow("Antrian",   QString("#%1").arg(ord.queueNumber),  "detailValueBold");
        mkRow("Order ID",  QString("#%1").arg(ord.orderId));
        mkRow("Pelanggan", ord.customerName,                     "detailValueBold");
        mkRow("Lokasi",    tableStr);
        mkRow("Status",    ord.status);

        detailLayout->addWidget(customerCard);

        // Daftar menu yang dipesan
        QLabel *lblMenu = new QLabel("Daftar Menu");
        lblMenu->setObjectName("sectionHeader");
        detailLayout->addWidget(lblMenu);

        int total = 0;
        for (const auto &food : ord.items) {
            QString formattedPrice = QString::number(food.price);
            int len = formattedPrice.length();
            for (int i = len - 3; i > 0; i -= 3)
                formattedPrice.insert(i, '.');

            QWidget *menuW = new QWidget;
            QHBoxLayout *menuRow = new QHBoxLayout(menuW);
            menuRow->setContentsMargins(0, 0, 0, 0);
            QLabel *nm = new QLabel(QString("• %1").arg(food.name));
            nm->setObjectName("detailText");
            QLabel *pr = new QLabel(QString("Rp %1").arg(formattedPrice));
            pr->setObjectName("detailPrice");
            menuRow->addWidget(nm, 1);
            menuRow->addWidget(pr);
            detailLayout->addWidget(menuW);
            total += food.price;
        }

        // Total
        detailLayout->addSpacing(8);
        QFrame *divider = new QFrame;
        divider->setFrameShape(QFrame::HLine);
        divider->setObjectName("divider");
        detailLayout->addWidget(divider);

        QString formattedTotal = QString::number(total);
        int len = formattedTotal.length();
        for (int i = len - 3; i > 0; i -= 3)
            formattedTotal.insert(i, '.');

        QWidget *totalW = new QWidget;
        QHBoxLayout *totalRow = new QHBoxLayout(totalW);
        totalRow->setContentsMargins(0, 0, 0, 0);
        QLabel *lblTot  = new QLabel("TOTAL");
        lblTot->setObjectName("totalLabelBold");
        QLabel *valTot  = new QLabel(QString("Rp %1").arg(formattedTotal));
        valTot->setObjectName("totalValueBold");
        totalRow->addWidget(lblTot, 1);
        totalRow->addWidget(valTot);
        detailLayout->addWidget(totalW);
    });

    // Proses Pembayaran
    connect(btnPay, &QPushButton::clicked, this, [=]() {
        int row = cashierListWidget->currentRow();
        if (row >= 0 && row < cashierOrders.size()) {
            Order ord = cashierOrders.takeAt(row);
            ord.status = "Dibayar & Masak";
            kitchenQueue.enqueue(ord);

            QString tableStr = (ord.tableNumber == 0)
                ? "Takeaway"
                : QString("Meja %1").arg(ord.tableNumber);

            QMessageBox::information(
                this, "✅  Pembayaran Berhasil",
                QString(
                    "Pesanan dari %1 (%2) telah dibayar.\n"
                    "Order #%3 dikirim ke dapur.")
                    .arg(ord.customerName)
                    .arg(tableStr)
                    .arg(ord.orderId));

            updateCashierUI();
            updateKitchenUI();
        } else {
            QMessageBox::warning(this, "Pilih",
                                 "Pilih pesanan yang ingin diproses pembayarannya.");
        }
    });

    connect(btnHome, &QPushButton::clicked, this,
            [=]() { stackedWidget->setCurrentIndex(0); });
}

void RestaurantApp::updateCashierUI() {
    cashierListWidget->clear();
    for (const auto &ord : cashierOrders) {
        QString tableStr = (ord.tableNumber == 0)
            ? "Takeaway"
            : QString("Meja %1").arg(ord.tableNumber);
        cashierListWidget->addItem(
            QString("🎫 #%1  |  %2  |  %3  |  %4 item")
                .arg(ord.queueNumber, 3, 10, QChar('0'))
                .arg(ord.customerName)
                .arg(tableStr)
                .arg(ord.items.size()));
    }
}
