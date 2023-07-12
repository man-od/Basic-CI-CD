#!/bin/bash

if [[ $CI_JOB_STATUS == 'success' ]]; then
    CI_JOB_STATUS="SUCCESS"
else
    CI_JOB_STATUS="FAILED"
fi

API_KEY="######################################"
USER_ID="################"
URL="https://api.telegram.org/bot${API_KEY}/sendMessage"
MESSAGE="$CI_COMMIT_AUTHOR%0A%0AJob name: $CI_JOB_NAME%0A%0AStatus:+$CI_JOB_STATUS%0A%0AProject:+$CI_PROJECT_NAME"

curl -s -d "chat_id=$USER_ID&disable_web_page_preview=1&text=$MESSAGE" $URL > /dev/null

