#include "content.h"
#include "ui_content.h"

Content::Content(QString name,QString content,QString time,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Content)
{
    ui->setupUi(this);
    this->ui->nameLabel->setText(name);
    this->ui->contentLabel->setText(content);
    this->ui->timeLabel->setText(time);
}

Content::~Content()
{
    delete ui;
}
