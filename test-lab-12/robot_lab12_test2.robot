*** Setting ***
Library    ${CURDIR}${/}../lab12/py-lab12-test.py

*** Variables ***
${ret}    nothing
${true}=    Convert To Boolean    True

*** Test Cases ***
Handles data larger than window size
    ${ret}=    Test Case    num_testcase=6
    Should Be Equal    ${ret}    ${true}
Handles segment corruption
    ${ret}=    Test Case    num_testcase=7
    Should Be Equal    ${ret}    ${true}
Handles segment drops
    ${ret}=    Test Case    num_testcase=8
    Should Be Equal    ${ret}    ${true}
Handles segment delay
    ${ret}=    Test Case    num_testcase=9
    Should Be Equal    ${ret}    ${true}
Handles duplicate segments
    ${ret}=    Test Case    num_testcase=10
    Should Be Equal    ${ret}    ${true}
