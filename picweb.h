#ifndef PICWEB_H
#define PICWEB_H

#include "mainwindow.h"
#include <QDebug>



//////////////////////////////////////////
///подключение к переферийному контроллеру
//////////////////////////////////////////

void MainWindow::getStatusXML()
{
    nam->get(QNetworkRequest(QUrl(statusURL)));
}

void MainWindow::StatusDownloaded(QNetworkReply* pReply)
{

    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();

    if(m_DownloadedData.size()!=0){parseXML();};
}

void MainWindow::parseXML()
{
    QXmlStreamReader xml(m_DownloadedData);

    QList< QMap<QString,QString> > lines;
       /* We'll parse the XML until we reach end of it.*/
       while(!xml.atEnd() &&
               !xml.hasError()) {
           /* Read next element.*/
           QXmlStreamReader::TokenType token = xml.readNext();
           /* If token is just StartDocument, we'll go to next.*/
           if(token == QXmlStreamReader::StartDocument) {
               continue;
           }
           /* If token is StartElement, we'll see if we can read it.*/
           if(token == QXmlStreamReader::StartElement) {
               /* If it's named lines, we'll go to the next.*/
               if(xml.name() == "response") {
                   continue;
               }
               /* If it's named line, we'll dig the information from there.*/
               if(xml.name() == "respons") {
                   lines.append(this->parseLine(xml));
               }
           }
       }
       /* Error handling. */
       if(xml.hasError()) {
           QMessageBox::critical(this,
                                 "QXSRExample::parseXML",
                                 xml.errorString(),
                                 QMessageBox::Ok);
       }
       /* Removes any device() or data from the reader
        * and resets its internal state to the initial state. */
       xml.clear();
       this->addLinesToList(lines);

}

QMap<QString, QString>  MainWindow::parseLine(QXmlStreamReader &xml)
{
    QMap<QString, QString> line;
       /* Let's check that we're really getting a line. */
       if(xml.tokenType() != QXmlStreamReader::StartElement &&
               xml.name() == "response") {
           return line;
       }
       /* Let's get the attributes for line */
       QXmlStreamAttributes attributes = xml.attributes();
       /* Let's check that line has id attribute. */
       if(attributes.hasAttribute("id")) {
           /* We'll add it to the map. */
           line["id"] = attributes.value("id").toString();
       }
       /* Next element... */
       xml.readNext();
       /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named line.
        */
       while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
               xml.name() == "response")) {
           if(xml.tokenType() == QXmlStreamReader::StartElement) {
               /* извлечения показаний статуса переферийного контроллера*/
               if(xml.name()!="") {
                   this->addElementDataToMap(xml, line);
               }
            }
           /* ...and next... */
           xml.readNext();
       }
       return line;

}

void MainWindow::addElementDataToMap(QXmlStreamReader& xml,
                                      QMap<QString, QString>& map) const {
    /* We need a start element, like <foo> */
    if(xml.tokenType() != QXmlStreamReader::StartElement) {
        return;
    }
    /* Let's read the name... */
    QString elementName = xml.name().toString();
    /* ...go to the next. */
    xml.readNext();
    /*
     * This elements needs to contain Characters so we know it's
     * actually data, if it's not we'll leave.
     */
    if(xml.tokenType() != QXmlStreamReader::Characters) {
        return;
    }
    /* Now we can add it to the map.*/
    map.insert(elementName, xml.text().toString());
}
///////////////////////////////////
///Отправляем результат парсинга в словарь
///////////////////////////////////

void MainWindow::addLinesToList(QList< QMap<QString,QString> >& lines) {

        //преабразуем словарь в QStringList
        QMap<QString,QString> line = lines.takeFirst();
        QMap<QString,QString>::iterator it = line.begin();
        //переменные элементов списка
         QString anstring="";
         QString ledstring="";
         QString tempstring="";
         QString potstring="";

         for(;it != line.end(); ++it)
        {
          if (it.key()[0]=='a' &&it.key()[2]!='6'){
               anstring=anstring+it.value()+",";
           } else if (it.key()[2]=='6') {anstring=anstring+it.value();}

               if (it.key()[0]=='l' && it.key()[3]!='7'){
               ledstring=ledstring+it.value()+",";
               } else if ( it.key()[3]=='7') {ledstring=ledstring+it.value();}

           if (it.key()[0]=='t'){
               tempstring=it.value();
           }

           if (it.key()[0]=='p'){
               potstring=it.value();
           }

        }

        //работа с общим списком
        picVaList.clear();
        picVaList.append(anstring);

        ui->SensorsLineAN->appendPlainText( anstring+" DT: "+ getDataTime());

        picVaList.append(ledstring);
        ui->SensorsLineDG->appendPlainText( ledstring+" DT: "+ getDataTime());
        picVaList.append(potstring);
        picVaList.append(tempstring);
        ui->SensorsLineTMP->appendPlainText("POT:"+potstring+" TEMP:"+tempstring+" "+ getDataTime());
        qDebug()<<picVaList;


}

QString MainWindow::getDataTime()
{
    QDateTime Datatime= QDateTime::currentDateTime();
    QString res=  Datatime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<res;
    return res;
}

#endif // PICWEB_H
