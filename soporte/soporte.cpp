#include "soporte.h"
#include "ui_soporte.h"
#include "qdebug.h"
#include <QThread>
#include <QTimer>
#include <QMovie>
#include <QHostAddress>
#include <QDesktopServices>
#include <QByteArray>
#include <QEventLoop>
#include <QNetworkReply>
#include <QTextCodec>
#include <QUrl>
#include <QUrlQuery>

#include "soporte/equipos.h"
#include "soporte/tabescaner.h"
#include "configuracion/configuracion.h"
#include "soporte/ejecutahilo.h"

Soporte::Soporte(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Soporte)
{
    ui->setupUi(this);

    ui->tabWidget->setMovable(false); //Evitamos que se puedan quitar los tab
    ui->tabWidget->setTabsClosable(true); //Ponemos un icono de cierre en los tab
    mascaraIP(); //Ponemos las máscaras a las ip introducidas
    cargaSedes(); //Cargamos las sedes en el combobox

}

void Soporte::cargaSedes(){
    //Cargamos las sedes en el combobox
       //QSqlQueryModel *model = new QSqlQueryModel();
       QString sql;
       sql = "select NOMBRE,ipLinea from nodo";
       QSqlQuery* query = new QSqlQuery(db);
       query->prepare(sql);
       if(!query->exec()){
           qDebug() <<"Error en la consulta: "<< query->lastError();
       }else{
           qDebug() <<"Consulta realizada con exito: "<<query->lastQuery();
           model->setQuery(*query);
           ui->cb_sede->setModel(model);
           on_cb_sede_activated(ui->cb_sede->itemText(0));
       }
}

void Soporte::mascaraIP(){

    QString ipRangeUltimo = "(?:[0-1]?[0-9]?[0-9]?[*]\\d{0}|2[0-4][0-9]|25[0-5])";
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRangeUltimo + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEdit_ip->setValidator(ipValidator);
}

Soporte::~Soporte()
{
    delete ui;
}

int Soporte::valida_ip(const QString &ip){
    QHostAddress myIP;
    QChar asterisco = QLatin1Char ('*');
    QChar punto = QLatin1Char('.');

    if (!ip.isEmpty()){
        if (( (ip.at(ip.count()-1)) == asterisco) && (ip.at(ip.count()-2))==punto)
            return true;
        if( myIP.setAddress(ip)&& (ip.count(punto)==3))
                return true;
    }

return false;
}

void Soporte::on_lineEdit_ip_textChanged(const QString &ip)
{
    if(valida_ip(ip))
        ui->lineEdit_ip->setStyleSheet("color:black");
    else
        ui->lineEdit_ip->setStyleSheet("color:red");
}

void Soporte::on_cb_sede_activated(const QString &nombre)
{
    QSqlQuery consultar;
    QSqlQuery consultar_telefono;
    QSqlQuery consultar_email;
    QSqlQuery consultar_poblacion;
    QSqlQuery consultar_municipio;
    QSqlQuery consultar_centro;
    QSqlQuery consultar_aplicaciones;

    QString idNodo;
    QString idMunicipio;
    QString idCentro;

    //QString resultado;
    consultar.prepare(QString("select * from nodo where nombre = :nombre"));
    consultar.bindValue(":nombre", nombre);
    if (consultar.exec() and consultar.first()){
            idNodo = consultar.value(NUM_COL_NODO_IDPOBLACION).toString();
            ui->lineEdit_ip->setText(consultar.value(NUM_COL_NODO_IPLINEA).toString());
            ui->lineEdit_adsl->setText(consultar.value(NUM_COL_NODO_ADSLLINEA).toString());
            ui->lineEdit_n_adm->setText(consultar.value(NUM_COL_NODO_NUMADMINISTRATIVO).toString());
            ui->lineEdit_direccion->setText(consultar.value(NUM_COL_NODO_NOMBREDIRECCION).toString());
            ui->lineEdit_numero->setText(consultar.value(NUM_COL_NODO_NUMERODIRECCION).toString());
            ui->lineEdit_servicio->setText(consultar.value(NUM_COL_NODO_SERVICIOLINEA).toString());
            ui->lineEdit_caudal->setText(consultar.value(NUM_COL_NODO_EQUIPAMIENTO).toString());
            ui->lineEdit_extension->setText(consultar.value(NUM_COL_NODO_EXTENSION).toString());

            consultar_telefono.prepare(QString("select * from telefononodo where idNodo =:idNodo"));
            consultar_telefono.bindValue(":idNodo",idNodo);

            if (consultar_telefono.exec() and consultar_telefono.first()){
                ui->lineEdit_telefono->setText(consultar_telefono.value(NUM_COL_TELEFONO_TELEFONO).toString());
            }
            consultar_poblacion.prepare(QString("select * from poblacion where id =:idPoblacion"));
            consultar_poblacion.bindValue(":idPoblacion", consultar.value(NUM_COL_NODO_IDPOBLACION).toString());

            if (consultar_poblacion.exec() and consultar_poblacion.first()){
                idMunicipio = consultar_poblacion.value(NUM_COL_POBLACION_IDMUNICIPIO).toString();

                consultar_municipio.prepare(QString("select * from municipio where id = :idMunicipio"));
                consultar_municipio.bindValue(":idMunicipio",idMunicipio);
                if (consultar_municipio.exec() and consultar_municipio.first()){

                }
            }
            consultar_aplicaciones.prepare(QString("select * from aplicacion where idNodo =:idNodo"));
            consultar_aplicaciones.bindValue(":idNodo", idNodo);

            if (consultar_aplicaciones.exec())
                    if (consultar_aplicaciones.first()){
                        atalaya = consultar_aplicaciones.value(NUM_COL_APLICACIONES_ATALAYA).toInt();
                        glpi = consultar_aplicaciones.value(NUM_COL_APLICACIONES_GLPI).toInt();
                        ocs = consultar_aplicaciones.value(NUM_COL_APLICACIONES_OCS).toInt();
                    }
            consultar_centro.prepare(QString("select * from centro where id =:idCentro"));
            consultar_centro.bindValue(":idCentro", consultar.value(2).toString());

            if (consultar_centro.exec() and consultar_centro.first()){
                idCentro = consultar_centro.value(NUM_COL_CENTRO_NOMBRE).toString();
                ui->lineEdit_centro->setText(consultar_centro.value(NUM_COL_CENTRO_NOMBRE).toString());
                ui->lineEdit_telefono_centro->setText(consultar_centro.value(NUM_COL_CENTRO_TELEFONO).toString());
            }
   } else
          qDebug()<<"Error No se ha podido realizar la consulta "<< consultar.lastError();
}

void Soporte::on_Btn_Buscar_clicked()
{
    if(valida_ip(ui->lineEdit_ip->text()))
        ejecuta_nmap();
}

void Soporte::ejecuta_nmap()
{
    Configuracion configuracion;
    QThread *hilo =new QThread();
    ejecutaHilo *hebra = new ejecutaHilo(ui->lineEdit_ip->text(),"-vvv -p"+configuracion.puertos_buscados());
    ui->TextoSalida->appendPlainText("Realizando escaneo para la ip:  "+ui->lineEdit_ip->text());
    hebra->moveToThread(hilo);
    qRegisterMetaType<QList<NMapScan>>("QList<NMapScan>");
    connect(hilo, &QThread::started, this, &Soporte::activa_barra_progreso );
    connect(hilo, &QThread::started, hebra, &ejecutaHilo::ejecuta);
    connect(hebra, reinterpret_cast<void (ejecutaHilo::*)(QList<NMapScan>)>(&ejecutaHilo::resultadoListo), this, &Soporte::resultados);
    connect(hebra, &ejecutaHilo::resultadoListo, this, &Soporte::desActiva_barra_progreso);
    connect(hilo, &QThread::finished, hilo, &QObject::deleteLater);
    hilo->start();
}

void Soporte::resultados(QList<NMapScan> res){

    NMapScan nmapscan;
    nmapscan = res[0];//Lo fijamos a cero porque sólo puede haber uno
    NMap *nmap =new NMap(nmapscan);
    ui->TextoSalida->appendPlainText("Equipos Buscados: "+QString::number(nmap->nmap_num_host_find()));
    ui->TextoSalida->appendPlainText("Equipos Encontrados: "+QString::number(nmap->nmap_num_host_up()));
    ui->TextoSalida->appendPlainText("Tiempo tardado: "+nmap->nmap_time_elapsed()+" Segundos");
    ui->tabWidget->insertTab(1,new tabEscaner(&nmapscan),nmap->nmap_arg_find());
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void Soporte::Ping()
{
    ui->TextoSalida->appendPlainText(ping->readAllStandardOutput());
}

void Soporte::on_Btn_Ping_clicked()
{
    if (valida_ip(ui->lineEdit_ip->text())){
        ping = new QProcess(this);
        ui->TextoSalida->appendPlainText("Haciendo ping a "+ui->lineEdit_ip->text());
        connect(ping,SIGNAL(readyReadStandardOutput()),this,SLOT(Ping()));
        ping->start("ping -c4 "+ui->lineEdit_ip->text());
    }
}

void Soporte::on_Btn_Mtr_clicked()
{
    if (valida_ip(ui->lineEdit_ip->text())){
        QProcess *mtr = new QProcess;
        ui->TextoSalida->appendPlainText("Haciendo mtr a "+ui->lineEdit_ip->text());
        mtr->startDetached("x-terminal-emulator -e mtr "+ ui->lineEdit_ip->text());
    }
}

void Soporte::on_pushButton_4_clicked()
{

}

void Soporte::activa_barra_progreso(){
    QMovie *movie = new QMovie(":imagenes/iconos/soporte/buscando.gif");
    QLabel *processLabel = new QLabel(this);
    ui->Estado->show();
    ui->Estado->setMovie(movie);
    processLabel->setMovie(movie);
    movie->start();
}

void Soporte::desActiva_barra_progreso(){
     ui->Estado->hide();
}

void Soporte::closeTab(int indice){
    if (indice>0)
        ui->tabWidget->removeTab(indice);
}

void Soporte::on_Btn_Limpiar_clicked()
{
    ui->TextoSalida->clear();
}

void Soporte::on_Btn_Incidencia_clicked(){
    Configuracion *configuracion = new Configuracion;
    QString para,asunto,cuerpo;
    para= configuracion->cual_es_para();
    asunto= configuracion->cual_es_asunto().arg(ui->cb_sede->currentText(),ui->lineEdit_direccion->text(),ui->lineEdit_adsl->text(),ui->lineEdit_n_adm->text()
                                            ,ui->lineEdit_ip->text(),ui->lineEdit_servicio->text(),ui->lineEdit_caudal->text(),ui->lineEdit_numero->text());
    cuerpo= configuracion->cual_es_cuerpo().arg(ui->cb_sede->currentText(),ui->lineEdit_direccion->text(),ui->lineEdit_adsl->text(),ui->lineEdit_n_adm->text()
                                            ,ui->lineEdit_ip->text(),ui->lineEdit_servicio->text(),ui->lineEdit_caudal->text(),ui->lineEdit_numero->text());

    QDesktopServices::openUrl(QUrl("mailto:"+para+"?subject="+asunto+"&body="+cuerpo, QUrl::TolerantMode));
}

void Soporte::on_Btn_Atalaya_clicked()
{
    QDesktopServices::openUrl(QUrl("http://atalaya.grx/Orion/NetPerfMon/NodeDetails.aspx?NetObject=N:"+QString::number(atalaya)));
}

void Soporte::resultado_html(QNetworkReply* p){
    qDebug()<< p;
}

void Soporte::insertaTexto(QString texto){

    ui->TextoSalida->moveCursor (QTextCursor::End);
    ui->TextoSalida->insertPlainText (texto);
    ui->TextoSalida->moveCursor (QTextCursor::End);

}

void Soporte::on_pB_gmenu_clicked()
{
    QDesktopServices::openUrl(QUrl("https://moad.dipgra.es/", QUrl::TolerantMode));
}

void Soporte::on_pB_portafirmas_clicked()
{
    QDesktopServices::openUrl(QUrl("https://moad.dipgra.es/moad/pfirma-moad/", QUrl::TolerantMode));
}

void Soporte::on_pB_gtablon_clicked()
{
    QDesktopServices::openUrl(QUrl("https://moad.dipgra.es/moad/Gtablon-moad/", QUrl::TolerantMode));
}

void Soporte::on_pB_epol_clicked()
{
    QDesktopServices::openUrl(QUrl("http://10.1.100.114/ePolV2/LoginNuevo!input.do", QUrl::TolerantMode));
}

void Soporte::on_pB_guadaltel_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.guadaltel.es/redmine/login", QUrl::TolerantMode));
}

void Soporte::on_pB_perilContratante_clicked()
{
    QDesktopServices::openUrl(QUrl("http://195.57.47.5:8080/pdc_srvGRA/", QUrl::TolerantMode));
}

void Soporte::on_pB_citrix_clicked()
{
    QDesktopServices::openUrl(QUrl("https://ctx.dipgra.es/Citrix/XenApp/auth/login.aspx", QUrl::TolerantMode));
}

void Soporte::on_pB_aytos_clicked()
{
    QDesktopServices::openUrl(QUrl("https://areacliente.aytos.es/areacliente/", QUrl::TolerantMode));
}

void Soporte::on_pB_isl_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.islonline.com/login/login.htm?wid=W409945021&rand=101138538&page=login&hl=es", QUrl::TolerantMode));
}

void Soporte::on_pB_listin_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.dipgra.es/listin", QUrl::TolerantMode));
}

void Soporte::on_Btn_Glpi_clicked()
{
    QDesktopServices::openUrl(QUrl("https://incidencias.dipgra.es/glpi/front/central.php?active_entity="+QString::number(glpi)));

}

