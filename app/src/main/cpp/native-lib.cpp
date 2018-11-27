#include "native_lib.h"

const char *PATH = "/data/data/com.example.chen.ndktest/my.sock";
int m_child_connfd;
const char *m_userId;
extern "C" JNIEXPORT jstring
JNICALL
Java_com_example_chen_ndktest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_chen_ndktest_Watcher_createWatcher(JNIEnv *env, jobject instance,
                                                    jstring userId_) {
    m_userId = env->GetStringUTFChars(userId_, 0);

    //开双进程
    pid_t pid = fork();
    if (pid < 0) {

    } else if (pid == 0) {
        //子进程 守护进程
        child_do_work();
    } else if (pid > 0) {
        //父进程

    }

    env->ReleaseStringUTFChars(userId_, m_userId);
}

void child_do_work() {
    //开启socket 服务端
    if (child_create_channel()) {
        child_listen_msg();
    }

}

/**
 * 服务端读取信息
 *
 */
void child_listen_msg() {
    fd_set rfds;
    struct timeval timeout = {3, 0};
    while (1) {
        FD_ZERO(&rfds);
        FD_SET(m_child_connfd, &rfds);
        int r = select(m_child_connfd + 1, &rfds, NULL, NULL, &timeout);
        LOGE("读取消息前 %d", r);
        if (r > 0) {
            //缓冲区
            char pak[256] = {0};
            //保证所读到的信息是指定的apk客服端的
            if (FD_ISSET(m_child_connfd, &rfds)) {
                //阻塞读
                int result = read(m_child_connfd, pak, sizeof(pak));
                //开启服务
                execl("am", "am", "startservice", "--user", m_userId,
                      "com.example.chen.ndktest/com.example.chen.ndktest.ProgressService",
                      (char *) NULL);
                break;
            }
        }
    }
}

/**
 * 创建服务端的socket
 * @return
 */
int child_create_channel() {
    int listenFd = socket(AF_LOCAL, SOCK_STREAM, 0);
    unlink(PATH);
    struct sockaddr_un addr;
    //清空内存
    memset(&addr, 0, sizeof(sockaddr_un));
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, PATH);
    if (bind(listenFd, (const sockaddr *) &addr, sizeof(sockaddr_un)) < 0) {
        LOGE("绑定错误");
        return 0;
    }
    LOGE("绑定成功");
    listen(listenFd, 5);

    //保证 宿主进程链接成功
    while (1) {
        //返回值 客服端的地址
        LOGE("开始 accept");
        m_child_connfd = accept(listenFd, NULL, NULL);
        if (m_child_connfd < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                LOGE("读取错误");
                return 0;
            }
        }
        LOGE("服务端 连接成功...");
        break;

    }
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_chen_ndktest_Watcher_connectMonitor(JNIEnv *env, jobject instance) {
    int socked;
    struct sockaddr_un addr;
    while (1) {
        LOGE("客服端 父进程开始连接");
        socked = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (socked < 0) {
            LOGE("客服端 父进程连接失败");
            return;
        }
        memset(&addr, 0, sizeof(sockaddr_un));
        addr.sun_family = AF_LOCAL;
//        addr.sun_path
        strcpy(addr.sun_path, PATH);
        if (connect(socked, (const sockaddr *) &addr, sizeof(sockaddr_un)) < 0) {
            LOGE("连接失败");
            close(socked);
            sleep(1);
            continue;
        }
        LOGE("客服端 连接成功");
        break;
    }

}