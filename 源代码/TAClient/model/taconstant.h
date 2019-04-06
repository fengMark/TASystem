#ifndef TACONSTANT_H
#define TACONSTANT_H

#include <QObject>
#include<QDate>
#include<QDateTime>
#include<QDataStream>
static const QString GROUP_NAME_DEFAULT = QString(QObject::tr("New Friends"));
//群成员身份
enum FlockStatus
{
    FLOCK_HOST = 0,    // 群主
    FLOCK_ADMAIN = 1,  // 群管理员
    FLOCK_MEMBER = 2   // 群成员
};
enum MessageType
{
    /***********************client to server************************/
    CHECK_CONNECTION,           // 检查是否能与服务器连接
    LOGIN,                      // 登录
    REGISTER,                   // 注册
    GET_QUESTION_ANSWER,    // 获取密保问题以及答案
    GET_QUESTION_ANSWER_SUCCESS,
    GET_QUESTION_ANSWER_FAIL,

    CHECK_MESSAGE,              // 检查有无给自己的信息（登录时检查）

    GET_ALL_FRIENDS,            // 获取所有好友信息
    GET_ALL_FLOCKS,             // 获取所有群信息
    GET_ALL_DISCUSSIONS,        // 获取所有讨论组信息
    GET_FLOCK_MEMBERS,          // 获取群成员
    GET_DISCUSSIONS_MEMBERS,    // 获取讨论组成员

    TALK,				// 发送对话
    ADD_FRIEND,                 // 添加好友



    ADD_FLOCK,                   // 用户加群
    ADD_DISCUSSION,

    FLOCK_ADD_MEMBER,            // 群加用户

    GET_FRIEND_INFORMATION,		// 获取好友信息
    DELETE_FRIEND,				// 删除好友
    GET_MYSELF_INFORMATION,		// 获取自己信息
    CHANGE_INFORMATION,			// 改变自己的信息
    CHANGE_REMARK,				// 改变（好友）备注
    CHANGE_PASSWORD,			    // 修改密码
    CHANGE_STATUE,				// 改变自身状态
    // 还有分组好友管理
    QUIT,						// 退出

    /***********************server to client************************/

    LOGIN_SUCCESS ,				// 登录成功
    LOGIN_FAIL,			        // 登录失败
    HAVE_LOGINED,				// 已经登录
    REGISTER_SUCCESS,	        // 注册成功
    REGISTER_FAIL,			    // 注册失败

    REMOVE_BOX,               // 删除分组
    REMOVE_BOX_SUCCESS,
    REMOVE_BOX_FAIL,
    RENAME_BOX,               // 重命名分组
    RENAME_BOX_SUCCESS,
    RENAME_BOX_FAIL,
    CREATE_BOX,               // 创建分组
    CREATE_BOX_SUCEESS,
    CREATE_BOX_FAIL,
    MOVE_FRIEND_BOX,          // 移动好友至其他分组  36
    MOVE_FRIEND_BOX_SUCCESS,
    MOVE_FRIEND_BOX_FAIL,


    GET_ALL_FRIENDS_SUCCESS,    // 获取所有好友信息成功
    GET_ALL_FRIENDS_FAIL,       // 获取所有好友信息失败
    NO_FRIEND,                  // 没有好友
    GET_ALL_FLOCKS_SUCCESS,     // 获取所有群信息成功
    GET_ALL_FLOCKS_FAIL,        // 获取所有群信息失败
    NO_FLOCK,
    GET_ALL_DISCUSSIONS_SUCCESS,// 获取所有讨论组信息成功
    GET_ALL_DISCUSSIONS_FAIL,   // 获取所有讨论组信息失败
    NO_DISCUSSION,

    GET_FLOCK_MEMBERS_FAIL,           // 获取群成员失败
    GET_FLOCK_MEMBERS_SUCCESS,        // 获取群成员成功
    NO_FLOCK_MEMBER,                // 没有群成员
    GET_DISCUSSION_MEMBERS_FAIL,     // 获取讨论组成员失败
    GET_DISCUSSION_MEMBERS_SUCCESS,  // 获取讨论组成员成功
    NO_DISCUSSION_MEMBER,          // 没有讨论组成员

    NO_MESSAGE,				    // 没有信息
    HAVE_MESSAGE,				// 有信息
    HAVE_TALK_MESSAGE,          // 有聊天信息
    //？？？？？？？？？？？？？？？？
    FRIEND_REQUESTED,			// 好友请求
    FRIEDN_NO_ACCOUNT,			// 没有这个用户名
    ALREAD_FRIENDS,			    // 回应好友请求

    GET_FRIEND_FAIL,
    GET_FRIEND_SUCCESS,			// 获得好友资料成功
    DELETE_FRIEND_FAIL,
    DELETE_FRIEND_SUCCESS,		// 删除好友成功
    GET_MYSELF_FAIL,
    GET_MYSELF_SUCCESS,		    // 获得自己详细资料成功
    CHANGE_INFORMATION_FAIL,
    CHANGE_INFORMATION_SUCCESS,	// 改变自身资料成功

    CHANGE_REMARK_FAIL,             // 修改备注失败
    CHANGE_REMARK_SUCCESS,		// 修改备注成功

    OLD_PWD_IS_WRONG,			// 旧密码是错的
    CHANGE_PWD_FAIL,              // 修改密码失败
    CHANGE_PWD_SUCCESS,			// 修改密码成功

    /***********************client to client************************/

    REQUEST_FRIEND,	     // 请求添加好友
    AGREE_FRIEND,	     // 同意好友请求
    REFUSE_FRIEND,		 // 不同意好友请求
    ADD_FRIEND_FAIL,      // 添加好友失败
    ADD_FRIEND_SUCCESS,   // 添加好友成功
    END_ADD_FRIEND,	     // 好友添加结束
    BE_DELETED,		     // 自己被好友删除了

    MESSAGE,				 // 普通信息
    TALK_MESSAGE,         // 聊天信息

    REQUEST_FLOCK,   // 用户请求加入群
    FLOCK_AGREE,    // 群主同意用户加入
    FLOCK_REFUSE,   // 群主拒绝用户加入
    ADD_FLOCK_FAIL,             // 加入群失败
    ADD_FLOCK_SUCCESS,          // 加入群成功
    NEW_FLOCK_MEMBER,           // 新成员入群
    LEAVE_FLOCK_MEMBER,


    FLOCK_REQUEST,  // 群主请求用户加入群
    AGREE_FLOCK,    // 用户同意加入群
    REFUSE_FLOCK,   // 用户拒绝加入群


    CREATE_FLOCK,               // 创建群
    CREATE_FLOCK_FAIL,
    CREATE_FLOCK_SUCCESS,

    CREATE_DISCUSSION,               // 创建讨论组
    CREATE_DISCUSSION_FAIL,
    CREATE_DISCUSSION_SUCCESS,

    DROP_FLOCK,                 // 解散群
    DROP_FLOCK_FAIL,            // 解散群失败
    DROP_FLOCK_SUCCESS,         // 解散群成功

    DROP_DISCUSSION,                 // 解散讨论组
    DROP_DISCUSSION_FAIL,            // 解散讨论组失败
    DROP_DISCUSSION_SUCCESS,         // 解散讨论组成功

    LEAVE_FLOCK,              // 退出群
    LEAVE_FLOCK_SUCCESS,      // 退出群成功
    LEAVE_FLCCK_FAIL,         // 退出群失败
    LEAVE_DISCUSSION,           // 退出讨论组
    LEAVE_DISCUSSION_FAIL,       // 退出讨论组失败
    LEAVE_DISCUSSION_SUCCESS,   // 退出讨论组成功


    ADD_DISCUSSION_FAIL,
    ADD_DISCUSSION_SUCCESS,
    NEW_DISCUSSION_MEMBER,      // 新成员入讨论组
    LEAVE_DISCUSSION_MEMBER,

    CHANGE_FLOCK,               // 修改群信息
    CHANGE_FLOCK_SUCCESS,
    CHANGE_FLOCK_FAIL,

    CHANGE_FLOCK_REMARK,             // 修改群成员备注
    CHANGE_FLOCK_REMARK_FAIL,
    CHANGE_FLOCK_REMARK_SUCCESS,

    CHANGE_DISCUSSION,               // 修改讨论组信息
    CHANGE_DISCUSSION_SUCCESS,
    CHANGE_DISCUSSION_FAIL,

    TALK_FLOCK,               // 群对话
    TALK_DISCUSSION,          // 讨论组对话

    REQUEST_VIDEO,
    REFUSE_VIDEO,
    END_VIDEO,
    REQUEST_VOICE,
    AGREE_VOICE,
    REFUSE_VOICE,
    END_VOICE,
    REQUEST_FILE,
    AGREE_FILE,
    REFUSE_FILE,
    END_FILE,


    GET_OFFLINE_MESSAGE_SUCCESS,
    GET_OFFLINE_MESSAGE,
       UPLOAD_ICONFILE,
    };
// 登录状态
enum LoginStatus
{
    ONLINE = 0,    // 在线
    INVISIBLE = 1, // 隐身
    BUSY = 2,      // 忙碌
    LEAVE = 3,     // 离开
    NOTDISTURB = 4,// 请勿打扰
    OFFLINE = 5
};
// 好友信息
typedef struct FriendInformation
{
    int TA_headPortrait;
    QString TA_userID;
    QString TA_nickname;
    QString TA_groupName;
    QString TA_remarkName;
    int TA_status;

    FriendInformation()
    {
       TA_status = OFFLINE;
       TA_headPortrait = 1;
       TA_groupName = GROUP_NAME_DEFAULT;
    }

    //重载输入输出操作符
    friend QDataStream & operator<< (QDataStream &out,
                                    const FriendInformation &fri)
    {
       out << fri.TA_headPortrait << fri.TA_userID << fri.TA_nickname
           << fri.TA_groupName << fri.TA_remarkName << fri.TA_status;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, FriendInformation &fri)
    {
       in >> fri.TA_headPortrait >> fri.TA_userID >> fri.TA_nickname
          >> fri.TA_groupName >> fri.TA_remarkName >> fri.TA_status;
       return in;
    }
}FriendInformation;


// 群成员信息
struct FlockMember
{
    QString TA_flockID;     // 群ID
    QString TA_userID;      // 用户ID
    int TA_headPortrait;    // 用户头像编号
    QString TA_nickname;    // 用户昵称
    int TA_loginStatus;     // 用户登录状态
    QString TA_remark;      // 用户群名片
    int TA_flockStatus;     // 用户身份
    QDateTime TA_joinDate;  // 加入时间

    friend QDataStream & operator<< (QDataStream &out, const FlockMember &flockMember)
    {
       out << flockMember.TA_flockID << flockMember.TA_userID
           << flockMember.TA_headPortrait << flockMember.TA_nickname
           << flockMember.TA_loginStatus << flockMember.TA_remark
           << flockMember.TA_flockStatus << flockMember.TA_joinDate;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, FlockMember &flockMember)
    {
       in >> flockMember.TA_flockID >> flockMember.TA_userID
          >> flockMember.TA_headPortrait >> flockMember.TA_nickname
          >> flockMember.TA_loginStatus >> flockMember.TA_remark
          >> flockMember.TA_flockStatus >> flockMember.TA_joinDate;
       return in;
    }
};

// 讨论组成员信息
struct DiscussionMember
{
    QString TA_discussionID;// 讨论组ID
    QString TA_userID;      // 用户ID
    int TA_headPortrait;    // 用户头像编号
    QString TA_nickname;    // 用户昵称
    int TA_loginStatus;     // 用户登录状态
    int TA_discussionStatus;// 用户身份
    QDateTime TA_joinDate;  // 加入时间

    friend QDataStream & operator<< (QDataStream &out, const DiscussionMember &disMember)
    {
       out << disMember.TA_discussionID << disMember.TA_userID
           << disMember.TA_headPortrait << disMember.TA_nickname
           << disMember.TA_loginStatus << disMember.TA_discussionStatus
           << disMember.TA_joinDate;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, DiscussionMember &disMember)
    {
       in >> disMember.TA_discussionID >> disMember.TA_userID
          >> disMember.TA_headPortrait >> disMember.TA_nickname
          >> disMember.TA_loginStatus  >> disMember.TA_discussionStatus
          >> disMember.TA_joinDate;
       return in;
    }
};

// 群信息
typedef struct FlockInformation
{
    QString TA_flockID;
    QString TA_creatorID;
    QDate TA_createDate;
    QString TA_flockName;
    QString TA_theme;

    friend QDataStream & operator<< (QDataStream &out, const FlockInformation &flock)
    {
       out << flock.TA_flockID << flock.TA_creatorID << flock.TA_createDate
           << flock.TA_flockName << flock.TA_theme;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, FlockInformation &flock)
    {
       in >> flock.TA_flockID >> flock.TA_creatorID >> flock.TA_createDate
          >> flock.TA_flockName >> flock.TA_theme;
       return in;
   }
}FlockInformation;
// 用户详细信息
typedef struct UserInformation
{
    int TA_headPortrait;
    QString TA_userID;
    QString TA_nickname;
    QString TA_password;
    QString TA_sex;
    QString TA_question;
    QString TA_answer;
    QString TA_phone;
    QDateTime TA_regDateTime;
    int TA_status;

    UserInformation()
    {
       TA_status = OFFLINE;
       TA_headPortrait = 1;
    }
    // 重载输入输出操作符
    friend QDataStream &operator<<(QDataStream &out, const UserInformation &user)
    {
       out << user.TA_headPortrait << user.TA_userID << user.TA_nickname
           << user.TA_password << user.TA_sex<< user.TA_question
           << user.TA_answer << user.TA_phone
           << user.TA_regDateTime<< user.TA_status;
       return out;
    }

    friend QDataStream &operator>>(QDataStream &in, UserInformation &user)
    {
       in >> user.TA_headPortrait >> user.TA_userID >> user.TA_nickname
          >> user.TA_password >> user.TA_sex
          >> user.TA_question >> user.TA_answer
          >>  user.TA_phone >>  user.TA_regDateTime
          >> user.TA_status;
       return in;
    }
}UserInformation;
// 储存在各种小窗口中填写的信息
typedef struct TempStrings
{
    QString TA_one;
    QString TA_two;
    QString TA_three;
    friend QDataStream & operator<< (QDataStream &out, const TempStrings &str)
    {
       out << str.TA_one << str.TA_two << str.TA_three;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, TempStrings &str)
    {
       in >> str.TA_one >> str.TA_two >> str.TA_three;
       return in;
    }
}TempStrings;
// 聊天框信息
typedef struct ChatInformation
{
     QString TA_myID;
    int TA_headPortrait;
    QString TA_friendID;          // 好友id
    QString TA_friendNickname;    // 好友昵称
    QString TA_friendRemark;      // 好友备注
    QString TA_friendMark;        // 好友签名
    QString TA_friendIP;          // 好友ip地址
    int TA_friendStatus;

    ChatInformation()
    {
       TA_friendStatus = OFFLINE;
       TA_headPortrait= 1;
       TA_friendMark = QString(QObject::tr("好友签名"));
    }

//    //重载输入输出操作符
//    friend QDataStream & operator<< (QDataStream &out, const ChatInformation &chat)
//    {
//       out << chat.TA_headPortrait << chat.TA_friendID << chat.TA_friendNickname
//           << chat.TA_friendRemark << chat.TA_friendMark << chat.TA_friendIP;
//       return out;
//    }
//    friend QDataStream & operator>> (QDataStream &in, ChatInformation &chat)
//    {
//       in >> chat.TA_headPortrait >> chat.TA_friendID >> chat.TA_friendNickname
//          >> chat.TA_friendRemark >> chat.TA_friendMark >> chat.TA_friendIP;
//       return in;
//    }
}ChatInformation;

// 对话信息
typedef struct TalkMessage
{
    int TA_type;               // 消息类型
    QString TA_senderID;       // 发送者ID
    QString TA_senderIP;       // 发送者IP
    QString TA_receiverID;     // 接收者ID
    QString TA_receiverIP;     // 接收者IP
    QString TA_text;           // 消息内容
    QDateTime TA_dateTime;

    TalkMessage() {	TA_type = 0; }

    friend QDataStream & operator<< (QDataStream &out, const TalkMessage &mes)
    {
       out << mes.TA_type << mes.TA_senderID << mes.TA_senderIP << mes.TA_receiverID
           << mes.TA_receiverIP << mes.TA_text << mes.TA_dateTime;
       return out;
    }

    friend QDataStream & operator>> (QDataStream &in, TalkMessage &mes)
    {
       in >> mes.TA_type >> mes.TA_senderID >> mes.TA_senderIP >> mes.TA_receiverID
          >> mes.TA_receiverIP >> mes.TA_text >>  mes.TA_dateTime;
       return in;
    }
}TalkMessage;

// 用户登录信息
typedef struct LoginInformation
{
    QString TA_userID;
    QString TA_password;
    int TA_status;

    LoginInformation()
    {	TA_status = OFFLINE;	}
    friend QDataStream & operator<< (QDataStream &out, const LoginInformation &li)
    {
       out << li.TA_userID << li.TA_password << li.TA_status;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, LoginInformation &li)
    {
       in >> li.TA_userID >> li.TA_password >> li.TA_status;
       return in;
    }
}LoginInformation;
//头像尺寸.
const int HEAD_BIG_SIZE = 40;
const int HEAD_SMALL_SIZE = 35;
const int HEAD_MINI_SIZE = 20;

//聊天室 工具栏尺寸
const int CHAT_WIDGET_TOOL_SIZE = 20;
const int CHAT_WIDGET_TOOL_SMALL_SIZE = 15;

const int FONT_SIZE = 2;

static const QString MAIL_ADDRESS_FORMAT = "@im.com";

static const QString LOCAL_HISTORY_MESSAGE_PATH = "resource/history/local/";
static const QString NETWORK_HISTORY_MESSAGE_PATH = "resource/history/network/";
#endif // TACONSTANT_H
