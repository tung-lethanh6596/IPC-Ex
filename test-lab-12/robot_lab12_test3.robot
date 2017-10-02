*** Setting ***
Library    ${CURDIR}${/}../lab12/py-lab12-test.py

*** Variables ***
${ret}    nothing
${true}=    Convert To Boolean    True

*** Test Cases ***
Handles truncated segments
    ${ret}=    Test Case    num_testcase=11
    Should Be Equal    ${ret}    ${true}
Sends FIN when reading in EOF
    ${ret}=    Test Case    num_testcase=12
    Should Be Equal    ${ret}    ${true}
Tears down connection
    ${ret}=    Test Case    num_testcase=13
    Should Be Equal    ${ret}    ${true}
Handles sliding window
    ${ret}=    Test Case    num_testcase=14
    Should Be Equal    ${ret}    ${true}
