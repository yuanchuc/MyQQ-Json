#ifndef CONTENT_H
#define CONTENT_H

#include <QWidget>

namespace Ui {
class Content;
}

class Content : public QWidget
{
    Q_OBJECT

public:
    explicit Content(QString name,QString content,QString time,QWidget *parent = nullptr);
    ~Content();

private:
    Ui::Content *ui;
};

#endif // CONTENT_H
