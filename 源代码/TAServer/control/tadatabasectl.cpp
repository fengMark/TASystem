#include "tadatabasectl.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <qdebug.h>
#include <QSqlError>
#include <QTextDocument>
#include "model/TAEncryption.h"

static const QString UPDATE_STATUS_IN_USERINFORMATION_SQL =
        "update UserInformation set status = 5";
static const QString UPDATE_PASSWORD_IN_USERINFORMATION_SQL =
        "update UserInformation set password = ? "
        "where userID = ? and password = ?";
static const QString SEARCH_QUESTION_ANSWER_IN_USERINFORMATION_SQL =
        "select password, question, answer from UserInformation where userID = ?;";
static const QString SEARCH_USER_IN_USERINFORMATION_SQL =
        "select * from UserInformation where (userID = ? and password = ?)";
static const QString SEARCH_USER_IN_USERINFORMATION_ID_SQL =
        "select * from UserInformation where userID = ?";
static const QString UPDATE_USERINFORMATION_SQL =
        "update UserInformation set "
        "head = ?, nickname = ?, sex = ?, "
        "phone = ?  where userID = ?;";
static const QString  UPDATE_STATUS_IN_USERINFORMATION_BY_ID_SQL=
        "update UserInformation set status = ? where userID = ?;";

static const QString  UPDATE_REMARKNAME_IN_USERFRIENDS_SQL=
        "update UserFriends set remarkName = ? where userID = ? and friendID = ?;";

static const QString ADD_USER_IN_USERINFORMATION_SQL =
        "insert into UserInformation values(?,?,?,?,?,?,?,?,?,?)";

static const QString COUNT_REGISTERED_USERS_IN_USERINFORMATION_SQL =
        "select count(*) from UserInformation where regDate >= ? and regDate < ?";

static const QString COUNT_FLOCKS_IN_FLOCKINFORMATION_SQL =
        "select count(*) from FlockInformation where createDate >= ? and createDate < ?";

static const QString SEARCH_FRIENDS_IN_USERFRIENDS_USERINFORMATION_SQL =
        "select UserFriends.groupName, UserFriends.friendID, "
        "UserInformation.nickName, UserFriends.remarkName,"
        "UserInformation.status, UserInformation.head "
        "from UserFriends,UserInformation "
        "where UserFriends.userID = ? "
        "and UserInformation.userID = UserFriends.friendID;";

static const QString SEARCH_ID_IN_USERFRIENDS_SQL =
        "select friendID from UserFriends where userID = ?";
static const QString SEARCH_GROUPNAME_IN_USERFRIENDS_SQL =
        "select groupName from UserFriends where userID = ? and friendID = ?";
static const QString DELETE_FRIEND_IN_USERFRIENDS_SQL =
        "delete from UserFriends where "
        "(userID = ? and friendID = ?) or "
        "(userID = ? and friendID = ?)";
static const QString ADD_FRIEND_IN_USERFRIENDS_SQL =
        "insert into UserFriends(userID, friendID, groupName) values (?, ?, ?);";

static const QString SEARCH_FLOCKS_IN_FLOCKINFORMAIONT_USERFLOCKS_SQL =
        "select * from FlockInformation "
        "where FlockInformation.flockID IN ("
        "select UserFlocks.flockID from UserFlocks "
        "where UserFlocks.userID = ?);";
static const QString SEARCH_FLOCK_IN_FLOCKINFORMAIONT_SQL =
        "select * from FlockInformation where flockID = ?";

static const QString SEARCH_FLOCKMEMBERS_IN_USERINFORMATION_USERFLOCKS_SQL =
        "select UserFlocks.flockID, UserFlocks.userID, UserInformation.head, "
        "UserInformation.nickname, UserInformation.status, UserFlocks.remark, "
        "UserFlocks.status, UserFlocks.joinDate "
        "from UserInformation, UserFlocks "
        "where UserFlocks.flockID = ? and "
        "UserInformation.userID = UserFlocks.userID;";

static const QString SEARCH_FLOCKMEMBER_IN_USERINFORMATION_USERFLOCKS_SQL =
        "select UserFlocks.flockID, UserFlocks.userID, UserInformation.head, "
        "UserInformation.nickname, UserInformation.status, UserFlocks.remark, "
        "UserFlocks.status, UserFlocks.joinDate "
        "from UserInformation, UserFlocks "
        "where UserFlocks.flockID = ? and UserFlocks.userID= ? and "
        "UserInformation.userID = UserFlocks.userID;";



////////////////////////////////////////////////////////////////////////////////////

static const QString SEARCH_FLOCK_CREATORID_IN_FLOCKINFORMATION_SQL =
        "select creatorID from FlockInformation where flockID = ?";

static const QString ADD_MEMBER_IN_USERFLOCKS_SQL =
        "insert into UserFlocks(flockID, userID, status, joinDate) values (?, ?, ?, ?);";

static const QString DELETE_FLOCK_IN_FLOCKINFORMATION_SQL =
        "delete from FlockInformation where flockID = ? and creatorID = ?";

static const QString DELETE_FLOCKMEMEBER_IN_USERFLOCKS_SQL =
        "delete from UserFlocks where flockID= ? and userID = ?";

static const QString UPDATE_FLOCKINFORMATION_SQL =
        "update FlockInformation set flockName = ?, theme = ? where flockID = ?;";

static const QString UPDATE_FLOCKMEMBER_REMAKR_IN_USERFLOCKS_SQL =
        "update UserFlocks set remark = ? where flockID = ? and userID = ?";

static const QString ADD_FLOCK_IN_FLOCKINFORMATOIN_SQL =
        "insert into FlockInformation values( ?, ?, ?, ?, ?);";
////////////////////////////////////////////////////////////////////////////////////



static const QString COUNT_DATARECORD_IN_DATECOUNTER_SQL =
        "select count(*) from DateCounter where date = ?;";
static const QString INSERT_VALUES_IN_DATECOUNTER_SQL =
        "insert into DateCounter values(?, 0, 0);";

static const QString SELECT_USERTIMES_IN_DATECOUNTER_SQL =
        "select userTimes from DateCounter where date = ?;";
static const QString SELECT_FLOCKTIMES_IN_DATECOUNTER_SQL =
        "select flockTimes from DateCounter where date = ?;";

static const QString INCREASE_USERTIMES_IN_DATECOUNTER_SQL =
        "update DateCounter set userTimes = userTimes + 1 where date = ?;";
static const QString INCREASE_FLOCKTIMES_IN_DATECOUNTER_SQL =
        "update DateCounter set flockTimes = flockTimes + 1 where date = ?;";

static const QString COUNT_FLOCKMEMBER_IN_USERFLOCKS_SQL =
        "select count(*) from UserFlocks where flockID = ? and userID = ?;";


/************************group***************************/
static const QString RENAME_GROUPNAME_IN_USERFREINS =
        "update UserFriends set groupName = ? where userID = ? and groupName = ?;";
static const QString MOVE_FRIEND_IN_USERFREINS =
        "update UserFriends set groupName = ? where userID = ? "
        "and friendID = ?;";

/************************message************************/
static const QString INSERT_MESSAGE_IN_HISTORYMESSAGE =
        "insert into HistoryMessage (senderID, receiverID, content, dateTime) "
        "values(?, ?, ?, ?); ";
static const QString INSERT_MESSAGE_IN_OFFLINEMESSAGE =
        "insert into OfflineMessage (kind, senderID, receiverID, content) "
        "values(?, ?, ?, ?);";
static const QString SEARCH_FRIEND_DATE_MESSAGE_IN_OFFLINEMESSAGE =
        "select * from OfflineMessage where receiverID = ?;";
static const QString DELETE_FRIEND_DATE_MESSAGE_IN_OFFLINEMESSAGE =
        "delete from OfflineMessage where receiverID = ?;";
static const QString SEARCH_FRIEND_DATE_MESSAGE_IN_HISTORYMESSAGE =
        "select senderID, receiverID, content, dateTime from HistoryMessage "
        "where ( (senderID = ? and receiverID = ?) "
        "or (senderID = ? and receiverID = ?) ) "
        "and dateTime >= ? and dateTime < ?;";
QString simplifyRichText( QString richText )
{
    QTextDocument doc;
    doc.setHtml(richText);
    qDebug() << "say:"<< doc.toPlainText();
    return doc.toPlainText();
}

TADatabaseCtl::TADatabaseCtl(QObject *parent) :
    QObject(parent)
{
    m_db = new QSqlDatabase;
}

TADatabaseCtl::~TADatabaseCtl()
{
    if (m_db != NULL)
    {
        if (m_db->isOpen())
            m_db->close();
        delete m_db;
        m_db = NULL;
    }
}
bool TADatabaseCtl::createConnection()
{
    if (m_db == NULL)
        m_db = new QSqlDatabase;
    if (m_db->isOpen())
    {
        return true;
    }
    if (QSqlDatabase::contains("TAServerDB"))
    {
        *m_db = QSqlDatabase::database("TAServerDB");
    }
    else
    {
        *m_db = QSqlDatabase::addDatabase("QMYSQL", "TAServerDB");
        m_db->setDatabaseName("TAServerDB");
    }

    m_db->setUserName("root");
    m_db->setPassword("1");
    m_db->setHostName(QLatin1String("localhost"));
    if (!m_db->open())
    {
        qDebug()<<"TAdata not open";
        return false;
    }
    qDebug()<<"TAdata  open";
    return true;
}
bool TADatabaseCtl::initDatabase()
{
    if (!createConnection())
    {
        return false;
    }

    QSqlQuery query(*m_db);
    query.exec(UPDATE_STATUS_IN_USERINFORMATION_SQL);//初始化用户登录状态
    errorSQLOrder(query, "initiDatabase");
    if (!query.isActive())
    {
        m_db->close();
        return false;
    }
    m_db->close();
    return true;
}
void TADatabaseCtl::close()
{
    if (m_db != NULL && m_db->isOpen())
        m_db->close();
}
bool TADatabaseCtl::isOpen()
{
    return (m_db->isOpen());
}
int TADatabaseCtl::updatePassword(TempStrings & tmp)
{
    if (!createConnection())
    {
        return CHANGE_PWD_FAIL;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(UPDATE_PASSWORD_IN_USERINFORMATION_SQL);

    query.addBindValue(tmp.m_three);
    query.addBindValue(tmp.m_one);
    query.addBindValue(tmp.m_two);

    query.exec();
    errorSQLOrder(query, "update-pwd");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return CHANGE_PWD_FAIL;
    }
    qDebug() << "size:" << query.numRowsAffected();
    if (query.numRowsAffected() <= 0)
    {
        m_db->close();
        return CHANGE_PWD_FAIL;
    }

    qDebug() << "size:" << query.numRowsAffected();
    return CHANGE_PWD_SUCCESS;
}
int TADatabaseCtl::searchQuestionAndAnswer(const QString &id, TempStrings &tmp)
{
    if (!createConnection())
    {
        return GET_QUESTION_ANSWER_FAIL;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_QUESTION_ANSWER_IN_USERINFORMATION_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "forget-pwd");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return GET_QUESTION_ANSWER_FAIL;
    }
    if (query.size() <= 0)
    {
        m_db->close();
        return GET_QUESTION_ANSWER_FAIL;
    }
    if (query.next())
    {
        tmp.m_one = query.value(0).toString();
        tmp.m_two = query.value(1).toString();
        tmp.m_three = query.value(2).toString();


        return GET_QUESTION_ANSWER_SUCCESS;
    }
    return GET_QUESTION_ANSWER_FAIL;
}

/*************************************************
Function Name： searchUserInUserInformation()
Description: 查询用户
*************************************************/
int TADatabaseCtl::searchUserInUserInformation(const LoginInformation &logInf,
                                                  UserInformation &userInf)
{
    if (!createConnection())
    {
        return LOGIN_FAIL;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_USER_IN_USERINFORMATION_SQL);
    query.addBindValue(logInf.m_userID);
    query.addBindValue(logInf.m_password);
    query.exec();
    errorSQLOrder(query, "login-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return LOGIN_FAIL;
    }
    if (query.size() <= 0)
    {
        m_db->close();
        return LOGIN_FAIL;
    }
    if (query.next())
    {
        userInf.m_userID = query.value(0).toString();
        userInf.m_headPortrait = query.value(1).toInt();
        userInf.m_nickname = query.value(2).toString();
        userInf.m_sex = query.value(4).toString();
        userInf.m_phone = query.value(9).toString();
        userInf.m_regDateTime = query.value(11).toDateTime();
        userInf.m_status = logInf.m_status;

        qDebug() << "searchUserInUserInformation:" <<userInf.m_userID<<userInf.m_status;
        if (updateStatus(logInf.m_status, logInf.m_userID))//更新用户登录状态
            return LOGIN_SUCCESS;
    }
    return LOGIN_FAIL;
}

/*************************************************
Function Name： searchFriendSimpleInformationByID()
Description: 通过id查询好友简易信息
*************************************************/
int TADatabaseCtl::searchFriendSimpleInformationByID(const QString &id,
                                                        FriendInformation &friInf)
{
    return 1;
}

/*************************************************
Function Name： searchFriendInformationByID()
Description: 通过id查询好友信息
*************************************************/
int TADatabaseCtl::searchFriendInformationByID(const QString &id,
                                            UserInformation &userInf)
{
    if (!createConnection())
    {
        return GET_FRIEND_FAIL;
    }
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_USER_IN_USERINFORMATION_ID_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "friend-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return GET_FRIEND_FAIL;
    }
    if (query.size() <= 0)
    {
        m_db->close();
        return GET_FRIEND_FAIL;
    }
    if (query.next())
    {
        userInf.m_userID = query.value(0).toString();
        userInf.m_headPortrait = query.value(1).toInt();
        userInf.m_nickname = query.value(2).toString();
        userInf.m_sex = query.value(4).toString();
        userInf.m_phone = query.value(7).toString();
        userInf.m_regDateTime = query.value(8).toDateTime();
        userInf.m_status = query.value(9).toInt();
        qDebug()<<userInf.m_userID<<userInf.m_nickname<<userInf.m_phone<<userInf.m_regDateTime;
    }
    return GET_FRIEND_SUCCESS;
}
int TADatabaseCtl::searchMyselfInformationByID(const QString &id,UserInformation &userInf)
{
    if (!createConnection())
    {
        return GET_MYSELF_FAIL;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_USER_IN_USERINFORMATION_ID_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "myself-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return GET_MYSELF_FAIL;
    }
    if (query.size() <= 0)
    {
        m_db->close();
        return GET_MYSELF_FAIL;
    }
    if (query.next())
    {
        userInf.m_userID = query.value(0).toString();
        userInf.m_headPortrait = query.value(1).toInt();
        userInf.m_nickname = query.value(2).toString();
        userInf.m_sex = query.value(4).toString();
        userInf.m_phone = query.value(7).toString();
        userInf.m_regDateTime = query.value(8).toDateTime();
        userInf.m_status = query.value(9).toInt();
    }
    return GET_MYSELF_SUCCESS;
}

/*************************************************
Function Name： updateMyselfInformation()
Description: 修改用户信息
*************************************************/
int TADatabaseCtl::updateMyselfInformation(const UserInformation &user)
{
    if (!createConnection())
    {
        return CHANGE_INFORMATION_FAIL;
    }
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    query.prepare(UPDATE_USERINFORMATION_SQL);
    query.addBindValue(user.m_headPortrait);
    query.addBindValue(user.m_nickname);
    query.addBindValue(user.m_sex);
    query.addBindValue(user.m_phone);
    query.addBindValue(user.m_userID);

    query.exec();
    errorSQLOrder(query, "change-update");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return CHANGE_INFORMATION_FAIL;
    }
    return CHANGE_INFORMATION_SUCCESS;
}

/*************************************************
Function Name： addUserInUserInformation()
Description: 添加用户
*************************************************/
int TADatabaseCtl::addUserInUserInformation(const UserInformation &userInf)
{
    if (!createConnection())
    {
        return REGISTER_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(ADD_USER_IN_USERINFORMATION_SQL);
    query.addBindValue(userInf.m_userID);
    query.addBindValue(userInf.m_headPortrait);
    query.addBindValue(userInf.m_nickname);
    query.addBindValue(userInf.m_password);
    query.addBindValue(userInf.m_sex);
    query.addBindValue(userInf.m_question);
    query.addBindValue(userInf.m_answer);
    query.addBindValue(userInf.m_phone);
    query.addBindValue(userInf.m_regDateTime);
    query.addBindValue(5);
    query.exec();
    errorSQLOrder(query, "register-add");
    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return REGISTER_FAIL;
    }

    if (!increaseUserTimes(QDate::currentDate()))
            return REGISTER_FAIL;
    return REGISTER_SUCCESS;
}


/*************************************************
Function Name： countRegisteredInPeriod()
Description: 获取一段时间注册数量
Input： IMUser
Output： int
Changes： NULL
*************************************************/
int TADatabaseCtl::countRegisteredInPeriod(const QDateTime &datePre,
                                            const QDateTime &dateLater)
{
    qDebug() << "among of date:" << datePre.toString("yyyy-MM-dd hh:mm:ss")
                << dateLater.toString("yyyy-MM-dd hh:mm:ss");

    if (!createConnection())
    {
        return 0;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(COUNT_REGISTERED_USERS_IN_USERINFORMATION_SQL);
    query.addBindValue(datePre);
    query.addBindValue(dateLater);
    query.exec();
    errorSQLOrder(query, "register-count");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return 0;
    }
    int sum = 0;
    if (query.next())
        sum = query.value(0).toInt() + 1;

    //m_db->close();
    return sum;

}


/*************************************************
Function Name： countFlockInPeriod()
Description: 获取一段时间群添加数量
*************************************************/
int TADatabaseCtl::countFlockInPeriod(const QDateTime &datePre,
                                 const QDateTime &dateLater)
{
    qDebug() << "among of date:" << datePre.toString("yyyy-MM-dd hh:mm:ss")
                << dateLater.toString("yyyy-MM-dd hh:mm:ss");

    if (!createConnection())
    {
        return 0;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(COUNT_FLOCKS_IN_FLOCKINFORMATION_SQL);
    query.addBindValue(datePre);
    query.addBindValue(dateLater);
    query.exec();
    errorSQLOrder(query, "register-count");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return 0;
    }
    int sum = 0;
    if (query.next())
        sum = query.value(0).toInt() + 1;

    //m_db->close();
    return sum;
}


/*************************************************
Function Name： searchFriendsInformation()
Description: 获取所有好友信息
*************************************************/
int TADatabaseCtl::searchFriendsInformation(const QString &id,
                                              QVector<FriendInformation> &friendsVec)
{
    if (!createConnection())
    {
        return GET_ALL_FRIENDS_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_FRIENDS_IN_USERFRIENDS_USERINFORMATION_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-friends-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return GET_ALL_FRIENDS_FAIL;
    }
    friendsVec.clear();
    while (query.next())
    {
        FriendInformation fri;

        fri.m_groupName = query.value(0).toString();
        fri.m_userID = query.value(1).toString();
        fri.m_nickname = query.value(2).toString();
        fri.m_remarkName = query.value(3).toString();
        fri.m_status = query.value(4).toInt();
        fri.m_headPortrait = query.value(5).toInt();
        friendsVec.push_back(fri);
    }
//    m_db->close();
    if (friendsVec.isEmpty())
    {
        return NO_FRIEND;
    }
    return GET_ALL_FRIENDS_SUCCESS;
}


/*************************************************
Function Name： searchFriendsID()
Description: 获取所有好友ID
*************************************************/
bool TADatabaseCtl::searchFriendsID(const QString &id,
                     QVector<QString> &friVec)
{
    friVec.clear();
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_ID_IN_USERFRIENDS_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-friends-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return false;
    }

    while (query.next())
    {
        friVec.push_back(query.value(0).toString());
    }

    return true;
}
void TADatabaseCtl::errorSQLOrder(QSqlQuery query, QString mark)
{
    //如果SQL语句错误，就弹出警告信息
    if(!query.isActive())
    {
        QString str = query.lastError().text() + "\n" + mark;
        qDebug() << "Mysql qurey error: " << str;
        qDebug() << "query text:" << query.lastQuery();
    }
}

/*************************************************
Function Name： createTable()
Description: 创建数据库表格
*************************************************/
void TADatabaseCtl::createTable()
{
}

/*************************************************
Function Name： updateStatus()
Description: 更新好友状态
*************************************************/
bool TADatabaseCtl::updateStatus(int status, const QString &id)
{
    qDebug() << "status is what :" << status;
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    query.prepare(UPDATE_STATUS_IN_USERINFORMATION_BY_ID_SQL);
    query.addBindValue(status);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "quit-update");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }

//    m_db->close();
    return true;
}

/*************************************************
Function Name： updateRemark()
Description: 更新好友备注
*************************************************/
int TADatabaseCtl::updateRemark(const TempStrings & tempStr)
{
    if (!createConnection())
    {
        return CHANGE_REMARK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    query.prepare(UPDATE_REMARKNAME_IN_USERFRIENDS_SQL);
    query.addBindValue(tempStr.m_three);
    query.addBindValue(tempStr.m_one);
    query.addBindValue(tempStr.m_two);
    query.exec();
    errorSQLOrder(query, "update-remark");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return CHANGE_REMARK_FAIL;
    }

//    m_db->close();
    return CHANGE_REMARK_SUCCESS;
}


/*************************************************
Function Name： deleteFriend()
Description: 删除好友
*************************************************/
int TADatabaseCtl::deleteFriend(const QString &myID, const QString &friendID,
                                   QString &groupName, QString &groupNameF)
{
    if (!createConnection())
    {
        return DELETE_FRIEND_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);

    /***********start 查询groupName***************/
    query.prepare(SEARCH_GROUPNAME_IN_USERFRIENDS_SQL);
    query.addBindValue(myID);
    query.addBindValue(friendID);
    query.exec();
    errorSQLOrder(query, "search-friend-groupName");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return DELETE_FRIEND_FAIL;
    }
    if (query.next())
    {
        groupName = query.value(0).toString();
    }
    query.clear();

    /***********start 查询groupNameF***************/
    query.prepare(SEARCH_GROUPNAME_IN_USERFRIENDS_SQL);
    query.addBindValue(friendID);
    query.addBindValue(myID);
    query.exec();
    errorSQLOrder(query, "search-friend-groupNameF");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return DELETE_FRIEND_FAIL;
    }
    if (query.next())
    {
        groupNameF = query.value(0).toString();
    }
    query.clear();

    /***********start 删除好友***************/
    query.prepare(DELETE_FRIEND_IN_USERFRIENDS_SQL);
    query.addBindValue(myID);
    query.addBindValue(friendID);
    query.addBindValue(friendID);
    query.addBindValue(myID);
    query.exec();
    errorSQLOrder(query, "delete-friend");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return DELETE_FRIEND_FAIL;
    }

//    m_db->close();
    return DELETE_FRIEND_SUCCESS;
}


/*************************************************
Function Name： addFriend()
Description: 添加好友
*************************************************/
bool TADatabaseCtl::addFriend(const TalkMessage & mes)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(ADD_FRIEND_IN_USERFRIENDS_SQL);
    query.addBindValue(mes.m_senderID);
    query.addBindValue(mes.m_receiverID);
    query.addBindValue(GROUP_NAME_DEFAULT);
    query.exec();
    errorSQLOrder(query, "add-friend1");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }
    query.clear();
    query.prepare(ADD_FRIEND_IN_USERFRIENDS_SQL);
    query.addBindValue(mes.m_receiverID);
    query.addBindValue(mes.m_senderID);
    query.addBindValue(GROUP_NAME_DEFAULT);
    query.exec();
    errorSQLOrder(query, "add-friend2");

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return true;
}
int TADatabaseCtl::searchOfflineMessage(const QString &id, QVector<TalkMessage> &mesVec)
{
    if (!createConnection())
    {
        return GET_OFFLINE_MESSAGE_FAIL;
    }
    QSqlDatabase::database().transaction();
    QSqlQuery query(*m_db);
    query.prepare(SEARCH_FRIEND_DATE_MESSAGE_IN_OFFLINEMESSAGE);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-offline_message");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return GET_OFFLINE_MESSAGE_FAIL;
    }
    mesVec.clear();
    while (query.next())
    {
        TalkMessage mes;
        mes.m_type= query.value(1).toInt();
        mes.m_senderID=query.value(2).toString();
        mes.m_receiverID=query.value(3).toString();
        mes.m_text=query.value(4).toString();
        qDebug()<<mes.m_text;
        mesVec.push_back(mes);
    }
    qDebug()<<id<<"mesVec is "<<mesVec.isEmpty();
    if (mesVec.isEmpty())
    {
        return NO_OFFLINE_MESSAGE;
    }
    else
    {
        QSqlDatabase::database().transaction();
        QSqlQuery query(*m_db);
        query.prepare(DELETE_FRIEND_DATE_MESSAGE_IN_OFFLINEMESSAGE);
        query.addBindValue(id);
        query.exec();
        errorSQLOrder(query, "delete offline message");
        QSqlDatabase::database().commit();
    }
    return GET_OFFLINE_MESSAGE_SUCCESS;
}
int TADatabaseCtl::searchFlocksInformation(const QString & id, QVector<FlockInformation> & flocksVec)
{
    if (!createConnection())
    {
        return GET_ALL_FLOCKS_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_FLOCKS_IN_FLOCKINFORMAIONT_USERFLOCKS_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-flocks-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return GET_ALL_FLOCKS_FAIL;
    }

    flocksVec.clear();
    while (query.next())
    {
        FlockInformation flock;

        flock.m_flockID = query.value(0).toString();
        flock.m_creatorID = query.value(1).toString();
        flock.m_createDate = query.value(2).toDate();
        flock.m_flockName = query.value(3).toString();
        flock.m_theme = query.value(4).toString();
        flocksVec.push_back(flock);
    }
//    m_db->close();
    if (flocksVec.isEmpty())
    {
        return NO_FLOCK;
    }
    return GET_ALL_FLOCKS_SUCCESS;
}

/*************************************************
Function Name： searchFlockInformation()
Description: 获取某个群信息
*************************************************/
bool TADatabaseCtl::searchFlockInformation(const QString & flockID,FlockInformation & flock)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_FLOCK_IN_FLOCKINFORMAIONT_SQL);
    query.addBindValue(flockID);
    query.exec();
    errorSQLOrder(query, "get-flock-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return false;
    }

    if (query.next())
    {
        flock.m_flockID = query.value(0).toString();
        flock.m_creatorID = query.value(1).toString();
        flock.m_createDate = query.value(2).toDate();
        flock.m_flockName = query.value(3).toString();
        flock.m_theme = query.value(4).toString();
        qDebug()<<"create flock sucess"<<"search flock information";
        return true;
    }
    return false;
}

/*************************************************
Function Name： searchFlockMembers()
Description: 获取某群中所有的成员
*************************************************/
int TADatabaseCtl::searchFlockMembers(
        const QString & id, QVector<FlockMember> &flockMembersVec)
{
    if (!createConnection())
    {
        return GET_FLOCK_MEMBERS_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_FLOCKMEMBERS_IN_USERINFORMATION_USERFLOCKS_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-flock-members-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return GET_FLOCK_MEMBERS_FAIL;
    }

    flockMembersVec.clear();
    while (query.next())
    {
        FlockMember flockMember;

        flockMember.m_flockID = query.value(0).toString();
        flockMember.m_userID = query.value(1).toString();
        flockMember.m_headPortrait = query.value(2).toInt();
        flockMember.m_nickname = query.value(3).toString();
        flockMember.m_loginStatus = query.value(4).toInt();
        flockMember.m_remark = query.value(5).toString();
        flockMember.m_flockStatus = query.value(6).toInt();
        flockMember.m_joinDate = query.value(7).toDateTime();
        flockMembersVec.push_back(flockMember);
    }
//    m_db->close();
    if (flockMembersVec.isEmpty())
    {
        return NO_FLOCK_MEMBER;
    }
    return GET_FLOCK_MEMBERS_SUCCESS;
}

/*************************************************
Function Name： searchFlockMember()
Description: 获取某群的某个成员
*************************************************/
bool TADatabaseCtl::searchFlockMember(const QString & flockID,
                                       const QString & userID,
                                       FlockMember & flockMember)
{

    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_FLOCKMEMBER_IN_USERINFORMATION_USERFLOCKS_SQL);
    query.addBindValue(flockID);
    query.addBindValue(userID);
    query.exec();
    errorSQLOrder(query, "get-flock-member-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return false;
    }

    if (query.next())
    {
        flockMember.m_flockID = query.value(0).toString();
        flockMember.m_userID = query.value(1).toString();
        flockMember.m_headPortrait = query.value(2).toInt();
        flockMember.m_nickname = query.value(3).toString();
        flockMember.m_loginStatus = query.value(4).toInt();
        flockMember.m_remark = query.value(5).toString();
        flockMember.m_flockStatus = query.value(6).toInt();
        flockMember.m_joinDate = query.value(7).toDateTime();
        return true;
    }
//    m_db->close();
    return false;
}
/*************************************************
Function Name： searchFlockHost()
Description: 获取群主帐号
*************************************************/
bool TADatabaseCtl::searchFlockHost(const QString & flockID, QString & hostID)
{
    if (!createConnection())
    {
        return false;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_FLOCK_CREATORID_IN_FLOCKINFORMATION_SQL);
    query.addBindValue(flockID);
    query.exec();
    errorSQLOrder(query, "flock-creatorID-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        m_db->close();
        return false;
    }
    if (query.size() <= 0)
    {
        m_db->close();
        return false;
    }
    if (query.next())
    {
        hostID = query.value(0).toString();
        return true;
    }
//    m_db->close();
    return false;
}




/*************************************************
Function Name： addFlockMember()
Description: 添加群成员
*************************************************/
bool TADatabaseCtl::addFlockMember(const QString & flockID,
                                    const QString & memberID,
                                    const int status)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);


    query.prepare(COUNT_FLOCKMEMBER_IN_USERFLOCKS_SQL);
    query.addBindValue(flockID);
    query.addBindValue(memberID);
    query.exec();

    errorSQLOrder(query, "count-flock member by id");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }

    int num = 0;
    if (query.next())
    {
        num = query.value(0).toInt();
    }
    if (0 != num)
        return false;

    query.prepare(ADD_MEMBER_IN_USERFLOCKS_SQL);
    query.addBindValue(flockID);
    query.addBindValue(memberID);
    query.addBindValue(status);
    query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.exec();
    errorSQLOrder(query, "add-flock-member");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return true;
}



/*************************************************
Function Name： addFlock()
Description: 添加群
*************************************************/
int TADatabaseCtl::addFlock(const TempStrings & tmpStr, QString & flockID)
{
    int sum = getFlockTimes(QDate::currentDate());

    if (0 == sum)
    {
        return CREATE_FLOCK_FAIL;
    }

    QString id = QDate::currentDate().toString("yyyyMMdd");
    id.append(QString::number(1));
    id.append(QString::number(sum));
    flockID = id;

    if (!createConnection())
    {
        return CREATE_FLOCK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(ADD_FLOCK_IN_FLOCKINFORMATOIN_SQL);
    query.addBindValue(id);
    query.addBindValue(tmpStr.m_one);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    query.addBindValue(tmpStr.m_two);
    query.addBindValue(tmpStr.m_three);
    query.exec();

    errorSQLOrder(query, "create-flock");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return CREATE_FLOCK_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return CREATE_FLOCK_FAIL;
    }

    QSqlDatabase::database().commit();

    if (!addFlockMember(flockID, tmpStr.m_one, FLOCK_HOST))
        return CREATE_FLOCK_FAIL;

//    m_db->close();
    if (!increaseFlockTimes(QDate::currentDate()))
            return CREATE_FLOCK_FAIL;
    return CREATE_FLOCK_SUCCESS;
}


/*************************************************
Function Name： deleteFlock()
Description: 删除群
*************************************************/
int TADatabaseCtl::deleteFlock(const QString userID, const QString & flockID)
{
    if (!createConnection())
    {
        return DROP_FLOCK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(DELETE_FLOCK_IN_FLOCKINFORMATION_SQL);
    query.addBindValue(flockID);
    query.addBindValue(userID);
    query.exec();

    errorSQLOrder(query, "delete-flock");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return DROP_FLOCK_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return DROP_FLOCK_FAIL;
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return DROP_FLOCK_SUCCESS;
}

/*************************************************
Function Name： deleteFlockMember()
Description: 删除群成员
*************************************************/
int TADatabaseCtl::deleteFlockMember(const QString & flockID,
                                       const QString & memberID)
{
    if (!createConnection())
    {
        return LEAVE_FLCCK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(DELETE_FLOCKMEMEBER_IN_USERFLOCKS_SQL);
    query.addBindValue(flockID);
    query.addBindValue(memberID);
    query.exec();

    errorSQLOrder(query, "delete-flock-member");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return LEAVE_FLCCK_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return LEAVE_FLCCK_FAIL;
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return LEAVE_FLOCK_SUCCESS;
}



/*************************************************
Function Name： updateFlockInformation()
Description: 更新群信息
*************************************************/
int TADatabaseCtl::updateFlockInformation(const FlockInformation & flock)
{
    if (!createConnection())
    {
        return CHANGE_FLOCK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(UPDATE_FLOCKINFORMATION_SQL);

    query.addBindValue(flock.m_flockName);
    query.addBindValue(flock.m_theme);
    query.addBindValue(flock.m_flockID);

    query.exec();

    errorSQLOrder(query, "update-flock-inf");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return CHANGE_FLOCK_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return CHANGE_FLOCK_FAIL;
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return CHANGE_FLOCK_SUCCESS;
}




/*************************************************
Function Name： updateFlockMemberRemark()
Description: 更新群成员名片
*************************************************/
int TADatabaseCtl::updateFlockMemberRemark(const TempStrings & tmpStr)
{
    if (!createConnection())
    {
        return  CHANGE_FLOCK_REMARK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(UPDATE_FLOCKMEMBER_REMAKR_IN_USERFLOCKS_SQL);

    query.addBindValue(tmpStr.m_three); // remark
    query.addBindValue(tmpStr.m_one);   // flockID
    query.addBindValue(tmpStr.m_two);   // userID

    query.exec();

    errorSQLOrder(query, "update-flock-member-remark");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return CHANGE_FLOCK_REMARK_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return CHANGE_FLOCK_REMARK_FAIL;
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return CHANGE_FLOCK_REMARK_SUCCESS;
}


/*************************************************
Function Name： insertValuesInDateCounter()
Description: 插入DateCounterb表某日的数据项
*************************************************/
bool TADatabaseCtl::insertValuesInDateCounter(const QDate & date)
{
    if (!createConnection())
    {
        return  false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(COUNT_DATARECORD_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "count-datecounter");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }
    int num = 0;
    if (query.next())
    {
        num = query.value(0).toInt();
    }
    if (num == 0)
    {
        query.prepare(INSERT_VALUES_IN_DATECOUNTER_SQL);
        query.addBindValue(date.toString("yyyy-MM-dd"));
        query.exec();

        errorSQLOrder(query, "insert datecounter");
        if (!query.isActive())
        {
            qDebug() << "QUERY_FAIL::" << query.lastError().text();
            m_db->close();
            return false;
        }

        if (0 >= query.numRowsAffected())
        {
            return false;
        }
    }

    QSqlDatabase::database().commit();
    return true;
}

/*************************************************
Function Name： updateFlockMemberRemark()
Description: 获取某日用户注册数量+1
*************************************************/
int TADatabaseCtl::getUserTimes(const QDate & date)
{
    if (!insertValuesInDateCounter(QDate::currentDate()))
        return 0;

    if (!createConnection())
    {
        return  0;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SELECT_USERTIMES_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "select-usertimes");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }

    int num = 0;
    if (query.next())
    {
        num = query.value(0).toInt();
    }
    QSqlDatabase::database().commit();

    return 1 + num;
}

/*************************************************
Function Name： getFlockTTAes()
Description: 获取某日新建群数量+1
*************************************************/
int TADatabaseCtl::getFlockTimes(const QDate & date)
{
    if (!insertValuesInDateCounter(QDate::currentDate()))
        return 0;

    if (!createConnection())
    {
        return  0;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SELECT_FLOCKTIMES_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "select-flocktimes");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }
    int num = 0;
    if (query.next())
    {
        num = query.value(0).toInt();
    }
    QSqlDatabase::database().commit();

    return 1 + num;
}



/*************************************************
Function Name： increaseUserTimes()
Description: 增加某日用户注册数量
*************************************************/
bool TADatabaseCtl::increaseUserTimes(const QDate & date)
{
    if (!createConnection())
    {
        return  0;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(INCREASE_USERTIMES_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "increase-usertimes");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }
    QSqlDatabase::database().commit();
    return true;
}

/*************************************************
Function Name： increaseFlockTimes()
Description: 增加某日新建群数量
*************************************************/
bool TADatabaseCtl::increaseFlockTimes(const QDate & date)
{
    if (!createConnection())
    {
        return  0;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(INCREASE_FLOCKTIMES_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "increase-flocktimes");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }
    QSqlDatabase::database().commit();
    return true;
}

/*************************************************
Function Name： moveFriendToGroup()
Description: 移动好友至其他分组
*************************************************/
int TADatabaseCtl::moveFriendToGroup(const QString & myID, const QString & friendID,
                      const QString & title, const QString & newTitle)
{
    if (!createConnection())
    {
        return MOVE_FRIEND_BOX_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(MOVE_FRIEND_IN_USERFREINS);
    query.addBindValue(newTitle);
    query.addBindValue(myID);
    query.addBindValue(friendID);
//    query.addBindValue(title);
    query.exec();

    errorSQLOrder(query, "move-friend-to-group");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return MOVE_FRIEND_BOX_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return MOVE_FRIEND_BOX_FAIL;
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return MOVE_FRIEND_BOX_SUCCESS;
}


/*************************************************
Function Name： renameGroup()
Description: 重命名分组
*************************************************/
int TADatabaseCtl::renameGroup(const QString & id, const QString & title,
                const QString & newTitle)
{
    if (!createConnection())
    {
        return RENAME_BOX_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(RENAME_GROUPNAME_IN_USERFREINS);
    query.addBindValue(newTitle);
    query.addBindValue(id);
    query.addBindValue(title);
    query.exec();

    errorSQLOrder(query, "rename-group");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return RENAME_BOX_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return RENAME_BOX_FAIL;
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return RENAME_BOX_SUCCESS;
}

/*************************************************
Function Name： addHistoryMessage()
Description: 存储历史记录
*************************************************/
bool TADatabaseCtl::addHistoryMessage(const TalkMessage & mes)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(INSERT_MESSAGE_IN_HISTORYMESSAGE);
    query.addBindValue(mes.m_senderID);
    query.addBindValue(mes.m_receiverID);
    query.addBindValue(simplifyRichText(mes.m_text));
    query.addBindValue(mes.m_dateTime);


    query.exec();

    errorSQLOrder(query, "inser-mes-to-HistoryMmesage");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return true;
}

/*************************************************
Function Name： addOfflineMessage()
Description: 存储离线记录
*************************************************/
bool TADatabaseCtl::addOfflineMessage(const TalkMessage & mes)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(INSERT_MESSAGE_IN_OFFLINEMESSAGE);
    query.addBindValue(mes.m_type);
    query.addBindValue(mes.m_senderID);
    query.addBindValue(mes.m_receiverID);
    query.addBindValue(mes.m_text);
    query.exec();

    errorSQLOrder(query, "inser-mes-to-Offline-Mmesage");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return true;
}


/*************************************************
Function Name： searchHistoryMessageOneFriendOneDate()
Description: 获取用户的某日与某位好友的聊天记录
*************************************************/
int TADatabaseCtl::searchHistoryMessageOneFriendOneDate(
        const QString & myID, const QString &friendID,
        const QDate & date, QVector<TalkMessage> & mesVec)
{
    if (!createConnection())
    {
        return GET_HISTORY_MESSAGE_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*m_db);
    query.prepare(SEARCH_FRIEND_DATE_MESSAGE_IN_HISTORYMESSAGE);
    query.addBindValue(myID);
    query.addBindValue(friendID);
    query.addBindValue(friendID);
    query.addBindValue(myID);
    query.addBindValue(date);
    query.addBindValue(date.addDays(1));
    query.exec();

    errorSQLOrder(query, "search-history-Mmesage-friend-date");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return GET_HISTORY_MESSAGE_FAIL;
    }

    mesVec.clear();
//    if (0 >= query.numRowsAffected())
//    {
//        return NO_HISTORY_MESSAGE;
//    }
    TalkMessage mes;
    while (query.next())
    {
        mes.m_senderID = query.value(0).toString();
        mes.m_receiverID = query.value(1).toString();
        mes.m_text = query.value(2).toString();
        mes.m_dateTime = query.value(3).toDateTime();
        mesVec.push_back(mes);
    }

    QSqlDatabase::database().commit();

//    m_db->close();
    return GET_HISTORY_MESSAGE_SUCCESS;
}
