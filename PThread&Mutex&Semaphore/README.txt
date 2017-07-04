*** Chương trình sử dụng POSIX threads và mutex/semaphore:

Chương trình bao gồm tất cả 12 threads được tạo ra, trong đó:
- 10 threads dùng để đọc dữ liệu trong thư mục data, mỗi thread đọc 1 file (các file từ 0->9), dữ liệu là
thông tin sinh viên, mỗi dòng là mã số sinh viên kèm theo tên sinh viên.
+ Ví dụ 1 dòng: 20145095 Le Thanh Tung
+ 10 threads này đọc và lưu dữ liệu vào mảng global tên là "students", lưu số lượng vào biến global tên là 
"stNumber". Khi cập nhật mảng students và vào biến stNumber, sử dụng mutex để lock/unlock
+ Ngoài ra còn có biến global khác tên là "num_threads_done" đếm số lượng luồng đọc file đã thực hiện xong, 
khi cập nhật cho biến này sử dụng semaphore tên là "building_array" để lock/unlock. Khi 10 luồng đọc file 
đã thực hiện xong hết thì unlock cho semaphore "finished_building_array" để 2 luồng cuối sau đó 
mới được thực hiện
- 2 thread cuối bao gồm 1 thread thực hiện việc sắp xếp sinh viên theo tên, sắp xếp trên mảng students đã có 
ở trên , rồi ghi kết quả vào file arrangeWithName.txt, tiếp đến unlock cho semaphore "finished_arranging_array"; 
+ 1 thread khác khi semaphore "finished_arranging_array" đã unlocked thì làm nhiệm vụ đếm số lượng sinh viên 
theo từng niên khóa nhập học và rồi lưu ra file countOn.txt
