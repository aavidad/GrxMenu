#ifndef EQUIPOS_H
#define EQUIPOS_H
#include "nmap_xml.h"
#include <QWidget>

/*
 * #include "KDSoapClient/KDSoapJob.h"
#include "KDSoapClient/KDSoap.h"
#include <QtCore/QObject>
#include <QtCore/QString>
#include "KDSoapClient/KDSoapClientInterface.h"
#include "KDSoapClient/KDSoapMessage.h"
#include "KDSoapClient/KDSoapValue.h"
#include "KDSoapClient/KDSoapPendingCallWatcher.h"
#include "KDSoapClient/KDSoapNamespaceManager.h"
#include "KDSoapClient/KDSoapGlobal.h"
*/
namespace Ui {
class Equipos;
}

class Equipos : public QWidget
{
    Q_OBJECT

public:
    explicit Equipos(Host *host,QWidget *parent = 0);
    QList <QString> host_ports_open(Host *host);
    ~Equipos();

private slots:

    int puerto_libre();

    int host_ports_open_int(Host *host);

    void on_pB_carpeta_clicked();

    void on_pB_web_clicked();

    void on_pB_webssl_clicked();

    void on_pB_telnet_clicked();

    void on_pB_ssh_clicked();

    void on_pB_systemconfig_clicked();

    void on_pB_networkManager_clicked();

    void on_pB_cups_clicked();

    void on_pB_vnc_clicked();

    void on_pushButton_12_clicked();

    void botonesActivos(QString puertos);

    void desactivaBotones();
    void on_pB_update_clicked();

    void on_pB_creaIncidencia_clicked();

    void on_pB_ISL_clicked();

private:
    Ui::Equipos *ui;
    QString ip;
    Host *host_tmp;
};

#endif // EQUIPOS_H