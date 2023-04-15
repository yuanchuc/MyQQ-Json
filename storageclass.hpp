#ifndef STORAGECLASS_H
#define STORAGECLASS_H
#include<QString>
#include<QCryptographicHash>

class Info{
    QString UserId;
    QString pwd;
    QString UserName;
    QString phone;
    QString curSock;
    QString status;
    QString RecordId;
public:
    Info(){

    }
    Info(QString UserId,QString UserName):UserId(UserId),UserName(UserName){

    }
    QString getUserId(){
        return UserId;
    }
    QString getPwd(){
        return pwd;
    }
    QString getUserName(){
        return UserName;
    }
    QString getPhone(){
        return phone;
    }
    QString getCurSock(){
        return curSock;
    }
    QString getStatus(){
        return status;
    }
    QString getRecordId(){
        return RecordId;
    }
    void setUserId(QString UserId){
        this->UserId = UserId;
    }
    void setPwd(QString pwd){
        this->pwd = pwd;
    }
    void setUserName(QString UserName){
        this->UserName = UserName;
    }
    void setPhone(QString phone){
        this->phone = phone;
    }
    void setCurSock(QString s){
        this->curSock = s;
    }
    void setStatus(QString s){
        this->status = s;
    }
    void setRecordId(QString s){
        this->RecordId = s;
    }
    QString GetMd5Pwd()
    {
        QString md5;
        QByteArray bb;//相当于是QChar的一个vector<>
        bb = QCryptographicHash::hash(getPwd().toUtf8(), QCryptographicHash::Md5);
        md5.append(bb.toHex());
        return md5;
    }
    char *getChinaUserName(){
        return UserName.toUtf8().data();
    }
    char *getCharUserId(){
        return this->UserId.toLocal8Bit().data();
    }
};


#endif // STORAGECLASS_H
