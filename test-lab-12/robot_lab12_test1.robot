*** Setting ***
Library    ${CURDIR}${/}../lab12/py-lab12-test.py

*** Variables ***
${ret}    nothing
${true}=    Convert To Boolean    True

*** Test Cases ***
Client sends data
    ${ret}=    Test Case    num_testcase=1
    Should Be Equal    ${ret}    ${true}
Client receives data
    ${ret}=    Test Case    num_testcase=2
    Should Be Equal    ${ret}    ${true}
Correct checksum
    ${ret}=    Test Case    num_testcase=3
    Should Be Equal    ${ret}    ${true}
Correct header fields
    ${ret}=    Test Case    num_testcase=4
    Should Be Equal    ${ret}    ${true}
Bidirectionally transfer data
    ${ret}=    Test Case    num_testcase=5
    Should Be Equal    ${ret}    ${true}
