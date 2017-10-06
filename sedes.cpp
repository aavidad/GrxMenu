#include "sedes.h"
#include "ui_sedes.h"
#include <QSqlQuery>
#include <QHostAddress>
#include <QAbstractItemModel>
Sedes::Sedes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sedes)
{
    ui->setupUi(this);


   // mascaraIP();//Ponemos una mascara para validar la ip introducida
    cargaCombo();//Cargamos las sedes en los combobox
    //checkBox_tresEstados();//Ponemos los checkbox con tres estados
}

Sedes::~Sedes()
{
    delete ui;
}

void Sedes::checkBox_tresEstados(){

    ui->checkBox_basedatos_juridica->setTristate(true);
    ui->checkBox_correo->setTristate(true);
    ui->checkBox_epol->setTristate(true);
    ui->checkBox_epol_movil->setTristate(true);
    ui->checkBox_gestion_economica->setTristate(true);
    ui->checkBox_perfil_contratante ->setTristate(true);
    ui->checkBox_gestion_municipal->setTristate(true);
    ui->checkBox_portal_web->setTristate(true);
    ui->checkBox_sede_electronica->setTristate(true);
    ui->checkBox_siapol->setTristate(true);
    ui->checkBox_soporte->setTristate(true);
    ui->checkBox_suscripcion->setTristate(true);

}

void Sedes::clean_checkbox(){
    ui->checkBox_portal_web->setCheckState((Qt::CheckState)0);
    ui->checkBox_correo->setCheckState((Qt::CheckState)0);
    ui->checkBox_basedatos_juridica->setCheckState((Qt::CheckState)0);
    ui->checkBox_gestion_municipal->setCheckState((Qt::CheckState)0);
    ui->checkBox_suscripcion->setCheckState((Qt::CheckState)0);
    ui->checkBox_perfil_contratante->setCheckState((Qt::CheckState)0);
    ui->checkBox_gestion_economica->setCheckState((Qt::CheckState)0);
    ui->checkBox_soporte->setCheckState((Qt::CheckState)0);
    ui->checkBox_sede_electronica->setCheckState((Qt::CheckState)0);
    ui->checkBox_epol->setCheckState((Qt::CheckState)0);
    ui->checkBox_epol_movil->setCheckState((Qt::CheckState)0);
    ui->checkBox_siapol->setCheckState((Qt::CheckState)0);
}

void Sedes::mascaraIP(){
    //Máscara para validar la ip introducida

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5][-]25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->comboBox_IP->setValidator(ipValidator);

}

void Sedes::cargaCombo(){

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery* query = new QSqlQuery(db);
    QSqlQuery* query_tlf = new QSqlQuery(db);
    QString sql;
    QString sql_tlf;
    sql = "select NOMBRE,ipLinea,codigoPostal,extension,id from nodo";
    sql_tlf = "select * from telefononodo";
    query->prepare(sql);
    query_tlf->prepare(sql_tlf);

    if(!query->exec())
        qDebug () << "Error en la consulta: " << query->lastError();
    else{
        qDebug () << "Consulta realizada con exito: " << query->lastQuery();

        model->setQuery(*query);
        ui->comboBox_IP->setModel(model);
        ui->comboBox_IP->setModelColumn(1);
        ui->comboBox_CP->setModel(model);
        ui->comboBox_CP->setModelColumn(2);
        ui->comboBox_extension->setModel(model);
        ui->comboBox_extension->setModelColumn(3);
        ui->comboBox_NODO->setModel(model);
        on_comboBox_NODO_activated(ui->comboBox_NODO->itemText(0));
        ui->comboBox_NODO->setFocus();
    }

    if(!query_tlf->exec())
        qDebug () << "Error en la consulta: " << query_tlf->lastError();
    else{
        qDebug () << "Consulta realizada con exito: " << query_tlf->lastQuery();
        model_tlf->setQuery(*query_tlf);
        ui->comboBox_TLF->setModel(model_tlf);
        ui->comboBox_TLF->setModelColumn(1);
    }
    conectar_checkbox();

    delete query;
    delete query_tlf;

}

void Sedes::conectar_checkbox(){
    connect(ui->checkBox_basedatos_juridica, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_correo, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_epol, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_epol_movil, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_gestion_economica, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_perfil_contratante, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_gestion_municipal, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_portal_web, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_sede_electronica, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_siapol, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_soporte, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_basedatos_juridica, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));
    connect(ui->checkBox_suscripcion, SIGNAL(clicked(bool)), this, SLOT(cambio_estado_checkbox()));

    connect(ui->lineEdit_adsl, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_altitud, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_bandera, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_caudal, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_cif, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_comarca, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_contacto, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_cp, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_dir3, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_direccion, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_equipamiento, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_escudo, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_extension, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_fax, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_habitantes, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_ine, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_ip_cifrado, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_latitud, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_latitud_municipio, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_letra, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_longitud, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_longitud_municipio, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_mancomunidad, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_movil, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_municipio, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_numero, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_n_adm, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_piso, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_portar_transparencia, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_puerta, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_sede, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_servicio, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_superficie, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_tablon, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_via, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));
    connect(ui->lineEdit_web, SIGNAL(textChanged(const QString &)), this, SLOT(cambio_estado_line()));

}

void Sedes::consultaNodo(const QString &nombre){

    QSqlQuery consultar;
    QSqlQuery consultar_telefono;
    QSqlQuery consultar_email;
    QSqlQuery consultar_poblacion;
    QSqlQuery consultar_municipio;
    QSqlQuery consultar_comarca;
    QSqlQuery consultar_mancomunidad;
    QSqlQuery consultar_programa;
    QString idNodo;
    QString idAnio;
    QString idMunicipio;

    consultar.prepare(QString("select * from nodo where nombre = :nombre"));
    consultar.bindValue(":nombre", nombre);

    if (consultar.exec() and consultar.first()){
            clean_checkbox();
            ui->comboBox_IP->clear();
            ui->comboBox_CP->clear();
            ui->comboBox_extension->clear();
            idNodo = consultar.value(1).toString();
            idAnio = ui->comboBox_anio->currentText();

            ui->lineEdit_adsl->setText(consultar.value(19).toString());
            ui->lineEdit_via->setText(consultar.value(4).toString());
            ui->lineEdit_direccion->setText(consultar.value(5).toString());
            ui->lineEdit_numero->setText(consultar.value(6).toString());
            ui->lineEdit_letra->setText(consultar.value(7).toString());
            ui->lineEdit_puerta->setText(consultar.value(10).toString());
            ui->lineEdit_piso->setText(consultar.value(9).toString());
            ui->lineEdit_cp->setText(consultar.value(11).toString());
            ui->lineEdit_latitud->setText(consultar.value(12).toString());
            ui->lineEdit_longitud->setText(consultar.value(13).toString());
            ui->lineEdit_extension->setText(consultar.value(15).toString());
            ui->lineEdit_fax->setText(consultar.value(16).toString());
            ui->lineEdit_sede->setText(consultar.value(18).toString());
            ui->lineEdit_adsl->setText(consultar.value(19).toString());
            ui->lineEdit_n_adm->setText(consultar.value(20).toString());
            ui->lineEdit_ip_cifrado->setText(consultar.value(22).toString());
            ui->lineEdit_caudal->setText(consultar.value(24).toString());
            ui->lineEdit_equipamiento->setText(consultar.value(25).toString());
            ui->lineEdit_servicio->setText(consultar.value(23).toString());
            ui->lineEdit_contacto->setText(consultar.value(14).toString());
            ui->lineEdit_movil->setText(consultar.value(26).toString());

            consultar_programa.prepare(QString("select * from programa where id =:idNodo and anio =:idAnio"));
            consultar_programa.bindValue(":idNodo",idNodo);
            consultar_programa.bindValue(":idAnio",idAnio);
            if (consultar_programa.exec() and consultar_programa.first()){

                ui->checkBox_portal_web->setCheckState((Qt::CheckState)consultar_programa.value(2).toInt());
                ui->checkBox_correo->setCheckState((Qt::CheckState)consultar_programa.value(3).toInt());
                ui->checkBox_basedatos_juridica->setCheckState((Qt::CheckState)consultar_programa.value(4).toInt());
                ui->checkBox_suscripcion->setCheckState((Qt::CheckState)consultar_programa.value(5).toInt());
                ui->checkBox_perfil_contratante->setCheckState((Qt::CheckState)consultar_programa.value(6).toInt());
                ui->checkBox_gestion_municipal->setCheckState((Qt::CheckState)consultar_programa.value(7).toInt());
                ui->checkBox_gestion_economica->setCheckState((Qt::CheckState)consultar_programa.value(8).toInt());
                ui->checkBox_soporte->setCheckState((Qt::CheckState)consultar_programa.value(9).toInt());
                ui->checkBox_sede_electronica->setCheckState((Qt::CheckState)consultar_programa.value(10).toInt());
                ui->checkBox_epol->setCheckState((Qt::CheckState)consultar_programa.value(11).toInt());
                ui->checkBox_epol_movil->setCheckState((Qt::CheckState)consultar_programa.value(12).toInt());
                ui->checkBox_siapol->setCheckState((Qt::CheckState)consultar_programa.value(13).toInt());
            }

            ui->comboBox_telefonos->clear();
            consultar_telefono.prepare(QString("select * from telefononodo where idNodo =:idNodo"));
            consultar_telefono.bindValue(":idNodo",idNodo);
            if (consultar_telefono.exec() and consultar_telefono.first()){
                ui->comboBox_TLF->setCurrentIndex(ui->comboBox_TLF->findText(consultar_telefono.value(1).toString()));
                do{
                    ui->comboBox_telefonos->addItem(consultar_telefono.value(1).toString());
                }while(consultar_telefono.next());
            }

            consultar_poblacion.prepare(QString("select * from poblacion where id =:idPoblacion"));
            consultar_poblacion.bindValue(":idPoblacion", consultar.value(1).toString());
            if (consultar_poblacion.exec() and consultar_poblacion.first()){
                idMunicipio = consultar_poblacion.value(1).toString();
                ui->lineEdit_habitantes->setText(consultar_poblacion.value(3).toString());
                ui->lineEdit_superficie->setText(consultar_poblacion.value(4).toString());

                consultar_municipio.prepare(QString("select * from municipio where id = :idMunicipio"));
                consultar_municipio.bindValue(":idMunicipio",idMunicipio);
                if (consultar_municipio.exec() and consultar_municipio.first()){
                    ui->lineEdit_dir3->setText(consultar_municipio.value(6).toString());
                    ui->lineEdit_ine->setText(consultar_municipio.value(7).toString());
                    ui->lineEdit_cif->setText(consultar_municipio.value(5).toString());
                    ui->lineEdit_habitantes->setText(consultar_municipio.value(8).toString());
                    ui->lineEdit_superficie->setText(consultar_municipio.value(9).toString());
                    ui->lineEdit_altitud->setText(consultar_municipio.value(10).toString());
                    ui->lineEdit_latitud->setText(consultar_municipio.value(11).toString());
                    ui->lineEdit_bandera->setText(consultar_municipio.value(13).toString());
                    ui->lineEdit_escudo->setText(consultar_municipio.value(14).toString());
                    ui->lineEdit_web->setText(consultar_municipio.value(15).toString());
                    ui->lineEdit_tablon->setText(consultar_municipio.value(20).toString());
                    ui->lineEdit_portar_transparencia->setText(consultar_municipio.value(21).toString());
                    ui->lineEdit_latitud_municipio->setText(consultar_municipio.value(11).toString());
                    ui->lineEdit_longitud_municipio->setText(consultar_municipio.value(12).toString());
                    consultar_comarca.prepare(QString("select nombre from comarca where id = :idMunicipio"));
                    consultar_comarca.bindValue(":idMunicipio",idMunicipio);
                    if (consultar_comarca.exec() and consultar_comarca.first()){
                        ui->lineEdit_comarca->setText(consultar_comarca.value(0).toString());
                    }
                    consultar_mancomunidad.prepare(QString("select nombre from mancomunidad where id = :idMunicipio"));
                    consultar_mancomunidad.bindValue(":idMunicipio",idMunicipio);
                    if (consultar_mancomunidad.exec() and consultar_mancomunidad.first()){
                        ui->lineEdit_mancomunidad->setText(consultar_mancomunidad.value(0).toString());
                    }
                }
            }
            ui->comboBox_email->clear();
            consultar_email.prepare(QString("select * from emailnodo where idNodo = :idNodo"));
            consultar_email.bindValue(":idNodo",idNodo);
            if (consultar_email.exec() and consultar_email.first()){
                do{
                    ui->comboBox_email->addItem(consultar_email.value(1).toString());
                }while(consultar_email.next());

            }
    } else
          qDebug()<<"Error No se ha podido realizar la consulta "<< consultar.lastError();

}

void Sedes::on_comboBox_NODO_activated(const QString &nombre)
{
    QSqlQuery query=model->query();
    consultaNodo(nombre);
    ui->comboBox_IP->setCurrentIndex(ui->comboBox_IP->findText(query.value(1).toString()));
    ui->comboBox_extension->setCurrentIndex(ui->comboBox_extension->findText(query.value(3).toString()));
    ui->comboBox_CP->setCurrentIndex(ui->comboBox_CP->findText(query.value(2).toString()));
}

void Sedes::on_comboBox_extension_activated(const QString &ext)
{
    QSqlQuery query=model->query();
    consultaNodo(query.value(0).toString());
    ui->comboBox_extension->setCurrentIndex(ui->comboBox_extension->findText(query.value(3).toString()));
    ui->comboBox_IP->setCurrentIndex(ui->comboBox_IP->findText(query.value(1).toString()));
    ui->comboBox_NODO->setCurrentIndex(ui->comboBox_NODO->findText(query.value(0).toString()));
    ui->comboBox_CP->setCurrentIndex(ui->comboBox_CP->findText(query.value(2).toString()));
}

void Sedes::on_comboBox_IP_activated(const QString &ip)
{
    //Vamos a usar el modelo
    QHostAddress myIP;
    QSqlQuery query= model->query();
    if( myIP.setAddress(ip)){
        consultaNodo(query.value(0).toString()); //query.value(0).toString() contiene el nombre de la consulta actual
        ui->comboBox_NODO->setCurrentIndex(ui->comboBox_NODO->findText(query.value(0).toString()));
        ui->comboBox_extension->setCurrentIndex(ui->comboBox_extension->findText(query.value(3).toString()));
        ui->comboBox_CP->setCurrentIndex(ui->comboBox_CP->findText(query.value(2).toString()));
    }
    else
        qDebug() << "Invalid IP address"<<endl;
}

void Sedes::on_comboBox_CP_activated(const QString &arg1)
{
    QSqlQuery query=model->query();
    consultaNodo(query.value(0).toString());
    ui->comboBox_extension->setCurrentIndex(ui->comboBox_extension->findText(query.value(3).toString()));
    ui->comboBox_IP->setCurrentIndex(ui->comboBox_IP->findText(query.value(1).toString()));
    ui->comboBox_NODO->setCurrentIndex(ui->comboBox_NODO->findText(query.value(0).toString()));

}

void Sedes::on_comboBox_TLF_activated(const QString &arg1)
{
    QSqlQuery query_tlf=model_tlf->query();
    QSqlQuery query=model->query();
    QVariant datos;
    QString tmp;
    datos = model->data(model->index(query_tlf.value(0).toInt()-1,0));
    tmp = datos.toString();
    if (tmp != ui->comboBox_NODO->currentText())
    {
        consultaNodo(datos.toString());
        ui->comboBox_extension->setCurrentIndex(ui->comboBox_extension->findText(query.value(3).toString()));
        ui->comboBox_IP->setCurrentIndex(ui->comboBox_IP->findText(query.value(1).toString()));
        ui->comboBox_NODO->setCurrentIndex(ui->comboBox_NODO->findText(query.value(0).toString()));
        ui->comboBox_CP->setCurrentIndex(ui->comboBox_CP->findText(query.value(2).toString()));
    }

}

void Sedes::on_comboBox_anio_activated(const QString &arg1)
{
    on_comboBox_NODO_activated(ui->comboBox_NODO->currentText());
}

void Sedes::on_pB_web_clicked()
{
    QProcess process;
    process.startDetached("xdg-open", QStringList() << ui->lineEdit_web->text());

}

void Sedes::on_pB_sede_clicked()
{
    QProcess process;
    process.startDetached("xdg-open", QStringList() << ui->lineEdit_sede ->text());
}

void Sedes::on_sP_tablon_clicked()
{
    QProcess process;
    process.startDetached("xdg-open", QStringList() << ui->lineEdit_tablon->text());
}

void Sedes::on_pB_portalTransparencia_clicked()
{
    QProcess process;
    process.startDetached("xdg-open", QStringList() << ui->lineEdit_portar_transparencia->text());
}

void Sedes::on_pB_bandera_clicked()
{
    QProcess process;
    process.startDetached("xdg-open", QStringList() << ui->lineEdit_bandera->text());
}

void Sedes::on_pB_escudo_clicked()
{
    QProcess process;
    process.startDetached("xdg-open", QStringList() << ui->lineEdit_escudo->text());
}

void Sedes::on_pb_Edicion_toggled(bool checked)
{
    if (ui->pb_Edicion->isChecked()){
        ui->pb_Edicion->setStyleSheet("background-color:red");
        ui->lineEdit_adsl->setReadOnly(false);
        ui->lineEdit_altitud->setReadOnly(false);
        ui->lineEdit_bandera->setReadOnly(false);
        ui->lineEdit_caudal->setReadOnly(false);
        ui->lineEdit_cif->setReadOnly(false);
        ui->lineEdit_comarca->setReadOnly(false);
        ui->lineEdit_contacto->setReadOnly(false);
        ui->lineEdit_cp->setReadOnly(false);
        ui->lineEdit_dir3->setReadOnly(false);
        ui->lineEdit_direccion->setReadOnly(false);
        ui->lineEdit_equipamiento->setReadOnly(false);
        ui->lineEdit_escudo->setReadOnly(false);
        ui->lineEdit_extension->setReadOnly(false);
        ui->lineEdit_fax->setReadOnly(false);
        ui->lineEdit_habitantes->setReadOnly(false);
        ui->lineEdit_ine->setReadOnly(false);
        ui->lineEdit_ip_cifrado->setReadOnly(false);
        ui->lineEdit_latitud->setReadOnly(false);
        ui->lineEdit_latitud_municipio->setReadOnly(false);
        ui->lineEdit_letra->setReadOnly(false);
        ui->lineEdit_longitud->setReadOnly(false);
        ui->lineEdit_longitud_municipio->setReadOnly(false);
        ui->lineEdit_mancomunidad->setReadOnly(false);
        ui->lineEdit_movil->setReadOnly(false);
        ui->lineEdit_municipio->setReadOnly(false);
        ui->lineEdit_numero->setReadOnly(false);
        ui->lineEdit_n_adm->setReadOnly(false);
        ui->lineEdit_piso->setReadOnly(false);
        ui->lineEdit_portar_transparencia->setReadOnly(false);
        ui->lineEdit_puerta->setReadOnly(false);
        ui->lineEdit_sede->setReadOnly(false);
        ui->lineEdit_servicio->setReadOnly(false);
        //ui->lineEdit_superficie->setReadOnly(false);
        ui->lineEdit_tablon->setReadOnly(false);
        ui->lineEdit_via->setReadOnly(false);
        ui->lineEdit_web->setReadOnly(false);
        ui->checkBox_basedatos_juridica->setCheckable(false);
        ui->checkBox_correo->setCheckable(false);
        ui->checkBox_epol->setCheckable(false);
        ui->checkBox_epol_movil->setCheckable(false);
        ui->checkBox_gestion_economica->setCheckable(false);
        ui->checkBox_gestion_municipal->setCheckable(false);
        ui->checkBox_perfil_contratante->setCheckable(false);
        ui->checkBox_portal_web->setCheckable(false);
        ui->checkBox_sede_electronica->setCheckable(false);
        ui->checkBox_siapol->setCheckable(false);
        ui->checkBox_soporte->setCheckable(false);
        ui->checkBox_suscripcion->setCheckable(false);

    }
    else{
        ui->pb_Edicion->setStyleSheet("background-color:#00aa7f");
        ui->lineEdit_adsl->setReadOnly(true);
        ui->lineEdit_altitud->setReadOnly(true);
        ui->lineEdit_bandera->setReadOnly(true);
        ui->lineEdit_caudal->setReadOnly(true);
        ui->lineEdit_cif->setReadOnly(true);
        ui->lineEdit_comarca->setReadOnly(true);
        ui->lineEdit_contacto->setReadOnly(true);
        ui->lineEdit_cp->setReadOnly(true);
        ui->lineEdit_dir3->setReadOnly(true);
        ui->lineEdit_direccion->setReadOnly(true);
        ui->lineEdit_equipamiento->setReadOnly(true);
        ui->lineEdit_escudo->setReadOnly(true);
        ui->lineEdit_extension->setReadOnly(true);
        ui->lineEdit_fax->setReadOnly(true);
        ui->lineEdit_habitantes->setReadOnly(true);
        ui->lineEdit_ine->setReadOnly(true);
        ui->lineEdit_ip_cifrado->setReadOnly(true);
        ui->lineEdit_latitud->setReadOnly(true);
        ui->lineEdit_latitud_municipio->setReadOnly(true);
        ui->lineEdit_letra->setReadOnly(true);
        ui->lineEdit_longitud->setReadOnly(true);
        ui->lineEdit_longitud_municipio->setReadOnly(true);
        ui->lineEdit_mancomunidad->setReadOnly(true);
        ui->lineEdit_movil->setReadOnly(true);
        ui->lineEdit_municipio->setReadOnly(true);
        ui->lineEdit_numero->setReadOnly(true);
        ui->lineEdit_n_adm->setReadOnly(true);
        ui->lineEdit_piso->setReadOnly(true);
        ui->lineEdit_portar_transparencia->setReadOnly(true);
        ui->lineEdit_puerta->setReadOnly(true);
        ui->lineEdit_sede->setReadOnly(true);
        ui->lineEdit_servicio->setReadOnly(true);
        //ui->lineEdit_superficie->setReadOnly(true);
        ui->lineEdit_tablon->setReadOnly(true);
        ui->lineEdit_via->setReadOnly(true);
        ui->lineEdit_web->setReadOnly(true);

        ui->checkBox_basedatos_juridica->setAttribute(Qt::WA_TransparentForMouseEvents,true);
        ui->checkBox_basedatos_juridica->setFocusPolicy(Qt::NoFocus);


        ui->checkBox_correo->setCheckable(true);
        ui->checkBox_epol->setCheckable(true);
        ui->checkBox_epol_movil->setCheckable(true);
        ui->checkBox_gestion_economica->setCheckable(true);
        ui->checkBox_gestion_municipal->setCheckable(true);
        ui->checkBox_perfil_contratante->setCheckable(true);
        ui->checkBox_portal_web->setCheckable(true);
        ui->checkBox_sede_electronica->setCheckable(true);
        ui->checkBox_siapol->setCheckable(true);
        ui->checkBox_soporte->setCheckable(true);
        ui->checkBox_suscripcion->setCheckable(true);

    }
}

bool Sedes::comprueba_datos_cambiados()
{
    if         (ui->lineEdit_adsl->isModified()||
                ui->lineEdit_adsl->isModified()||
                ui->lineEdit_altitud->isModified()||
                ui->lineEdit_bandera->isModified()||
                ui->lineEdit_caudal->isModified()||
                ui->lineEdit_cif->isModified()||
                ui->lineEdit_comarca->isModified()||
                ui->lineEdit_contacto->isModified()||
                ui->lineEdit_cp->isModified()||
                ui->lineEdit_dir3->isModified()||
                ui->lineEdit_direccion->isModified()||
                ui->lineEdit_equipamiento->isModified()||
                ui->lineEdit_escudo->isModified()||
                ui->lineEdit_extension->isModified()||
                ui->lineEdit_fax->isModified()||
                ui->lineEdit_habitantes->isModified()||
                ui->lineEdit_ine->isModified()||
                ui->lineEdit_ip_cifrado->isModified()||
                ui->lineEdit_latitud->isModified()||
                ui->lineEdit_latitud_municipio->isModified()||
                ui->lineEdit_letra->isModified()||
                ui->lineEdit_longitud->isModified()||
                ui->lineEdit_longitud_municipio->isModified()||
                ui->lineEdit_mancomunidad->isModified()||
                ui->lineEdit_movil->isModified()||
                ui->lineEdit_municipio->isModified()||
                ui->lineEdit_numero->isModified()||
                ui->lineEdit_n_adm->isModified()||
                ui->lineEdit_piso->isModified()||
                ui->lineEdit_portar_transparencia->isModified()||
                ui->lineEdit_puerta->isModified()||
                ui->lineEdit_sede->isModified()||
                ui->lineEdit_servicio->isModified()||
                ui->lineEdit_tablon->isModified()||
                ui->lineEdit_via->isModified()||
                ui->lineEdit_web->isModified())

    {
        return true;
        qDebug()<< "ha cambiado";
    }
return false;
}

void Sedes::on_comboBox_NODO_currentIndexChanged(int index)
{
          if (comprueba_datos_cambiados()||cambiado)
              qDebug()<<"si ha cambiado";
}

void Sedes::on_lineEdit_adsl_textChanged(const QString &arg1)
{
    cambio_estado_line();
}

void Sedes::cambio_estado_line(){
    QLineEdit * linea =qobject_cast<QLineEdit*>(sender());
    if (linea->isModified()){
        linea->setStyleSheet("color:red");
        cambiado = true;
    }
}

void Sedes::cambio_estado_checkbox(){
     QCheckBox * CB =qobject_cast<QCheckBox*>(sender());

         CB->setStyleSheet("color:red");
         cambiado = true;
}
