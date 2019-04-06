#include "taclientsocketctl.h"
#include "model/tauser.h"
#include <QMultiMap>
#include <QDateTime>
TAClientSocketCtl::TAClientSocketCtl(QObject *parent):
    QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteScoket()));
    m_blockSize = 0;
    m_save.m_clientSocket = this;
}

TAClientSocketCtl::~TAClientSocketCtl()
{
    if (m_database.isOpen())
        m_database.close();
}
void TAClientSocketCtl::receiveMessage()

{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_7);
    if (m_blockSize == 0)
    {
        if (bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> m_blockSize;
    }

    if (bytesAvailable() < m_blockSize)
        return;

    in >> m_save.m_requestKind;

    qDebug() << "receive message: " << m_save.m_requestKind;

    switch (m_save.m_requestKind)
    {
    case CHECK_CONNECTION:
    {
        m_save.m_replyKind = CHECK_CONNECTION;
        sendMessage(m_save);
        m_blockSize = 0;
        break;
    }
    case LOGIN:
    {
        in >> m_save.m_loginInf;
        m_save.m_myID = m_save.m_loginInf.m_userID;
        m_save.m_socketID = m_save.m_myID;
        break;
    }
    case CHECK_MESSAGE:
    {
        in >> m_save.m_myID;
        break;
    }
    case REGISTER:
    {
        in >> m_save.m_userInf;
        break;
    }
    case GET_OFFLINE_MESSAGE:
    {
        in >> m_save.m_myID;
        break;
    }
    case GET_QUESTION_ANSWER:
    {
        in >> m_save.m_myID;
        break;
    }
    case TALK:
    {
        in >> m_save.m_message;
        break;
    }
    case GET_ALL_FRIENDS:
    {
        in >> m_save.m_myID >> m_save.m_status;
        m_save.m_socketID = m_save.m_myID;
        break;
    }
    case GET_ALL_FLOCKS:
    {
        in >> m_save.m_myID;
        break;
    }

    case GET_FLOCK_MEMBERS:
    {
        in >> m_save.m_flockID;
        break;
    }
    case GET_FRIEND_INFORMATION:
    {
        in >> m_save.m_peerID;
        break;
    }
    case GET_MYSELF_INFORMATION:
    {
        in >> m_save.m_myID;
        break;
    }
    case CHANGE_INFORMATION:
    {
        in >> m_save.m_userInf;
        break;
    }
    case CHANGE_REMARK:
    {
        in >> m_save.m_tempStr;
        break;
    }
    case CHANGE_STATUE:
    {
        in >> m_save.m_myID >> m_save.m_status;
        break;
    }
    case ADD_FRIEND:
    {
        in >> m_save.m_message;
        break;
    }
    case REFUSE_FRIEND:
    {
        //in >> m_save.m_peerID;
        break;
    }
    case DELETE_FRIEND:
    {
        in >> m_save.m_myID >> m_save.m_peerID;
        break;
    }

    case ADD_FLOCK:
    {
        in >> m_save.m_message;
        break;
    }
    case DROP_FLOCK:
    {
        in >> m_save.m_myID >> m_save.m_flockID;
        break;
    }
    case LEAVE_FLOCK:
    {
        in >> m_save.m_myID >> m_save.m_flockID;
        break;
    }
    case CHANGE_FLOCK:
    {
        in >> m_save.m_flockInf;
        break;
    }
    case CHANGE_FLOCK_REMARK:
    {
        in >> m_save.m_tempStr;
        break;
    }
    case CREATE_FLOCK:
    {
        in >> m_save.m_tempStr;
        qDebug()<<"create flock";
        break;
    }


    case REMOVE_BOX:
    case CREATE_BOX:
    {
        in >> m_save.m_myID >> m_save.m_title;
        break;
    }
    case MOVE_FRIEND_BOX:
    {
        in >> m_save.m_myID >> m_save.m_peerID
           >> m_save.m_title >> m_save.m_newTitle;
        break;
    }
    case RENAME_BOX:
    {
        in >> m_save.m_myID >> m_save.m_title >> m_save.m_newTitle;
        break;
    }
    case GET_HISTORY_MESSAGE:
    {
        in >> m_save.m_myID >> m_save.m_peerID >> m_save.m_date;
        break;
    }
    case CHANGE_PASSWORD:
    {
        in >> m_save.m_tempStr;
        break;
    }

    default:
        break;
    }


    QByteArray data = this->readAll();
    qDebug() << "leaved in socket: " << data.size();

    m_blockSize =0;
    emit sendSignal(m_save);
}
void TAClientSocketCtl::sendMessage(const SaveTmpInformation &temp)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << (int)temp.m_replyKind;

    switch (temp.m_replyKind)
    {
        case LOGIN_SUCCESS:
        {
            out << temp.m_userInf;
            qDebug()<<"login sucess"<<temp.m_userInf.m_nickname;
            break;
        }
        case REGISTER_SUCCESS:
        {
            out << temp.m_userInf.m_userID << temp.m_userInf.m_regDateTime;
            break;
        }
        case GET_QUESTION_ANSWER_SUCCESS:
        {
            out << temp.m_tempStr;
            break;
        }
        case GET_ALL_FRIENDS_SUCCESS:
        {
            int len = temp.m_friendsVec.size();
            out << len;
            for (int i=0; i<len; ++i)
                out << temp.m_friendsVec[i];
            break;
        }
        case GET_ALL_FLOCKS_SUCCESS:
        {
            int len = temp.m_flocksVec.size();
            out << len;
            for (int i=0; i<len; ++i)
                out << temp.m_flocksVec[i];
            break;
        }
        case GET_FLOCK_MEMBERS_SUCCESS:
        {
            int len = temp.m_flockMembersVec.size();
            out << len;
            for (int i=0; i<len; ++i)
                out << temp.m_flockMembersVec[i];
            break;
        }
        case HAVE_MESSAGE:
            break;
        case CHANGE_STATUE:
            {
            out << temp.m_peerID << temp.m_status;
            break;
        }
        case GET_MYSELF_SUCCESS:
        {
            out << temp.m_userInf;
            break;
        }
        case DELETE_FRIEND_SUCCESS:
        {
            out << temp.m_myID << temp.m_peerID << temp.m_groupName;
            break;
        }
        case CHANGE_INFORMATION_SUCCESS:
        {
            out << temp.m_userInf;
            break;
        }
        case CHANGE_INFORMATION:
        {
            out << temp.m_friendInf;
            break;
        }
        case ADD_FRIEND:
        {
            out << temp.m_message << temp.m_userInf;
            break;
        }
        case GET_FRIEND_SUCCESS:
        {
            out << temp.m_userInf;
            break;
        }
        case TALK:
            out << temp.m_message;
            break;
        case CHANGE_REMARK_SUCCESS:
        {
            out << temp.m_tempStr;
            break;
        }
        case BE_DELETED:
        {
            out << temp.m_myID << temp.m_peerID << temp.m_groupNameF;
            break;
        }
        case REQUEST_FLOCK:
        {
            out << temp.m_message;
            break;
        }
        case ADD_FLOCK:
        {
            out << temp.m_message << temp.m_flockMember << temp.m_flockInf;
            break;
        }

        case DROP_FLOCK_SUCCESS:
        {
            out << temp.m_flockID;
            break;
        }
         case LEAVE_FLOCK_SUCCESS:
        {
            out << temp.m_myID << temp.m_flockID << temp.m_peerID;
            break;
        }
        case LEAVE_FLOCK_MEMBER:
        {
            out << temp.m_myID << temp.m_flockID << temp.m_peerID;
            break;
        }
        case CHANGE_FLOCK_SUCCESS:
        {
            out << temp.m_flockInf;
            break;
        }
        case CHANGE_FLOCK_REMARK_SUCCESS:
        {
            out << temp.m_tempStr;
            break;
        }
        case CREATE_FLOCK_SUCCESS:
        {
            out << temp.m_flockInf;
            break;
        }
        case RENAME_BOX_SUCCESS:
        {
            out << temp.m_myID << temp.m_title << temp.m_newTitle;
            break;
        }
        case MOVE_FRIEND_BOX_SUCCESS:
        {
            out << temp.m_myID << temp.m_peerID << temp.m_title << temp.m_newTitle;
            break;
        }
        case GET_HISTORY_MESSAGE_SUCCESS:
        {
            out << temp.m_myID << temp.m_peerID << temp.m_date;
            int len = temp.m_historyMessageVec.size();
            out << len;
            for (int i=0; i<len; ++i)
                out << temp.m_historyMessageVec[i];
            break;
        }
        default:
            break;
    }
    out.device()->seek(0);
    qDebug()<<"write message";
    out << quint16(block.size() - sizeof(quint16));
    write(block);
}
void TAClientSocketCtl::deleteScoket()
{
    if (!m_save.m_socketID.isEmpty())
    {
        if ( LOGIN != m_save.m_requestKind)
        emit deleteSignal(m_save.m_socketID);
    }
    deleteLater();
}
