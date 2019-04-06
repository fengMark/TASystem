#include "tamainctl.h"
TAMainCtl::TAMainCtl(const QString myID, QObject *parent) :
    TA_myID(myID), QObject(parent)
{
    TA_tcpSocket = new TATcpSocket(this);
    timer=new QTimer(this);
    connect (timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    timer->start (10000);
    connect(TA_tcpSocket, SIGNAL(connected()), this, SLOT(requestGetFriendsInformation()));
    connect(TA_tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(TA_tcpSocket, SIGNAL(disconnected()), this, SLOT(closeWindow()));/*
    connect(TA_udpSocket,SIGNAL(readyRead()),this,SLOT(readTalkMessage()));
    connect(TA_udpSocket,SIGNAL(readyToreadMessage()),this,SLOT(readTalkMessage()));*/
}
void TAMainCtl::deleteFriend(const QString &myID, const QString &friendID){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestDeleteFriend(myID, friendID);
}
void TAMainCtl::getFriendsInformation(const QString &uid, const int status){
    TA_myID = uid;
    TA_status = status;
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
    {
        requestGetFriendsInformation();
    }
    else
    {
        TA_blockSize = 0;
        TA_tcpSocket->requestConnect();
    }
}
void TAMainCtl::getFlockMembers(const QString &id){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestGetFlockMembers(id);
}
void TAMainCtl::sendTalkMessage(TalkMessage &mes){
        mes.TA_senderID = TA_myID;
        mes.TA_senderIP = TATcpSocket::getIP();
        TA_messageVector.push_back(mes);
        if (NULL == TA_tcpSocket)
            return;
        if (TA_tcpSocket->isConnected())
        {
            requestSendTalkMessage();
        }
}
void TAMainCtl::requestCreateFlock(const TempStrings &tmpStr)
{
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(CREATE_FLOCK) << tmpStr;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestSendTalkMessage(){
    if (NULL == TA_tcpSocket)
        return;

    while (TA_messageVector.size() > 0)
    {
        TA_blockSize = 0;
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_7);
        out << quint16(0) << int(TALK)
            << TA_messageVector.front();
        out.device()->seek(0);
        out << quint16(block.size() - sizeof(quint16));
        TA_tcpSocket->write(block);

        TA_messageVector.pop_front();
    }
}
void TAMainCtl::requestChangePwd(const QString &oldPwd, const QString &newPwd){
    if (NULL == TA_tcpSocket)
        return;

    TempStrings tmpStr;
    tmpStr.TA_one = TA_myID;
    tmpStr.TA_two = oldPwd;
    tmpStr.TA_three = newPwd;

    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(CHANGE_PASSWORD) << tmpStr;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestDeleteFriend(const QString &myID, const QString &friendID){
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(DELETE_FRIEND) << myID << friendID;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::getFlocksInformation(const QString &id){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestGetFlocksInformation(id);
}
void TAMainCtl::requestGetFlocksInformation(const QString &id){
    if (NULL == TA_tcpSocket)
        return;
    qDebug() << "request Flocks: " << id;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(GET_ALL_FLOCKS) << id;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestGetFriendsInformation(){
    if (NULL == TA_tcpSocket)
        return;
    qDebug() << "request friends: " << TA_myID;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(GET_ALL_FRIENDS) << TA_myID << TA_status;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::getFriendInformation(const QString &id){
        if (NULL == TA_tcpSocket)
            return;
        if (TA_tcpSocket->isConnected())
        {
            requestGetFriendInformation(id);
        }
}
void TAMainCtl::requestMoveFriendToBox(const QString & friendID,const QString & oldTitle,const QString & newTitle)
{
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(MOVE_FRIEND_BOX) << TA_myID << friendID << oldTitle << newTitle;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestGetFriendInformation(const QString &id){
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(GET_FRIEND_INFORMATION) << id;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::moveFriendToBox(const QString &friendID, const QString &oldTitle, const QString &newTitle){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestMoveFriendToBox(friendID, oldTitle, newTitle);
}
void TAMainCtl::dropFlock(const QString &userID, const QString &flockID){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestDropFlock(userID, flockID);
}
void TAMainCtl::requestChangeFlockInformation(const FlockInformation &flock){
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(CHANGE_FLOCK) << flock;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestgetLatestMessage(const QString &id){
    if (NULL == TA_tcpSocket)
        return;
    qDebug() << "request New message: " << id;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(GET_OFFLINE_MESSAGE) << id;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestRenameBox(const QString &oldTitle, const QString &newTitle){
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(RENAME_BOX) << TA_myID << oldTitle << newTitle;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestSendResultOfFriendRequest(const TalkMessage &mes){
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(ADD_FRIEND) << mes;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::leaveFlock(const QString &userID, const QString &flockID){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestLeaveFlock(userID, flockID);
}
void TAMainCtl::uploadIconFile(const QString &icon_name, const QString &userID){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestuploadIconFile(icon_name, userID);
}
void TAMainCtl::changeFlockMemberRemark(const TempStrings &tmpStr){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestChangeFlockMemberRemark(tmpStr);
}
void TAMainCtl::changeFlockInformation(const FlockInformation &flock){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestChangeFlockInformation(flock);
}
void TAMainCtl::requestSendResultOfFlockRequest(const TalkMessage &mes){
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(ADD_FLOCK) << mes;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::resultOfFlockRequest(const TalkMessage &mes){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestSendResultOfFlockRequest(mes);
}
void TAMainCtl::resultOfFriendRequest(const TalkMessage &mes){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestSendResultOfFriendRequest(mes);
}
void TAMainCtl::changeFriendRemark(const QString &myID, const QString &friendID, const QString &remark){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestChangeFriendRemark(myID, friendID, remark);
}
void TAMainCtl::getLatestMessage(const QString &id){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected()){
        requestgetLatestMessage(id);
    }
}
void TAMainCtl::addFriend(const TalkMessage &mes){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected()){
        requestAddFriend(mes);
    }
}
void TAMainCtl::addFlock(const TalkMessage &mes){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected()){
        requestAddFlock(mes);
    }
}
void TAMainCtl::requestAddFlock(const TalkMessage &mes){
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(ADD_FLOCK) << mes;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::createFlock(const TempStrings & tmpStr)
{
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestCreateFlock(tmpStr);
}
void TAMainCtl::changeMyStatus(const QString id, const int status){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestChangeMyStatus(id, status);
}
void TAMainCtl::requestChangeFlockMemberRemark(const TempStrings &tmpStr){
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(CHANGE_FLOCK_REMARK) << tmpStr;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);

}
void TAMainCtl::requestuploadIconFile(const QString &icon_name, const QString &userID){
    if (NULL == TA_tcpSocket)
        return;
    qDebug() << "request upload Icon File: " << userID;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(UPLOAD_ICONFILE) << userID<<icon_name;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestGetFlockMembers(const QString &id){
    if (NULL == TA_tcpSocket)
        return;
    qDebug() << "request flock members: " << id;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(GET_FLOCK_MEMBERS) << id;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestLeaveFlock(const QString &userID, const QString &flockID){
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(LEAVE_FLOCK) << userID << flockID;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestDropFlock(const QString &userID, const QString &flockID){
    if (NULL ==TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(DROP_FLOCK) << userID << flockID;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestGetMyInformation()
{
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(GET_MYSELF_INFORMATION) << TA_myID;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::getMyinformation(const QString &id){
    TA_myID = id;
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
    {
        requestGetMyInformation();
    }
    else
    {
        TA_blockSize = 0;
        TA_tcpSocket->requestConnect();
    }
}
void TAMainCtl::closeConnect(){
    TA_tcpSocket->close();
}
void TAMainCtl::renameBox(const QString &oldTitle, const QString &newTitle){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
        requestRenameBox(oldTitle, newTitle);
}
void TAMainCtl::requestChangeMyStatus(const QString id, const int status){
    if (NULL == TA_tcpSocket)
        return;

    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(CHANGE_STATUE) << id << status;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::changeMyInformation(const UserInformation &me){
    if (NULL == TA_tcpSocket)
        return;
    if (TA_tcpSocket->isConnected())
    {
        requestChangeMyInformation(me);
    }
    else
    {
        TA_blockSize = 0;
        TA_tcpSocket->requestConnect();
    }
}
void TAMainCtl::requestAddFriend(const TalkMessage &mes){
    if (NULL == TA_tcpSocket)
        return;
    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(ADD_FRIEND) << mes;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestChangeMyInformation(const UserInformation &me)
{
    if (NULL == TA_tcpSocket)
        return;

    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(CHANGE_INFORMATION) << me;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::requestChangeFriendRemark(const QString &myID, const QString &friendID, const QString &remark){
    if (NULL == TA_tcpSocket)
        return;
    TempStrings tempStr;
    tempStr.TA_one = myID;
    tempStr.TA_two = friendID;
    tempStr.TA_three = remark;

    TA_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(CHANGE_REMARK) << tempStr;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    TA_tcpSocket->write(block);
}
void TAMainCtl::readMessage(){
    if (NULL == TA_tcpSocket)
        return;

    QDataStream in(TA_tcpSocket);
    in.setVersion(QDataStream::Qt_4_7);
    if (TA_blockSize == 0)
    {
        if (TA_tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> TA_blockSize;
    }

    if (TA_tcpSocket->bytesAvailable() < TA_blockSize)
        return;
    int type;

    in >> type;
    int len;

    switch (type)
    {
    case NO_FRIEND:
    {
        requestGetFlocksInformation(TA_myID);
        break;
    }
    case GET_ALL_FRIENDS_FAIL:
        break;
    case GET_ALL_FRIENDS_SUCCESS:
    {
        in >> len;
        TA_friendsVec.clear();
        TA_friendsVec.resize(len);
        for(int i=0; i<len; i++)
        {
            in >> TA_friendsVec[i];
        }
        emit getFriendsSuccess(TA_friendsVec);
        break;
    }
    case NO_FLOCK:
    {
        getLatestMessage(TA_myID);
        break;
    }
    case GET_ALL_FLOCKS_FAIL:
    {
        getLatestMessage(TA_myID);
        break;
    }
    case UPLOAD_ICONFILE:
    {
        in>>headPortrait;
        emit uploadIconFileSuccess(headPortrait);
    }
    case GET_ALL_FLOCKS_SUCCESS:
    {
        in >> len;
        TA_flocksVec.clear();
        TA_flocksVec.resize(len);
        for(int i=0; i<len; i++)
        {
            in >> TA_flocksVec[i];
        }
        qDebug() << "flocks len: " << len;
        emit getFlocksSuccess(TA_flocksVec);
        getLatestMessage(TA_myID);
        break;
    }

    case GET_FLOCK_MEMBERS_SUCCESS:
    {
        in >> len;
        TA_flockMembersVec.clear();
        TA_flockMembersVec.resize(len);
        for(int i=0; i<len; i++)
        {
            in >> TA_flockMembersVec[i];
        }
        qDebug() << "flock-members len: " << len;
        emit getFlockMembersSuccess(TA_flockMembersVec);
        break;
    }
    case GET_FRIEND_FAIL:
        break;
    case GET_FRIEND_SUCCESS:
    {
        in >> TA_friInf;
        emit getFriendInformationSuccess(TA_friInf);
        break;
    }
    case GET_MYSELF_FAIL:
    {
        break;
    }
    case GET_MYSELF_SUCCESS:
    {
        in >> TA_myInf;
        emit getMyInformationSuccess(TA_myInf);
        break;
    }
    case CHANGE_INFORMATION_FAIL:
    {
        QMessageBox::critical(NULL, tr("修改个人信息"), tr("修改个人信息失败"));
        break;
    }
    case CHANGE_INFORMATION_SUCCESS:
    {
        in >> TA_myInf;
        emit changeMyInformationSuccess(TA_myInf);
        QMessageBox::information(NULL, tr("修改个人信息"), tr("修改个人信息成功"));
        break;
    }
    case TALK:
    {
        in >> TA_message;
        emit getFriendTalkMessage(TA_message);
        break;
    }
    case CHANGE_STATUE:
    {
        in >> TA_peerID >> TA_peerStatus;
        emit getFriendChangedStatus(TA_peerID, TA_peerStatus);
        break;
    }
    case CHANGE_INFORMATION:
    {
        in >> TA_friSimInf;
        emit getFriendChangedInformation(TA_friSimInf);
        break;
    }
    case CHANGE_REMARK_SUCCESS:
    {
        in >> TA_tempStr;
        emit changeFriendRemarkSuccess(TA_tempStr);
        break;
    }
    case DELETE_FRIEND_SUCCESS:
    {
        in >> TA_id >> TA_peerID >> TA_groupName;
        emit deleteFriendSuccess(TA_id, TA_peerID, TA_groupName);
        break;
    }
    case DELETE_FRIEND_FAIL:
    {
        break;
    }
    case BE_DELETED:
    {
        in >> TA_id >> TA_peerID >> TA_groupName;

        QString temp = QString(tr("你被好友(%1)删除了")).arg(TA_peerID);
        QMessageBox::information(NULL, tr("消息"), temp);
        emit deleteFriendSuccess(TA_id, TA_peerID, TA_groupName);
        break;
    }
    case ADD_FRIEND:
    {
        in >> TA_message >> TA_friInf;
        emit getFriendRequest(TA_message, TA_friInf);
        break;
    }
    case ADD_FLOCK:
    {
        in >> TA_message >> TA_flockMember >> TA_flockInf;
        emit getFlockRequest(TA_message, TA_flockMember, TA_flockInf);
        break;
    }
    case DROP_FLOCK_SUCCESS:
    {
        in >> TA_flockID;
        emit dropFlockSuccess(TA_flockID);
        break;
    }
    case LEAVE_FLOCK_SUCCESS:
    {
        in >> TA_id >> TA_flockID >> TA_creatorID;
        emit leaveFlockSuccess(TA_id, TA_flockID, TA_creatorID);
        break;
    }
    case LEAVE_FLOCK_MEMBER:
    {
        in >> TA_id >> TA_flockID >> TA_creatorID;
        emit haveMemberleavedFlock(TA_id, TA_flockID, TA_creatorID);
        break;
    }
    case CHANGE_FLOCK_SUCCESS:
    {
        in >> TA_flockInf;
        emit changeFlockSuccess(TA_flockInf);
        break;
    }
    case CHANGE_FLOCK_REMARK_SUCCESS:
    {
        in >> TA_tempStr;
        emit changeFlockRemarkSuccess(TA_tempStr);
        break;
    }
    case CREATE_FLOCK_SUCCESS:
    {
        in >> TA_flockInf;
        emit createFlockSuccess(TA_flockInf);
        break;
    }
    case RENAME_BOX_SUCCESS:
    {
        in >> TA_id >> TA_title >> TA_newTitle;
        if (TA_id == TA_myID)
        {
            emit renameBoxSuccess(TA_title, TA_newTitle);
        }
        break;
    }
    case MOVE_FRIEND_BOX_SUCCESS:
    {
        in >> TA_id >> TA_peerID >> TA_title >> TA_newTitle;
        if (TA_id == TA_myID)
        {
            emit moveFriendToBoxSuccess(TA_peerID, TA_title, TA_newTitle);
        }
        break;
    }

//    case GET_HISTORY_MESSAGE_SUCCESS:
//    {
//        in >> TA_id >> TA_peerID >> TA_date >> len;
//        TA_HistoryMessageVector.clear();
//        TA_HistoryMessageVector.resize(len);
//        for(int i=0; i<len; i++)
//        {
//            in >> TA_HistoryMessageVector[i];
//        }
//        qDebug() << "history message len: " << len;
//        if (TA_id == TA_myID)
//        {
//            emit getNetworkHistoryMessagesSuccess(TA_peerID, TA_date,
//                                           TA_HistoryMessageVector);
//        }
//        break;
//    }
    case CHANGE_PWD_FAIL:
    {
        QMessageBox::information(NULL, tr("修改密码"), tr("修改密码失败！"));
        break;
    }
    case CHANGE_PWD_SUCCESS:
    {
        TA_tcpSocket->setFlag(1);
        QMessageBox::information(NULL, tr("修改密码"), tr("修改成功！请重新登录"));
        emit closeWindowSignal();
        TALoginWidget *loginWidget = new TALoginWidget;
        loginWidget->show();
        break;
    }
    case GET_OFFLINE_MESSAGE_SUCCESS:
    {
        in >> len;
        TA_OfflineMessageVector.clear();
        TA_OfflineMessageVector.resize(len);
        for(int i=0; i<len; i++)
        {
            in >> TA_OfflineMessageVector[i];
            emit getFriendTalkMessage(TA_OfflineMessageVector[i]);
        }
        break;
    }
    default:
        break;
    }
    TA_blockSize = 0;
    QByteArray data = TA_tcpSocket->readAll();
    qDebug() << "leaved in socket: " << data.size();
}
