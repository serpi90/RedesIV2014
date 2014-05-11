#include "Sampler.h"
#include <signal.h>

int main()
{
    // Prevent zombie processes
    struct sigaction sigchld_action;
    sigchld_action.sa_handler = SIG_DFL;
    sigchld_action.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sigchld_action, NULL);
    Sampler s;
    s.init();
    return 0;
}
