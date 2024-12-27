#define _GNU_SOURCE
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <pwd.h>
#include <sys/types.h>
#include <cstring>

void sigusr1_handler(int signum, siginfo_t* info, void* context) {
    if (signum != SIGUSR1) {
        return;
    }

    pid_t sender_pid = info->si_pid;
    uid_t sender_uid = info->si_uid;
    struct passwd* pwd = getpwuid(sender_uid);
    const char* username = pwd ? pwd->pw_name : "unknown";

    ucontext_t* uctx = static_cast<ucontext_t*>(context);
    uintptr_t eip = uctx->uc_mcontext.gregs[REG_RIP];
    uintptr_t eax = uctx->uc_mcontext.gregs[REG_RAX];
    uintptr_t ebx = uctx->uc_mcontext.gregs[REG_RBX];

    std::cout << "Received a SIGUSR1 signal from process " << sender_pid
              << " executed by " << sender_uid << " (" << username << ")." << std::endl;
    std::cout << "State of the context: EIP = 0x" << std::hex << eip
              << ", EAX = 0x" << eax << ", EBX = 0x" << ebx << std::dec << "." << std::endl;
}

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sigusr1_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, nullptr) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    std::cout << "Process PID: " << getpid() << std::endl;

    while (true) {
        sleep(10);
    }

    return EXIT_SUCCESS;
}

