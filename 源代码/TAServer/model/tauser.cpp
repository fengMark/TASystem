#include "tauser.h"

TAUser::TAUser(QObject *parent) :
    QObject(parent)
{
}

TAUser::TAUser(QString id, const QString &nick, const QString &pwd, const QString &sex,
               const QDate birth, const QString &name, const QString &addr,
               const QString &phone, const QString &ques, const QString &ans,
               const QDateTime regDate, const int status)
{
    m_userID = id;
    m_nickname = nick;
    m_password = pwd;
    m_sex = sex;
    m_birthday = birth;
    m_question = ques;
    m_answer = ans;
    m_name = name;
    m_phone = phone;
    m_address = addr;
    m_regDateTime = regDate;
    m_status = status;
}


/*************************************************
Function Name： getXX()
Description: 获取用户信息
Input： NULL
Output：**(QString / int / QDateTime)
Changes： NULL
*************************************************/

/********************* bgein getXX()****************************/
QString TAUser::getUserID() const { return m_userID; }

QString TAUser::getNickname() const { return m_nickname; }

QString TAUser::getPassword() const { return m_password; }

QString TAUser::getSex() const { return m_sex; }

QDate TAUser::getBirthday() const { return m_birthday;}

QString TAUser::getQuestion() const { return m_question; }

QString TAUser::getAnswer() const { return m_answer; }

QString TAUser::getName() const { return m_name; }

QString TAUser::getPhone() const { return m_phone; }

QString TAUser::getAddress() const { return m_address; }

QDateTime TAUser::getRegDateTime() const { return m_regDateTime; }

int TAUser::getStatus() const {return m_status; }

/********************* end getXX()******************************/

/********************* bgein setXX()****************************/
 void TAUser::setUserID(const QString &id) { m_userID = id; }
 void TAUser::setNickname(const QString &nick) { m_nickname = nick; }
 void TAUser::setPassword(const QString &pwd) { m_password = pwd; }
 void TAUser::setSex(const QString &sex) { m_sex = sex; }
 void TAUser::setBirthday(const QDate &birth) { m_birthday = birth; }
 void TAUser::setQusetion(const QString &ques) { m_question = ques; }
 void TAUser::setAnswer(const QString &ans) { m_answer = ans; }
 void TAUser::setName(const QString &name) { m_name = name; }
 void TAUser::setPhone(const QString &phone) { m_phone = phone; }
 void TAUser::setAddress(const QString & add) { m_address = add; }
 void TAUser::setRegDateTime(const QDateTime &date) { m_regDateTime = date; }
 void TAUser::setStatus(const int status) { m_status = status; }
/********************* end setXX()******************************/


 QDataStream &operator<<(QDataStream &out, const TAUser &user)
 {
     out << user.m_userID << user.m_nickname << user.m_password
         << user.m_sex << user.m_birthday << user.m_question
         << user.m_answer << user.m_name << user.m_phone
         << user.m_address << user.m_regDateTime << user.m_status;
     return out;
 }

 QDataStream &operator>>(QDataStream &in, TAUser &user)
 {
     in >> user.m_userID >> user.m_nickname >> user.m_password
        >> user.m_sex >> user.m_birthday >> user.m_question
        >> user.m_answer >> user.m_name >>  user.m_phone
        >> user.m_address >> user.m_regDateTime >> user.m_status;
     return in;
 }
