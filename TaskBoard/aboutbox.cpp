#include "aboutbox.h"
#include "ui_aboutbox.h"

#include "version.h"

AboutBox::AboutBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutBox)
{
    ui->setupUi(this);
    setWindowTitle(tr("About Task Board"));

    QString content = QString(
                "<html>"
                "<p>Version: %1</p>"
                "<h1>Project Home:</h1>"
                "<a href=\"https://github.com/atilm/RequirementsManager\">https://github.com/atilm/RequirementsManager</a>"
                "<h1>GNU Lesser General Public License</h1>"
                "<p>Copyright (C) 2016  Andreas Thielmann</p>"
                "<p>This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.</p>"
                "<p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.</p>"
                "<p>You should have received a copy of the GNU Lesser General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.</p>"
                "</html>").arg(Version::versionNumberString());

    ui->textEdit->setHtml(content);
    ui->textEdit->setReadOnly(true);
}

AboutBox::~AboutBox()
{
    delete ui;
}
