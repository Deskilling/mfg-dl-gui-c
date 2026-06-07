#include <stdio.h>

#include "request/requests.h"
#include "request/client.h"

#include "src/request/api/api.h"

int main() {
    CURL* curl = initClient();
    if (!curl) {
        fprintf(stderr, "Failed to init curl\n");
        return 1;
    }

    Response r = postHealth(curl);

    freeResponse(&r);

    cleanupClient(curl);
    return 0;
}
