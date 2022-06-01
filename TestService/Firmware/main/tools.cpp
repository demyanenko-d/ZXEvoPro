#include "tools.hpp"
#include <stdio.h>

void esp_pass_fail(const char *msg, esp_err_t err)
{
    printf(msg);
    (err == ESP_OK) ? printf("[PASS]\r\n") : printf("[FAIL]\r\n");
}