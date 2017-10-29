#include "botonera.h"
#include "ui_botonera.h"
#include "usuario.h"
#include "soporte.h"
#include "sedes.h"
#include "configuracion.h"
#include "equipos.h"
#include "tunel.h"
#include <QNetworkInterface>
#include "qdebug.h"
#include "nmap_xml.h"

struct variables{
    QString keyfile1;
    QString keyfile2;
    QString username_ssh;
    QString password_ssh;
    QString username_DB;
    QString password_DB;
    QString server_ip;
    QString local_listenip;
    unsigned int remote_port;
    unsigned int local_listenport;
    QString remote_desthost;
    QString databasename;
    QString hostname_DB;
    unsigned int remote_destport;
    bool usar_ssh;
}datos;

Botonera::Botonera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Botonera)
{
    QSqlDatabase db = QSqlDatabase::database();
    ui->setupUi(this);

    cargaVariables();

}

Botonera::~Botonera()
{
    delete ui;
}

void Botonera::on_actionUsuarios_triggered()
{
    Usuario *usuario = new Usuario();
    usuario->show();
}

void Botonera::on_actionSalir_triggered()
{
    QMainWindow::close();
}

void Botonera::on_actionCronos_triggered()
{
    Configuracion *configuracion = new Configuracion;
    QProcess process;
    process.startDetached("xdg-open", QStringList() << configuracion->cual_es_cronos());
    delete configuracion;
}

void Botonera::on_actionWebmail_triggered()
{
    Configuracion *configuracion = new Configuracion;
    QProcess process;
    process.startDetached("xdg-open", QStringList() << configuracion->cual_es_correo());
    delete configuracion;
}

void Botonera::on_actionBeiro_triggered()
{
    Configuracion *configuracion = new Configuracion;
    QProcess process;
    process.startDetached("xdg-open", QStringList() << configuracion->cual_es_beiro());
    delete configuracion;
}

void Botonera::on_actionGlpi_triggered()
{
    Configuracion *configuracion = new Configuracion;
    QProcess process;
    process.startDetached("xdg-open" , QStringList() << configuracion->cual_es_glpi());
    delete configuracion;
}

void Botonera::on_actionActiveDirectory_triggered()
{
    Configuracion *configuracion = new Configuracion;
    QProcess process;
    if (configuracion->es_rdesktop())
       process.startDetached("rdesktop", QStringList() << "-u"+configuracion->cual_es_usuarioAD() <<"-p"+configuracion->cual_es_claveAD() << "-g"+configuracion->cual_es_resolucion() << configuracion->cual_es_servidorAD());
    else
       process.startDetached("xfreerdp", QStringList() << "/v:"+configuracion->cual_es_servidorAD() << "/u:"+configuracion->cual_es_usuarioAD() << "/p:"+configuracion->cual_es_claveAD() << "/size:"+configuracion->cual_es_resolucion());
    delete configuracion;
}

void Botonera::on_actionISL_triggered()
{
    Configuracion *configuracion = new Configuracion;
    QProcess process;
    process.startDetached("/usr/bin/proxychains", QStringList() << configuracion->cual_es_isl());
    delete configuracion;
}

void Botonera::on_actionConfigurar_triggered()
{
    Configuracion *configuracion= new Configuracion;
    configuracion->show();
}

void Botonera::on_actionNuevo_Men_triggered()
{
    Botonera *menu = new Botonera;
    menu->show();
}

void Botonera::on_actionSedes_triggered()
{
    Sedes *sedes = new Sedes;
    sedes->show();
}

void Botonera::on_actionSoporte_triggered()
{
    Soporte *soporte = new Soporte;
    soporte->show();
}

unsigned int puerto_libre(){
    QTcpServer server;
    if(server.listen(QHostAddress::Any, 0))
           return server.serverPort();
 return -1;
}

bool Botonera::basedatos(){
    db.setDatabaseName(datos.databasename);
    db.setHostName("127.0.0.1");
    db.setUserName(datos.username_DB);
    db.setPassword(datos.password_DB);
    db.setPort(datos.local_listenport);

    if (!db.open()){
        ui->label_DB->setText("Cerrado");
        ui->actionSedes->setDisabled(true);
        ui->actionSoporte->setDisabled(true);
        return false;
    }else
        ui->label_DB->setText("Abierto");
        ui->actionSedes->setEnabled(true);
        ui->actionSoporte->setEnabled(true);

    return true;
}

char* Botonera::convierte(QString dato){
    char* cstr;
    std::string fname = dato.toStdString();
    cstr = new char [fname.size()+1];
    strcpy( cstr, fname.c_str() );
    return cstr;
}

bool Botonera::creaConexion()
{
    Tunel *tunel = new Tunel;
    QThread *hilo= new QThread;
    tunel->keyfile1=convierte(datos.keyfile1);
    tunel->keyfile2=convierte(datos.keyfile2);
    tunel->username_ssh=convierte(datos.username_ssh);
    tunel->remote_destport=datos.remote_destport;
    tunel->local_listenport=datos.local_listenport;
    tunel->local_listenip="127.0.0.1";
    tunel->remote_port=datos.remote_port;
    tunel->remote_desthost=convierte(datos.remote_desthost);
    tunel->password_ssh=convierte(datos.password_ssh);
    tunel->server_ip=convierte(datos.server_ip);
    tunel->remote_destport=3306;//mysql remoto
    tunel->moveToThread(hilo);
    QObject::connect(hilo,&QThread::started, tunel, &Tunel::crea_conexion);
    QObject::connect(tunel,&Tunel::destroyed, hilo, &QThread::quit);
    QObject::connect(tunel,&Tunel::sshConectado, this, &Botonera::basedatos);
    QObject::connect(tunel,&Tunel::sshDesconectado,tunel,&Tunel::cierra_conexion);
    hilo->start();
    return true;
};

bool Botonera::cargaVariables(){

    db = QSqlDatabase::addDatabase("QMYSQL");
    Configuracion *configuracion = new Configuracion;
    datos.keyfile1=configuracion->cual_es_keyfile_publica();
    datos.keyfile2=configuracion->cual_es_keyfile_privada();
    datos.username_ssh=configuracion->cual_es_usuarioSSH();
    datos.password_ssh=configuracion->cual_es_password_ssh();
    datos.username_DB=configuracion->cual_es_usernameDB();
    datos.password_DB=configuracion->cual_es_passwordDB();
    datos.hostname_DB=configuracion->cual_es_hostnameDB();
    datos.local_listenip="127.0.0.1";

     if (configuracion->es_usarSSH()){
         datos.remote_port=configuracion->cual_es_puerto_remoto_ssh();
         datos.server_ip=configuracion->cual_es_servidorSSH().toLatin1().data();
     }
     else{
         datos.remote_port=configuracion->cual_es_PuertoDB().toInt();
         datos.server_ip="127.0.0.1";
     }
    datos.remote_desthost="127.0.0.1";
    datos.databasename=configuracion->cual_es_DataBaseName();

    NMap* nmap = new NMap();
    nmap->nmap_run_scan(QString::number(datos.remote_port),datos.server_ip);
    if (nmap->nmap_is_open_port(datos.server_ip, QString::number(datos.remote_port))){
        if (configuracion->es_usarSSH()){ //Tenemos seleccionado usar tunel ssh
                datos.local_listenport=puerto_libre();
                datos.usar_ssh=true;
                db.setPort(datos.local_listenport);
                creaConexion();
        }
        else{
                datos.local_listenport=configuracion->cual_es_PuertoDB().toInt();
                datos.usar_ssh=false;
                db.setDatabaseName(datos.databasename);
                db.setHostName("127.0.0.1");
                db.setUserName(datos.username_DB);
                db.setPassword(datos.password_DB);
                db.setPort(datos.local_listenport);
                if (!db.open()){
                    ui->label_DB->setText("Cerrado");
                    ui->actionSedes->setDisabled(true);
                    ui->actionSoporte->setDisabled(true);
                    return false;
                }
                else  {
                    ui->label_DB->setText("Abierto");
                    ui->actionSedes->setEnabled(true);
                    ui->actionSoporte->setEnabled(true);
                    }
                }

    }
     else{
        ui->statusBar->messageChanged("Puerto Cerrado");
        ui->actionSedes->setDisabled(true);
        ui->actionSoporte->setDisabled(true);
    }

    //Muestra la ip
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            ui->label_ip->setText(address.toString());
    }
    //Muestra el usuario del sistema con el que nos hemos logado
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    ui->label_username->setText(name);

delete configuracion;
delete nmap;
return true;
}

void Botonera::on_pushButton_clicked()
{
    cargaVariables();
}
