#include "talocalmessagewidget.h"

TALocalMessageWidget::TALocalMessageWidget(TAMainWidget *mainWidget, QWidget *parent) :
    TAMessageWidget(mainWidget, parent), m_mainWidget(mainWidget)
{
    if (m_mainWidget == NULL)
    {
        return;
    }
    setShowMode(LOCAL);
}


 TALocalMessageWidget::~TALocalMessageWidget()
 {
 }



 // public slots:------------------------------------

 /*************************************************
 Function Name： onClickDeleteDate()
 Description:  删除某日的信息记录
 *************************************************/
 void TALocalMessageWidget::onClickDeleteDate()
 {
 }

 /*************************************************
 Function Name： onClickDeleteOne()
 Description:  删除某好友信息记录
 *************************************************/
 void TALocalMessageWidget::onClickDeleteOne()
 {
     int row = m_listViewFriend->currentIndex().row();
     if (row < 0)
     {
         QString temp = QString(tr("请先选择好友！"));
         QMessageBox::information(NULL, tr("删除聊天记录"), temp);
         return;
     }
     if (NULL == m_mainWidget)
         return;
     m_mainWidget->deleteLocalMessage(m_indexOfFriendList);
 }

 /*************************************************
 Function Name： onClickDeleteAll()
 Description:  删除全部消息记录
 *************************************************/
 void TALocalMessageWidget::onClickDeleteAll()
 {
     if (NULL == m_mainWidget)
         return;
     m_mainWidget->deleteLocalMessageAll();
 }

 /*************************************************
 Function Name： onClickImport()
 Description:  导人消息记录
 *************************************************/
 void TALocalMessageWidget::onClickImport()
 {
 }

 /*************************************************
 Function Name： onClickExportOne()
 Description:  导出当前好友消息记录
 *************************************************/
 void TALocalMessageWidget::onClickExportOne()
 {
     int row = m_listViewFriend->currentIndex().row();
     if (row < 0)
     {
         QString temp = QString(tr("请先选择好友！"));
         QMessageBox::information(NULL, tr("导出聊天记录"), temp);
         return;
     }
     if (NULL == m_mainWidget)
         return;
     m_mainWidget->exportLocalMessage(m_indexOfFriendList);
 }

 /*************************************************
 Function Name： onClickExportAll()
 Description:  导出所有消息记录
 *************************************************/
 void TALocalMessageWidget::onClickExportAll()
 {
     if (NULL == m_mainWidget)
         return;
     m_mainWidget->exportLocalMessageAll();
 }


 /*************************************************
 Function Name： onClickGetMessage()
 Description:  获取消息记录
 *************************************************/
 void TALocalMessageWidget::onClickGetMessage()
 {
//     if (m_listViewFriend->isSe)
     int row = m_listViewFriend->currentIndex().row();
     if (row < 0)
     {
         QString temp = QString(tr("请先选择好友！"));
         QMessageBox::information(NULL, tr("显示聊天记录"), temp);
         return;
     }
     m_date = m_leBirthday->getDate();
     qDebug() << "get  date:" <<  m_date;
     m_mainWidget->getLocalHistoryMessage(m_indexOfFriendList, m_date);
 }


// /*************************************************
// Function Name： friendItemClicked()
// Description:  单击好友列表项
// *************************************************/
// void TALocalMessageWidget::friendItemClicked(const QModelIndex & index)
// {
//     qDebug() << "friend:" << index.row() << ":" << index.data().toString();
//     emit currentIndexClicked(index.row());
// }

