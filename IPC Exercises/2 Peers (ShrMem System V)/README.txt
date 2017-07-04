- Chương trình thử nghiệm dùng semaphore và shared memory của System V API:
Giữa 2 tiến trình peer1 và peer2 sẽ dùng một vùng nhớ chung để có thể đọc, ghi trên cùng một mảng số nguyên. 
Tiến trình peer1 cho phép người dùng nhập vào số lượng phần tử mảng và từng phần tử, rồi thiết lập semaphore 
cho peer2 thực hiện, sau đó đứng chờ  tiến trình peer2 thực hiện việc sắp xếp theo thứ tự tăng dần. Sau khi 
peer2 sắp xếp xong thiết lập semaphore cho peer1 thực hiện việc in kết quả ra màn hình còn peer2 thì in kết 
quả ra file res.txt  