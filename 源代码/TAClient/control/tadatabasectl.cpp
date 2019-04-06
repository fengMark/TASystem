#include "tadatabasectl.h"

/************************message************************/
static const QString INSERT_MESSAGE_IN_HISTORYMESSAGE =
        "insert into ##HISTORY##MESSAGE## (senderID, receiverID, content, dateTime) "
        "values(?, ?, ?, ?);";
static const QString SEARCH_MESSAGE_IN_HISTORYMESSAGE =
        "select * from ##HISTORY##MESSAGE## where ( (senderID = ? and receiverID = ?) "
        "or (senderID = ? and receiverID = ?) )and dateTime >=? and dateTime < ?;";
static const QString EXPORT_ONE_MESSAGE_IN_HISTORY =
        "select * from ##HISTORY##MESSAGE## where (senderID = ? and receiverID = ?) or "
        "(senderID = ? and receiverID = ?) into outfile ? "
        "fields terminated by ',' optionally enclosed by '\"' escaped by '\"'"
        "lines terminated by '\\r\\n';";

static const QString EXPORT_ALL_MESSAGE_IN_HISTORY =
        "select * from ##HISTORY##MESSAGE## where senderID = ? or receiverID = ? "
        "into outfile ? "
        "fields terminated by ',' optionally enclosed by '\"' escaped by '\"'"
        "lines terminated by '\\r\\n';";

static const QString DELETE_ONE_MESSAGE_IN_HISTORY =
        "delete from ##HISTORY##MESSAGE## where (senderID = ? and receiverID = ?) "
        "or (senderID = ? and receiverID = ?)";
static const QString DELETE_ALL_MESSAGE_IN_HISTORY =
        "delete from ##HISTORY##MESSAGE## where senderID = ? or receiverID = ? ";

/********************create tables********************/
static const QString CREATE_TABLE_HISTORYMESSAGE_SQL =
"create table if not exists ##HISTORY##MESSAGE## "
"("
"messageID int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY, "
"senderID varchar(15) NOT NULL, "
"receiverID varchar(15) NOT NULL, "
"content varchar(1000) NOT NULL DEFAULT '', "
"dateTime datetime NOT NULL "
")ENGINE=InnoDB DEFAULT CHARSET=utf8;";

TADatabaseCtl::TADatabaseCtl(const QString myID, QObject *parent) :
    QObject(parent), m_myID(myID)
{
    m_localHistoryTableName = "HistoryMessage";
    m_localHistoryTableName.append(m_myID);

    m_db = new QSqlDatabase;
    createConnection();
    createTable();
    close();
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

/*************************************************
Function Name： createConnection()
Description: 创建连接
*************************************************/
bool TADatabaseCtl::createConnection()
{
    if (m_db == NULL)
        m_db = new QSqlDatabase;
    if (m_db->isOpen())
    {
        return true;
    }
    if (QSqlDatabase::contains("TAClientDB"))
    {
        *m_db = QSqlDatabase::database("TAClientDB");
    }
    else
    {
        *m_db = QSqlDatabase::addDatabase("QMYSQL", "TAClientDB");
        m_db->setDatabaseName("TAClientDB");
    }

    m_db->setUserName("root");
    m_db->setPassword("1");
    m_db->setHostName(QLatin1String("localhost"));
    if (!m_db->open())
    {
        return false;
    }
    return true;
}


/*************************************************
Function Name： close()
Description: 关闭数据库
*************************************************/
void TADatabaseCtl::close()
{
    if (m_db != NULL && m_db->isOpen())
        m_db->close();
}


/*************************************************
Function Name： isOpen()
Description: 是否连接着
*************************************************/
bool TADatabaseCtl::isOpen()
{
    return (m_db->isOpen());

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

    QString queryStr = INSERT_MESSAGE_IN_HISTORYMESSAGE;
    queryStr.replace("##HISTORY##MESSAGE##", m_localHistoryTableName);
    QSqlQuery query(*m_db);
    query.prepare(queryStr);
    query.addBindValue(mes.TA_senderID);
    query.addBindValue(mes.TA_receiverID);
    query.addBindValue(TAClientFileCtl::simplifyRichText(mes.TA_text));
    query.addBindValue(mes.TA_dateTime);

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
    return true;
}

/*************************************************
Function Name： searchHistoryMessage()
Description: 获取某日的消息记录
*************************************************/
bool TADatabaseCtl::searchHistoryMessage(const QString & myID, const QString & friendID,
                                          const QDate & date, QVector<TalkMessage> & mesVec)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QString queryStr = SEARCH_MESSAGE_IN_HISTORYMESSAGE;
    queryStr.replace("##HISTORY##MESSAGE##", m_localHistoryTableName);
    QSqlQuery query(*m_db);
    query.prepare(queryStr);
    query.addBindValue(myID);
    query.addBindValue(friendID);
    query.addBindValue(friendID);
    query.addBindValue(myID);
    query.addBindValue(date);
    query.addBindValue(date.addDays(1));

    query.exec();

    errorSQLOrder(query, "search-mes-in-HistoryMmesage");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return false;
    }

    TalkMessage mes;
    while (query.next())
    {
        mes.TA_senderID = query.value(1).toString();
        mes.TA_receiverID = query.value(2).toString();
        mes.TA_text = query.value(3).toString();
        mes.TA_dateTime = query.value(4).toDateTime();
        mesVec.push_back(mes);
    }

    QSqlDatabase::database().commit();
    return true;
}


/*************************************************
Function Name： searchHistoryMessage()
Description: 导出好友消息记录
*************************************************/
int TADatabaseCtl::exportHistoryMessageOneFriend(const QString &myID ,const QString &friendID,const QString &name)
{
    if (!createConnection())
    {
        return -1;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QString queryStr = EXPORT_ONE_MESSAGE_IN_HISTORY;
    queryStr.replace("##HISTORY##MESSAGE##", m_localHistoryTableName);
    QSqlQuery query(*m_db);
    query.prepare(queryStr);
    query.addBindValue(myID);
    query.addBindValue(friendID);
    query.addBindValue(friendID);
    query.addBindValue(myID);
    query.addBindValue(name);

    query.exec();

    errorSQLOrder(query, "exportl-one-message");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return -1;
    }

    if (0 >= query.numRowsAffected())
    {
        m_db->close();
        return 0;
    }
    QSqlDatabase::database().commit();
    m_db->close();
    return 1;
}

/*************************************************
Function Name： exportHistoryMessageAll()
Description: 导出所有消息记录
*************************************************/
int TADatabaseCtl::exportHistoryMessageAll(const QString &myID,const QString &fileName)
{
    if (!createConnection())
    {
        return -1;
    }

    QString queryStr = EXPORT_ALL_MESSAGE_IN_HISTORY;
    queryStr.replace("##HISTORY##MESSAGE##", m_localHistoryTableName);
    QSqlQuery query(*m_db);
    query.prepare(queryStr);
    query.addBindValue(myID);
    query.addBindValue(myID);
    query.addBindValue(fileName);

    query.exec();

    errorSQLOrder(query, "exportl-all-message");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return -1;
    }

    if (0 >= query.numRowsAffected())
    {
        m_db->close();
        return 0;
    }
    QSqlDatabase::database().commit();
    m_db->close();
    return 1;
}

/*************************************************
Function Name： deleteHistoryMessageOneFriend()
Description: 删除单个好友消息记录
*************************************************/
int TADatabaseCtl::deleteHistoryMessageOneFriend(const QString myID,const QString friendID)
{
    if (!createConnection())
    {
        return -1;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QString queryStr = DELETE_ONE_MESSAGE_IN_HISTORY;
    queryStr.replace("##HISTORY##MESSAGE##", m_localHistoryTableName);
    QSqlQuery query(*m_db);
    query.prepare(queryStr);
    query.addBindValue(myID);
    query.addBindValue(friendID);
    query.addBindValue(friendID);
    query.addBindValue(myID);


    query.exec();

    errorSQLOrder(query, "delete-one-message");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return -1;
    }

    if (0 >= query.numRowsAffected())
    {
        m_db->close();
        return 0;
    }
    QSqlDatabase::database().commit();
    m_db->close();
    return 1;
}

/*************************************************
Function Name： deleteHistoryMessageAll()
Description: 删除所有好友消息记录
*************************************************/
int TADatabaseCtl::deleteHistoryMessageAll(const QString myID)
{
    if (!createConnection())
    {
        return -1;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QString queryStr = DELETE_ALL_MESSAGE_IN_HISTORY;
    queryStr.replace("##HISTORY##MESSAGE##", m_localHistoryTableName);
    QSqlQuery query(*m_db);
    query.prepare(queryStr);
    query.addBindValue(myID);
    query.addBindValue(myID);
    query.exec();

    errorSQLOrder(query, "delete-all-message");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        m_db->close();
        return -1;
    }

    if (0 >= query.numRowsAffected())
    {
        m_db->close();
        return 0;
    }
    QSqlDatabase::database().commit();
    m_db->close();
    return 1;
}


/*************************************************
Function Name： errorSQLOrder()
Description: 检查SQL语句的正确性
*************************************************/
void TADatabaseCtl::errorSQLOrder(QSqlQuery query, QString mark)
{
    //如果SQL语句错误，就弹出警告信息
    if(!query.isActive())
    {
        QString str = query.lastError().text() + "\n" + mark;
        //QMessageBox::warning(NULL, "ERROR", str);
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
    QString queryStr = CREATE_TABLE_HISTORYMESSAGE_SQL;
    queryStr.replace("##HISTORY##MESSAGE##", m_localHistoryTableName);
    QSqlQuery query(*m_db);
    query.exec(queryStr);
    errorSQLOrder(query, "createTable-historymessage");
}
