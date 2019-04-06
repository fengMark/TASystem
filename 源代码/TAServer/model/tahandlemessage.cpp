#include "tahandlemessage.h"
QMap<QString, TAClientSocketCtl*> TAHandleMessage::m_userMap;
TAHandleMessage::TAHandleMessage(QObject *parent) :
    QObject(parent)
{
}
void TAHandleMessage::clientDisconnected(const QString &id)
{
        m_userMap.remove(id);
        if (m_database.updateStatus(OFFLINE, id))
            tellFriendsStatusChanged(id, OFFLINE);
}
void TAHandleMessage::sendMessage(const SaveTmpInformation &save)
{
    m_save.m_requestKind = save.m_requestKind;
    switch (m_save.m_requestKind)
    {
    case LOGIN:
    {
        m_save.m_loginInf = save.m_loginInf;
        m_save.m_clientSocket = save.m_clientSocket;
        m_save.m_myID = save.m_myID;
        if (m_userMap.contains(m_save.m_myID))
            m_save.m_replyKind = HAVE_LOGINED;
        else
        {
            m_save.m_replyKind = m_database.searchUserInUserInformation(m_save.m_loginInf, m_save.m_userInf);
        }
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case REGISTER:
    {
        m_save.m_userInf = save.m_userInf;
        int sum = m_database.getUserTimes(QDate::currentDate());
        if (0 == sum)
        {
            m_save.m_replyKind = REGISTER_FAIL;
        }
        else
        {
            QString id = QDate::currentDate().toString("yyyyMMdd");
            id.append(QString::number(0));
            id.append(QString::number(sum));
            m_save.m_userInf.m_userID = id;
            m_save.m_userInf.m_regDateTime = QDateTime::currentDateTime();
            m_save.m_replyKind = m_database.addUserInUserInformation(m_save.m_userInf);
        }
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case GET_OFFLINE_MESSAGE:
    {

        m_save.m_myID = save.m_myID;
        qDebug()<<"get OFFline message"<<save.m_myID;
        m_save.m_replyKind = m_database.searchOfflineMessage(save.m_myID,m_save.m_offlineMessageVec);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case GET_QUESTION_ANSWER:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_replyKind = m_database.searchQuestionAndAnswer(m_save.m_myID,m_save.m_tempStr);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case CHECK_MESSAGE:
        break;
    case GET_ALL_FRIENDS:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_status = save.m_status;
        m_save.m_clientSocket = save.m_clientSocket;
        // 访问数据库 查询所有好友信息
        m_save.m_replyKind = m_database.searchFriendsInformation(m_save.m_myID, m_save.m_friendsVec);
        save.m_clientSocket->sendMessage(m_save);
        if (GET_ALL_FRIENDS_FAIL != m_save.m_replyKind)
        {
            if (!m_userMap.contains(m_save.m_myID))
            {
                m_userMap.insert(m_save.m_myID, m_save.m_clientSocket);
            }
            tellFriendsStatusChanged(m_save.m_myID, m_save.m_status);
        }
        break;
    }
    case GET_ALL_FLOCKS:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_replyKind = m_database.searchFlocksInformation(m_save.m_myID, m_save.m_flocksVec);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case GET_FLOCK_MEMBERS:
    {
        m_save.m_flockID = save.m_flockID;
        // 获取群成员
        m_save.m_replyKind = m_database.searchFlockMembers(m_save.m_flockID, m_save.m_flockMembersVec);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case TALK:
    {
        talkRequest(save);
        break;
    }
    case CHANGE_INFORMATION:
    {
        m_save.m_userInf = save.m_userInf;
        m_save.m_replyKind = m_database.updateMyselfInformation(save.m_userInf);
        save.m_clientSocket->sendMessage(m_save);
        if (m_save.m_replyKind == CHANGE_INFORMATION_SUCCESS)
        {
                tellFriendsInformationChanged(m_save.m_userInf.m_userID);
        }
        break;
    }
    case GET_MYSELF_INFORMATION:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_replyKind = m_database.searchMyselfInformationByID(m_save.m_myID,m_save.m_userInf);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case GET_FRIEND_INFORMATION:
    {
        m_save.m_peerID = save.m_peerID;
        m_save.m_replyKind = m_database.searchFriendInformationByID(
                    m_save.m_peerID,
                    m_save.m_userInf);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }

    case DELETE_FRIEND:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_peerID = save.m_peerID;
        m_save.m_replyKind = m_database.deleteFriend(m_save.m_myID,
                                                     m_save.m_peerID,
                                                     m_save.m_groupName,
                                                     m_save.m_groupNameF);
        save.m_clientSocket->sendMessage(m_save);

        // 如果好友在线，就发送被删除信息， 如果不在线，就先存储
        QMap<QString, TAClientSocketCtl*>::iterator iter;
        iter = m_userMap.find(m_save.m_peerID);
        if(m_userMap.end() == iter)
        {
            TalkMessage mes;
            mes.m_type = BE_DELETED;
            mes.m_senderID = save.m_myID;
            mes.m_receiverID = save.m_peerID;
            mes.m_text = save.m_groupNameF;
            // 存储离线消息
            //m_database.messageRequest(save.message);
        }
        else
        {
            m_save.m_myID = save.m_peerID;
            m_save.m_peerID = save.m_myID;
            m_save.m_replyKind = BE_DELETED;
            m_save.m_groupName = m_save.m_groupNameF;
            iter.value()->sendMessage(m_save);
        }
        break;
    }
    case CHANGE_REMARK:
    {
        m_save.m_tempStr = save.m_tempStr;
        m_save.m_replyKind = m_database.updateRemark(save.m_tempStr);
        save.m_clientSocket->sendMessage(m_save);
//        if (m_save.m_replyKind == CHANGE_INFORMATION_SUCCESS)
//        {
//        }
        break;
    }
    case ADD_FRIEND:
    {
        friendRequest(save);
        break;
    }
    case CHANGE_STATUE:
    {
        if (m_database.updateStatus(save.m_status, save.m_myID))
            tellFriendsStatusChanged(save.m_myID, save.m_status);
        break;
    }
    case ADD_FLOCK:
    {
        flockRequest(save);
        break;
    }
    case DROP_FLOCK:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_flockID = save.m_flockID;

        tellMembersFlockDroped(m_save.m_flockID);

        /*m_save.m_replyKind = */
        m_database.deleteFlock(m_save.m_myID, m_save.m_flockID);
//        if (DROP_FLOCK_SUCCESS == m_save.m_replyKind)
//        {

//        }
        break;
    }
    case LEAVE_FLOCK:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_flockID = save.m_flockID;

      //  tellMembersFlockDroped(m_save.m_flockID);

        m_save.m_replyKind =
                m_database.deleteFlockMember(m_save.m_flockID, m_save.m_myID);

        // 如果删除成功，通知群主，群成员退出群
        if ( LEAVE_FLOCK_SUCCESS == m_save.m_replyKind)
        {
            // 通过群号flockID 获取到 群主号peerID
            if (m_database.searchFlockHost(m_save.m_flockID, m_save.m_peerID))
            {
//                save.m_clientSocket->sendMessage(m_save);

                // 如果群主在线，就发送群成员退出群信息， 如果不在线，就先存储
                QMap<QString, TAClientSocketCtl*>::iterator iter;
                iter = m_userMap.find(m_save.m_peerID);
                if(m_userMap.end() == iter)
                {
                    TalkMessage mes;
                    mes.m_type = LEAVE_FLOCK_SUCCESS;
                    mes.m_senderID = save.m_myID;
                    mes.m_receiverID = save.m_peerID;
//                    mes.m_text = save.m_groupNameF;
                    // 存储离线消息
                    //m_database.messageRequest(save.message);
                }
                else
                {
//                    TalkMessage mes;
//                    mes.m_type = LEAVE_FLOCK_SUCCESS;
//                    mes.m_senderID = save.m_myID;
//                    mes.m_receiverID = save.m_myID;
                    iter.value()->sendMessage(m_save);
                }

                // 如果群成员在线，就发送群成员退出群信息， 如果不在线，就先存储
//                QMap<QString, TAClientSocketCtrl*>::iterator iter;
                iter = m_userMap.find(m_save.m_myID);
                if(m_userMap.end() == iter)
                {
                    TalkMessage mes;
                    mes.m_type = LEAVE_FLOCK_SUCCESS;
                    mes.m_senderID = save.m_myID;
                    mes.m_receiverID = save.m_peerID;
//                    mes.m_text = save.m_groupNameF;
                    // 存储离线消息
                    //m_database.messageRequest(save.message);
                }
                else
                {
//                    TalkMessage mes;
//                    mes.m_type = LEAVE_FLOCK_SUCCESS;
//                    mes.m_senderID = save.m_myID;
//                    mes.m_receiverID = save.m_myID;
                    iter.value()->sendMessage(m_save);
                }


            }
//            tellFlockMemberHaveLeavedMember(m_save.m_flockID);

            tellFlockMemberHaveLeavedMember(m_save.m_flockID);
        }
//        save.m_clientSocket->sendMessage(m_save);


        break;
    }

    case CHANGE_FLOCK:
    {
        m_save.m_flockInf = save.m_flockInf;
        m_save.m_replyKind = m_database.updateFlockInformation(m_save.m_flockInf);
        if (CHANGE_FLOCK_SUCCESS == m_save.m_replyKind)
        {
            tellMembersFlockChanged(m_save.m_flockInf);
        }
        break;
    }
    case CHANGE_FLOCK_REMARK:
    {
        m_save.m_tempStr = save.m_tempStr;
        m_save.m_replyKind = m_database.updateFlockMemberRemark(m_save.m_tempStr);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case CREATE_FLOCK:
    {
        m_save.m_tempStr = save.m_tempStr;
        QString flockID;
        m_save.m_replyKind = m_database.addFlock(m_save.m_tempStr, flockID);
        if (CREATE_FLOCK_SUCCESS == m_save.m_replyKind)
        {
            m_database.searchFlockInformation(flockID, m_save.m_flockInf);
        }
            save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case REMOVE_BOX:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_title = save.m_title;
        break;
    }
    case CREATE_BOX:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_title = save.m_title;
        break;
    }
    case MOVE_FRIEND_BOX:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_title = save.m_title;
        m_save.m_peerID = save.m_peerID;
        m_save.m_newTitle = save.m_newTitle;
        m_save.m_replyKind = m_database.moveFriendToGroup(
                    m_save.m_myID, m_save.m_peerID,
                    m_save.m_title, m_save.m_newTitle);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case RENAME_BOX:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_title = save.m_title;
        m_save.m_newTitle = save.m_newTitle;
        m_save.m_replyKind = m_database.renameGroup(
                    m_save.m_myID, m_save.m_title, m_save.m_newTitle);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case GET_HISTORY_MESSAGE:
    {
        m_save.m_myID = save.m_myID;
        m_save.m_peerID = save.m_peerID;
        m_save.m_date = save.m_date;
        m_save.m_replyKind = m_database.searchHistoryMessageOneFriendOneDate(
                    m_save.m_myID, m_save.m_peerID,
                    m_save.m_date, m_save.m_historyMessageVec);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    case CHANGE_PASSWORD:
    {
        m_save.m_tempStr = save.m_tempStr;
        m_save.m_replyKind = m_database.updatePassword(m_save.m_tempStr);
        save.m_clientSocket->sendMessage(m_save);
        break;
    }
    default:
        break;
    }
}

void TAHandleMessage::friendRequest(const SaveTmpInformation &save)
{
    m_save.m_replyKind = ADD_FRIEND;
    m_save.m_message = save.m_message;
    switch (save.m_message.m_type)
    {
    case REQUEST_FRIEND:
    {
        if (GET_FRIEND_SUCCESS != m_database.searchFriendInformationByID(
                 m_save.m_message.m_senderID, m_save.m_userInf))
        {
            return;
        }
        break;
    }
    case REFUSE_FRIEND:
        break;
    case AGREE_FRIEND:
    {
        if (!m_database.addFriend(m_save.m_message))
        {
            m_save.m_message.m_type = ADD_FRIEND_FAIL;
            break;
        }

        m_save.m_message.m_senderID = save.m_message.m_receiverID;
        m_save.m_message.m_receiverID = save.m_message.m_senderID;
        if (GET_FRIEND_SUCCESS != m_database.searchFriendInformationByID(
                 m_save.m_message.m_senderID, m_save.m_userInf))
        {
            return;
        }
        save.m_clientSocket->sendMessage(m_save);

        m_save.m_message.m_senderID = save.m_message.m_senderID;
        m_save.m_message.m_receiverID = save.m_message.m_receiverID;
        if (GET_FRIEND_SUCCESS != m_database.searchFriendInformationByID(
                 m_save.m_message.m_senderID, m_save.m_userInf))
        {
            m_save.m_message.m_type = ADD_FRIEND_FAIL;
            return;
        }
        break;
    }
    default:
        return;
    }
    QMap<QString, TAClientSocketCtl*>::iterator iter;
    iter = m_userMap.find(m_save.m_message.m_receiverID);
    if(m_userMap.end() == iter)
    {
        // 存储离线消息
        //m_database.messageRequest(save.message);
    }
    else
        iter.value()->sendMessage(m_save);
}

void TAHandleMessage::flockRequest(const SaveTmpInformation & save)
{
    m_save.m_replyKind = ADD_FLOCK;
    m_save.m_message = save.m_message;
    switch (save.m_message.m_type)
    {
    case REQUEST_FLOCK:
    {
        // 查询到群主的ID
        if (!m_database.searchFlockHost(m_save.m_message.m_receiverID,
                                        m_save.m_peerID))
        {
            return;
        }
        qDebug() << "request user add flock";
        break;
    }
    case FLOCK_REQUEST:
    {
        m_save.m_peerID = m_save.m_message.m_receiverID;
        break;
    }
    case FLOCK_REFUSE:
    {
        m_save.m_peerID = m_save.m_message.m_receiverID;
        qDebug() << "refuse user add flock";
        break;
    }
    case FLOCK_AGREE:
    {
        if (!m_database.addFlockMember(m_save.m_message.m_senderID, m_save.m_message.m_receiverID))
        {
            m_save.m_message.m_type = ADD_FLOCK_FAIL;
            break;
        }
        m_save.m_message.m_senderID = save.m_message.m_receiverID; // 新成员
        m_save.m_message.m_receiverID = save.m_message.m_senderID; // 群号

        // 获取新成员信息，发送给群主
        if (!m_database.searchFlockMember(m_save.m_message.m_receiverID, m_save.m_message.m_senderID,m_save.m_flockMember))
        {
            return;
        }

        // 获取群信息，发送给请求入群者 以及 群主
        if (!m_database.searchFlockInformation(m_save.m_message.m_receiverID, m_save.m_flockInf))
        {
            m_save.m_message.m_type = ADD_FLOCK_FAIL;
            return;
        }

        m_save.m_message.m_type = AGREE_FLOCK;
        save.m_clientSocket->sendMessage(m_save);

        tellFlockMemberHaveNewMember(m_save.m_flockInf.m_flockID);


        m_save.m_message.m_type = FLOCK_AGREE;
        m_save.m_message.m_senderID = save.m_message.m_senderID;     // 群号
        m_save.m_message.m_receiverID = save.m_message.m_receiverID; // 新成员
        m_save.m_peerID = m_save.m_message.m_receiverID;
        break;
    }
    case AGREE_FLOCK:
    {
        // 通过群ID(receiverID)查询到群主的ID(peerID)
        if (!m_database.searchFlockHost(m_save.m_message.m_receiverID,m_save.m_peerID))
        {
            return;
        }

        // 在数据库中添加群成员信息
        if (!m_database.addFlockMember(m_save.m_message.m_receiverID,m_save.m_message.m_senderID))
        {
            m_save.m_message.m_type = ADD_FLOCK_FAIL;
            return;
        }

        // 获取新成员信息，发送给群主
        if (!m_database.searchFlockMember(m_save.m_message.m_receiverID, m_save.m_message.m_senderID,m_save.m_flockMember))
        {
            return;
        }
        // 获取群信息，发送给入群者 以及 群主
        if (!m_database.searchFlockInformation(m_save.m_message.m_receiverID, m_save.m_flockInf))
        {
            m_save.m_message.m_type = ADD_FLOCK_FAIL;
            return;
        }

        m_save.m_message.m_type = FLOCK_AGREE;
        m_save.m_message.m_senderID = save.m_message.m_receiverID;     // 群号
        m_save.m_message.m_receiverID = save.m_message.m_senderID; // 新成员
        save.m_clientSocket->sendMessage(m_save);

        tellFlockMemberHaveNewMember(m_save.m_flockInf.m_flockID);

        m_save.m_message.m_type = AGREE_FLOCK;
        m_save.m_message.m_senderID = save.m_message.m_senderID;     // 新成员
        m_save.m_message.m_receiverID = save.m_message.m_receiverID; // 群号

        break;
    }
    case REFUSE_FLOCK:
    {
        break;
    }
    default:
        return;
    }

    QMap<QString, TAClientSocketCtl*>::iterator iter;
    iter = m_userMap.find(m_save.m_peerID);
    if(m_userMap.end() != iter)
    {
        iter.value()->sendMessage(m_save);
    }

}
void TAHandleMessage::talkRequest(const SaveTmpInformation & save)
{
    m_save.m_message = save.m_message;
    switch (m_save.m_message.m_type)
    {
    case TALK_MESSAGE:
    {
        m_save.m_replyKind = TALK;
        QMap<QString, TAClientSocketCtl*>::iterator iter;
        iter = m_userMap.find(m_save.m_message.m_receiverID);


        if(m_userMap.end() == iter)
        {
            m_database.addOfflineMessage(m_save.m_message);
        }
        else
            iter.value()->sendMessage(m_save);
        break;
    }

    case REQUEST_FILE:
    {
        m_save.m_replyKind = TALK;
        QMap<QString, TAClientSocketCtl*>::iterator iter;
        iter = m_userMap.find(m_save.m_message.m_receiverID);

        m_database.addHistoryMessage(m_save.m_message);
        if(m_userMap.end() == iter)
        {
            m_database.addOfflineMessage(m_save.m_message);
        }
        else
            iter.value()->sendMessage(m_save);
        break;
    }
    case AGREE_FILE:
    {
        qDebug()<<"agree file";
        m_save.m_replyKind = TALK;
        QMap<QString, TAClientSocketCtl*>::iterator iter;
        iter = m_userMap.find(m_save.m_message.m_receiverID);

        m_database.addHistoryMessage(m_save.m_message);
        if(m_userMap.end() == iter)
        {
            m_database.addOfflineMessage(m_save.m_message);
        }
        else
            iter.value()->sendMessage(m_save);
        break;
    }
    case REFUSE_FILE:
    {
        m_save.m_replyKind = TALK;
        QMap<QString, TAClientSocketCtl*>::iterator iter;
        iter = m_userMap.find(m_save.m_message.m_receiverID);

        m_database.addHistoryMessage(m_save.m_message);
        if(m_userMap.end() == iter)
        {
            m_database.addOfflineMessage(m_save.m_message);
        }
        else
            iter.value()->sendMessage(m_save);
        break;
     }
    case TALK_FLOCK:
    {
        m_save.m_replyKind = m_database.searchFlockMembers(m_save.m_message.m_receiverID,
                                                           m_save.m_flockMembersVec);
        if (GET_FLOCK_MEMBERS_SUCCESS == m_save.m_replyKind)
        {
             m_save.m_replyKind = TALK;
            QMap<QString, TAClientSocketCtl*>::iterator iter;
            for(int i=0; i<m_save.m_flockMembersVec.size(); ++i)
            {
                iter = m_userMap.find(m_save.m_flockMembersVec[i].m_userID);
                if(m_userMap.end() != iter)
                {
                    iter.value()->sendMessage(m_save);
                }
            }
        }
        break;
    }
    default:
        break;
    }
}

void TAHandleMessage::tellFriendsStatusChanged(const QString &id, int status)
{
    m_save.m_replyKind = CHANGE_STATUE;
    m_save.m_status = status;
    m_save.m_peerID = id;
    m_database.searchFriendsID(id, m_save.m_IDVec);

    QMap<QString, TAClientSocketCtl*>::iterator iter;
    for(int i=0; i<m_save.m_IDVec.size(); ++i)
    {
        iter = m_userMap.find(m_save.m_IDVec[i]);
        if(iter != m_userMap.end())
        {
            iter.value()->sendMessage(m_save);
        }
    }
}

void TAHandleMessage::tellFriendsInformationChanged(const QString & id)
{
    m_save.m_replyKind = CHANGE_INFORMATION;
    if (!m_database.searchFriendsID(id, m_save.m_IDVec))
        return;
    m_save.m_friendInf.m_headPortrait = m_save.m_userInf.m_headPortrait;
    m_save.m_friendInf.m_userID = m_save.m_userInf.m_userID;
    m_save.m_friendInf.m_nickname = m_save.m_userInf.m_nickname;
    m_save.m_friendInf.m_status = m_save.m_userInf.m_status;

    QMap<QString, TAClientSocketCtl*>::iterator iter;
    for(int i=0; i<m_save.m_IDVec.size(); ++i)
    {
        iter = m_userMap.find(m_save.m_IDVec[i]);
        if(iter != m_userMap.end())
        {
            iter.value()->sendMessage(m_save);
        }
    }
}

void TAHandleMessage::tellMembersFlockDroped(const QString & flockID)
{
    m_save.m_replyKind = m_database.searchFlockMembers(flockID,m_save.m_flockMembersVec);
    if (GET_FLOCK_MEMBERS_SUCCESS == m_save.m_replyKind)
    {
         m_save.m_replyKind = DROP_FLOCK_SUCCESS;
         m_save.m_flockID = flockID;
        QMap<QString, TAClientSocketCtl*>::iterator iter;
        for(int i=0; i<m_save.m_flockMembersVec.size(); ++i)
        {
            iter = m_userMap.find(m_save.m_flockMembersVec[i].m_userID);
            if(m_userMap.end() == iter)
            {
                iter.value()->sendMessage(m_save);
            }
        }
    }


}

void TAHandleMessage::tellMembersFlockChanged(const FlockInformation & flock)
{
    int kind = m_database.searchFlockMembers(flock.m_flockID,m_save.m_flockMembersVec);
    if (GET_FLOCK_MEMBERS_SUCCESS == kind)
    {
        QMap<QString, TAClientSocketCtl*>::iterator iter;
        for(int i=0; i<m_save.m_flockMembersVec.size(); ++i)
        {
            iter = m_userMap.find(m_save.m_flockMembersVec[i].m_userID);
            if(iter != m_userMap.end())
            {
                iter.value()->sendMessage(m_save);
            }
        }
    }
}

void TAHandleMessage::tellFlockMemberHaveNewMember(const QString & flockID)
{
    int replyKind = m_database.searchFlockMembers(flockID,m_save.m_flockMembersVec);
    if (GET_FLOCK_MEMBERS_SUCCESS == replyKind)
    {
        m_save.m_message.m_type = NEW_FLOCK_MEMBER;

        QMap<QString, TAClientSocketCtl*>::iterator iter;
        for(int i=0; i<m_save.m_flockMembersVec.size(); ++i)
        {
            if (m_save.m_flockMembersVec[i].m_userID.compare(m_save.m_flockMember.m_userID) == 0)
                continue;
            if (m_save.m_flockMembersVec[i].m_userID.compare(m_save.m_flockInf.m_creatorID) == 0)
                continue;

            iter = m_userMap.find(m_save.m_flockMembersVec[i].m_userID);
            if(m_userMap.end() != iter)
            {
                 iter.value()->sendMessage(m_save);
            }
        }
    }
}

void TAHandleMessage::tellFlockMemberHaveLeavedMember(const QString & flockID)
{
    int replyKind = m_database.searchFlockMembers(flockID,  m_save.m_flockMembersVec);
    if (GET_FLOCK_MEMBERS_SUCCESS == replyKind)
    {
        m_save.m_replyKind = LEAVE_FLOCK_MEMBER;

        QMap<QString, TAClientSocketCtl*>::iterator iter;
        for(int i=0; i<m_save.m_flockMembersVec.size(); ++i)
        {
            if (m_save.m_flockMembersVec[i].m_userID.compare(m_save.m_peerID) == 0)
                continue;

            iter = m_userMap.find(m_save.m_flockMembersVec[i].m_userID);
            if(m_userMap.end() != iter)
                iter.value()->sendMessage(m_save);
        }
    }
}


